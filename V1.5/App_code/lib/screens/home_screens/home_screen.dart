import 'package:dio/dio.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/screens/New_data_hub/Start_connect_data_hub.dart';
import 'package:flutter_app1/screens/auth/welcome_screen.dart';
import 'package:flutter_app1/screens/home_screens/Camera_screen.dart';
import 'package:flutter_app1/screens/home_screens/Iot_screen.dart';
import 'package:flutter_app1/screens/home_screens/pressure_screen.dart';
import 'package:flutter_app1/screens/home_screens/vital_sign_screen.dart';
import 'dart:io';

import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';
import 'package:path_provider/path_provider.dart';
import 'package:permission_handler/permission_handler.dart';

class home_page extends StatefulWidget {
  const home_page({Key? key}) : super(key: key);

  @override
  State<home_page> createState() => _home_pageState();
}

class _home_pageState extends State<home_page> {
  var imageUrl = "http://20.120.242.95:65080/reporte/?user=";
  bool downloading = true;
  String downloadingStr = "No data";
  String savePath = "";
  final FirebaseAuth auth = FirebaseAuth.instance;
  final database = FirebaseDatabase.instance.ref();
  bool buttonactive = true;
  String _buttontext = "Desvincular datahub";
  Future<bool> _onWillPop() async {
    return (await showDialog(
          context: context,
          builder: (context) => new AlertDialog(
            title: new Text('¿Estas seguro?'),
            content: new Text('¿Quieres salir de la app?'),
            actions: <Widget>[
              TextButton(
                onPressed: () => Navigator.of(context).pop(false),
                child: new Text('No'),
              ),
              TextButton(
                onPressed: () => exit(0),
                child: new Text('Si'),
              ),
            ],
          ),
        )) ??
        false;
  }

  void del_datahub() async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    final vdatabase =
        database.child("VitalMonitor").child("Tokens").child(uid.toString());
    final sing_up_data = <String, dynamic>{
      "Conf": "D",
      "Dir": "False",
    };
    await vdatabase.update(sing_up_data).then((value) {
      print("value");
    }).onError((error, stackTrace) {
      print("Error ${error.toString()}");
    });
    await FirebaseAuth.instance.signOut();
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => (welcome_page())));
  }

  @override
  void initState() {
    super.initState();
  }

  Future<String?> getDownloadPath() async {
    Directory? directory;
    try {
      if (Platform.isIOS) {
        directory = await getApplicationDocumentsDirectory();
      } else {
        directory = Directory('/storage/emulated/0/Download');
        // Put file in global download folder, if for an unknown reason it didn't exist, we fallback
        // ignore: avoid_slow_async_io
        if (!await directory.exists())
          directory = await getExternalStorageDirectory();
      }
    } catch (err, stack) {
      print("Cannot get download folder path");
      return null;
    }
    return directory?.path;
  }

  Future downloadFile() async {
    final user = await auth.currentUser;
    final uid = user?.uid;

    imageUrl = imageUrl + uid.toString();
    await showDialog(
      context: context,
      builder: (context) => new AlertDialog(
        title: new Text('Reporte guardado en '),
        content: new Text(imageUrl),
      ),
    );
    try {
      Map<Permission, PermissionStatus> statuses = await [
        Permission.storage,
        //add more permission to request here.
      ].request();

      if (statuses[Permission.storage]!.isGranted) {
        var dir = await getDownloadPath();
        if (dir != null) {
          String savename = "reporte.xlsx";
          String savePath = dir + "/$savename";
          print(savePath);
          //output:  /storage/emulated/0/Download/banner.png

          try {
            await Dio().download(imageUrl, savePath,
                onReceiveProgress: (received, total) {
              if (total != -1) {
                print((received / total * 100).toStringAsFixed(0) + "%");
                //you can build progressbar feature too
              }
            });
            print("File is saved to download folder.");
            await showDialog(
              context: context,
              builder: (context) => new AlertDialog(
                title: new Text('Reporte guardado en '),
                content: new Text(savePath),
              ),
            );
          } on DioError catch (e) {
            print(e.message);
          }
        }
      } else {
        print("No permission to read and write.");
      }
    } catch (e) {
      print(e.toString());
    }
  }

  Future<String> getFilePath(uniqueFileName) async {
    String path = '';

    Directory dir = await getApplicationDocumentsDirectory();

    path = '${dir.path}/$uniqueFileName';

    return path;
  }

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
        onWillPop: _onWillPop,
        child: Scaffold(
          appBar: PreferredSize(
            preferredSize: const Size.fromHeight(120),
            child: Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                left: 24,
                top: 40,
                right: 24,
              ),
              width: double.maxFinite,
              decoration: const BoxDecoration(
                color: Color(0xFFFFFFFF),
                border: Border(
                  left: BorderSide(width: 0, color: Color(0xFF000000)),
                  top: BorderSide(width: 0, color: Color(0xFF000000)),
                  right: BorderSide(width: 0, color: Color(0xFF000000)),
                  bottom: BorderSide(width: 0, color: Color(0xFF000000)),
                ),
              ),
              child: Padding(
                padding: const EdgeInsets.only(
                  bottom: 20,
                ),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    GestureDetector(
                      child: Icon(
                        MdiIcons.fromString('logout'),
                        size: 32,
                        color: Color(0xFF000000),
                      ),
                    ),
                    Text(r'''VitalMonitor''',
                        style: GoogleFonts.outfit(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w600,
                            fontSize: 26,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.center,
                        maxLines: 1),
                    GestureDetector(
                      child: Icon(
                        MdiIcons.fromString('home'),
                        size: 32,
                        color: Color(0xFF000000),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ),
          backgroundColor: const Color(0xFF000000),
          body: Center(
              child: Container(
            margin: EdgeInsets.zero,
            padding: EdgeInsets.zero,
            width: double.maxFinite,
            height: double.maxFinite,
            decoration: const BoxDecoration(
              color: Color(0xFFFFFFFF),
              border: Border(
                left: BorderSide(
                    width: 0,
                    style: BorderStyle.none,
                    color: Color(0xFF000000)),
                top: BorderSide(
                    width: 0,
                    style: BorderStyle.none,
                    color: Color(0xFF000000)),
                right: BorderSide(
                    width: 0,
                    style: BorderStyle.none,
                    color: Color(0xFF000000)),
                bottom: BorderSide(
                    width: 0,
                    style: BorderStyle.none,
                    color: Color(0xFF000000)),
              ),
            ),
            child: SingleChildScrollView(
              child: Column(
                children: [
                  Container(
                    margin: EdgeInsets.zero,
                    padding: EdgeInsets.zero,
                    width: double.maxFinite,
                    decoration: const BoxDecoration(
                      color: Color(0xFFFFFFFF),
                      border: Border(
                        left: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                      ),
                    ),
                    child: Center(
                      child: ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          onSurface: Colors.blue,
                        ),
                        onPressed: buttonactive
                            ? () async {
                                del_datahub();
                              }
                            : null,
                        child: Text(_buttontext),
                      ),
                    ),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: EdgeInsets.zero,
                    width: double.maxFinite,
                    decoration: const BoxDecoration(
                      color: Color(0xFFFFFFFF),
                      border: Border(
                        left: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                      ),
                    ),
                    child: Padding(
                      padding: const EdgeInsets.only(
                        top: 20,
                        bottom: 20,
                      ),
                      child: Center(
                        child: Text(r'''Tablero''',
                            style: GoogleFonts.outfit(
                              textStyle: TextStyle(
                                color: const Color(0xFF000000),
                                fontWeight: FontWeight.w700,
                                fontSize: 36,
                                fontStyle: FontStyle.normal,
                                decoration: TextDecoration.none,
                              ),
                            ),
                            textAlign: TextAlign.center,
                            maxLines: 1),
                      ),
                    ),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: EdgeInsets.zero,
                    width: double.maxFinite,
                    decoration: const BoxDecoration(
                      color: Color(0xFFFFFFFF),
                      border: Border(
                        left: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                      ),
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        GestureDetector(
                          onTap: () {
                            Navigator.push(
                                context,
                                MaterialPageRoute(
                                    builder: (context) => vital_sign_screen()));
                          },
                          child: Container(
                            margin: const EdgeInsets.only(),
                            padding: EdgeInsets.zero,
                            width: 110,
                            height: 150,
                            decoration: BoxDecoration(
                              color: const Color(0xFF65A8FF),
                              borderRadius: BorderRadius.only(
                                topLeft: Radius.circular(15),
                                topRight: Radius.circular(15),
                                bottomRight: Radius.circular(15),
                                bottomLeft: Radius.circular(15),
                              ),
                              border: Border(
                                left: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                top: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                right: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                bottom: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                              ),
                            ),
                            child: Center(
                              child: Text(r'''Signos Vitales''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFFFFFFFF),
                                      fontWeight: FontWeight.w500,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.center,
                                  maxLines: 2),
                            ),
                          ),
                        ),
                        GestureDetector(
                          onTap: () {
                            Navigator.push(
                                context,
                                MaterialPageRoute(
                                    builder: (context) => pressure_screen()));
                          },
                          child: Container(
                            margin: EdgeInsets.zero,
                            padding: EdgeInsets.zero,
                            width: 110,
                            height: 150,
                            decoration: BoxDecoration(
                              color: const Color(0xFF65A8FF),
                              borderRadius: BorderRadius.only(
                                topLeft: Radius.circular(15),
                                topRight: Radius.circular(15),
                                bottomRight: Radius.circular(15),
                                bottomLeft: Radius.circular(15),
                              ),
                              border: Border(
                                left: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                top: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                right: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                bottom: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                              ),
                            ),
                            child: Center(
                              child: Text(r'''Presion Arterial''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFFFFFFFF),
                                      fontWeight: FontWeight.w500,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.center,
                                  maxLines: 2),
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: const EdgeInsets.only(
                      top: 20,
                    ),
                    width: double.maxFinite,
                    decoration: const BoxDecoration(
                      color: Color(0xFFFFFFFF),
                      border: Border(
                        left: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                      ),
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        GestureDetector(
                          onTap: () async {
                            final user = await auth.currentUser;
                            final uid = user?.uid;
                            final snapshot = await database
                                .child("VitalMonitor")
                                .child(uid.toString())
                                .child("Iot")
                                .child("Cam")
                                .get();
                            if (snapshot.exists) {
                              final final_vald = snapshot.value;
                              if (final_vald != "False") {
                                Navigator.push(
                                    context,
                                    MaterialPageRoute(
                                        builder: (context) => cam_screen()));
                              } else {
                                final vdatabase = database
                                    .child("VitalMonitor")
                                    .child("Tokens")
                                    .child(uid.toString());
                                final sing_up_data = <String, dynamic>{
                                  "Conf": "C",
                                };
                                await vdatabase
                                    .update(sing_up_data)
                                    .then((value) {
                                  print("value");
                                }).onError((error, stackTrace) {
                                  print("Error ${error.toString()}");
                                });
                                Navigator.push(
                                    context,
                                    MaterialPageRoute(
                                        builder: (context) =>
                                            start_conf_hub()));
                              }
                            }
                          },
                          child: Container(
                            margin: EdgeInsets.zero,
                            padding: EdgeInsets.zero,
                            width: 110,
                            height: 150,
                            decoration: BoxDecoration(
                              color: const Color(0xFF9F3F00),
                              borderRadius: BorderRadius.only(
                                topLeft: Radius.circular(15),
                                topRight: Radius.circular(15),
                                bottomRight: Radius.circular(15),
                                bottomLeft: Radius.circular(15),
                              ),
                              border: Border(
                                left: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                top: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                right: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                bottom: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                              ),
                            ),
                            child: Center(
                              child: Text(
                                r'''Cámara''',
                                style: GoogleFonts.outfit(
                                  textStyle: TextStyle(
                                    color: const Color(0xFFFFFFFF),
                                    fontWeight: FontWeight.w500,
                                    fontSize: 20,
                                    fontStyle: FontStyle.normal,
                                    decoration: TextDecoration.none,
                                  ),
                                ),
                                textAlign: TextAlign.center,
                              ),
                            ),
                          ),
                        ),
                        GestureDetector(
                          onTap: () {
                            Navigator.push(
                                context,
                                MaterialPageRoute(
                                    builder: (context) => Iot_screen()));
                          },
                          child: Container(
                            margin: EdgeInsets.zero,
                            padding: EdgeInsets.zero,
                            width: 110,
                            height: 150,
                            decoration: BoxDecoration(
                              color: const Color(0xFF9F3F00),
                              borderRadius: BorderRadius.only(
                                topLeft: Radius.circular(15),
                                topRight: Radius.circular(15),
                                bottomRight: Radius.circular(15),
                                bottomLeft: Radius.circular(15),
                              ),
                              border: Border(
                                left: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                top: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                right: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                                bottom: BorderSide(
                                    width: 0,
                                    style: BorderStyle.none,
                                    color: Color(0xFF000000)),
                              ),
                            ),
                            child: Center(
                              child: Text(
                                r'''Dispositivo Iot''',
                                style: GoogleFonts.outfit(
                                  textStyle: TextStyle(
                                    color: const Color(0xFFFFFFFF),
                                    fontWeight: FontWeight.w500,
                                    fontSize: 20,
                                    fontStyle: FontStyle.normal,
                                    decoration: TextDecoration.none,
                                  ),
                                ),
                                textAlign: TextAlign.center,
                              ),
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: const EdgeInsets.only(
                      top: 20,
                    ),
                    width: double.maxFinite,
                    decoration: const BoxDecoration(
                      color: Color(0xFFFFFFFF),
                      border: Border(
                        left: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFF000000)),
                      ),
                    ),
                    child: Center(
                      child: GestureDetector(
                        onTap: () {
                          downloadFile();
                        },
                        child: Container(
                          margin: EdgeInsets.zero,
                          padding: EdgeInsets.zero,
                          width: 110,
                          height: 150,
                          decoration: BoxDecoration(
                            color: const Color(0xFF8D8377),
                            borderRadius: BorderRadius.only(
                              topLeft: Radius.circular(15),
                              topRight: Radius.circular(15),
                              bottomRight: Radius.circular(15),
                              bottomLeft: Radius.circular(15),
                            ),
                            border: Border(
                              left: BorderSide(
                                  width: 0,
                                  style: BorderStyle.none,
                                  color: Color(0xFF000000)),
                              top: BorderSide(
                                  width: 0,
                                  style: BorderStyle.none,
                                  color: Color(0xFF000000)),
                              right: BorderSide(
                                  width: 0,
                                  style: BorderStyle.none,
                                  color: Color(0xFF000000)),
                              bottom: BorderSide(
                                  width: 0,
                                  style: BorderStyle.none,
                                  color: Color(0xFF000000)),
                            ),
                          ),
                          child: Center(
                            child: Text(
                              r'''Reporte de datos''',
                              style: GoogleFonts.outfit(
                                textStyle: TextStyle(
                                  color: const Color(0xFFFFFFFF),
                                  fontWeight: FontWeight.w500,
                                  fontSize: 20,
                                  fontStyle: FontStyle.normal,
                                  decoration: TextDecoration.none,
                                ),
                              ),
                              textAlign: TextAlign.center,
                            ),
                          ),
                        ),
                      ),
                    ),
                  ),
                ],
              ),
            ),
          )),
        ));
  }
}
