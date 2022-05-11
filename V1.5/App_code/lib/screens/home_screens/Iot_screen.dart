import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/CircleProgress.dart';
import 'package:flutter_app1/screens/New_data_hub/Start_connect_data_hub.dart';
import 'package:flutter_app1/screens/home_screens/home_screen.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class Iot_screen extends StatefulWidget {
  const Iot_screen({Key? key}) : super(key: key);

  @override
  State<Iot_screen> createState() => _Iot_screenState();
}

class _Iot_screenState extends State<Iot_screen> {
  String _humid = "0";
  String _tempe = "0";
  String _humid_t = "N/A";
  String _tempe_t = "N/A";

  final FirebaseAuth auth = FirebaseAuth.instance;
  final database = FirebaseDatabase.instance.ref();
  late StreamSubscription _streaming;
  late StreamSubscription _stream;
  bool buttonactive = true;
  String _buttontext = "Encender";
  bool check_config = false;

  @override
  void initState() {
    super.initState();
    get_values_for_compare();
    get_values();
    get_stream_val();
  }

  Future<bool> _onWillPop() async {
    deactivate();
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => home_page()));
    return false;
  }

  void get_values() async {
    String _humi = "0";
    String _Temp = "0";
    final user = await auth.currentUser;
    final uid = user?.uid;

    _streaming = database
        .child("VitalMonitor")
        .child(uid.toString())
        .child("Data")
        .limitToLast(1)
        .onChildAdded
        .listen((event) {
      final data = Map<String, dynamic>.from(event.snapshot.value as dynamic);
      print(data);
      if (data["Ahumi"] == "R") {
        _tempe = "0";
        _humid = "0";
        _tempe_t = "N/A";
        _humid_t = "N/A";
      } else {
        _tempe = data["Atemp"];
        _humid = data["Ahumi"];
        _tempe_t = data["Atemp"];
        _humid_t = data["Ahumi"];
      }

      setState(() {});
    });
  }

  void get_values_for_compare() async {
    final user = await auth.currentUser;
    final uid = user?.uid;

    final snapshot = await database
        .child("VitalMonitor")
        .child("Tokens")
        .child(uid.toString())
        .child("Wifidir")
        .get();
    if (snapshot.exists) {
      final final_vald = snapshot.value.toString();
      if (final_vald == "False") {
        check_config = false;
        buttonactive = true;
        _buttontext = "configurar";
      } else {
        check_config = true;
        buttonactive = true;
        _buttontext = "Encender";
      }
    } else {
      check_config = false;
      buttonactive = true;
      _buttontext = "configurar";
    }
  }

  void get_stream_val() async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    _stream = database
        .child("VitalMonitor")
        .child(uid.toString())
        .child("Iot")
        .child("Switch")
        .onValue
        .listen((event) {
      final datas = event.snapshot.value.toString();
      print(datas);
      if (check_config == true) {
        if (datas == "False") {
          buttonactive = true;
          _buttontext = "Encender";
        } else {
          buttonactive = true;
          _buttontext = "Apagar";
        }
      } else {
        check_config = false;
        buttonactive = true;
        _buttontext = "configurar";
      }

      setState(() {});
    });
  }

  void donoting() {}
  void send_data() async {
    print("mandando");

    final user = await auth.currentUser;
    final uid = user?.uid;
    if (_buttontext == "Apagar") {
      final vdatabase =
          database.child("VitalMonitor").child(uid.toString()).child("Iot");
      final sing_up_data = <String, dynamic>{
        "Switch": "False",
      };
      await vdatabase.update(sing_up_data).then((value) {
        print("value");
      }).onError((error, stackTrace) {
        print("Error ${error.toString()}");
      });
    } else if (_buttontext == "Encender") {
      final vdatabase =
          database.child("VitalMonitor").child(uid.toString()).child("Iot");
      final sing_up_data = <String, dynamic>{
        "Switch": "True",
      };
      await vdatabase.update(sing_up_data).then((value) {
        print("value");
      }).onError((error, stackTrace) {
        print("Error ${error.toString()}");
      });
    } else {
      print("configurar");
    }
  }

  void quitar_disp() async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    final vdatabase =
        database.child("VitalMonitor").child("Tokens").child(uid.toString());
    final sing_up_data = <String, dynamic>{
      "Wifidir": "False",
    };
    await vdatabase.update(sing_up_data).then((value) {
      print("value");
    }).onError((error, stackTrace) {
      print("Error ${error.toString()}");
    });
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => home_page()));
  }

  void configure() async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    final vdatabase =
        database.child("VitalMonitor").child("Tokens").child(uid.toString());
    final sing_up_data = <String, dynamic>{
      "Conf": "S",
    };
    await vdatabase.update(sing_up_data).then((value) {
      print("value");
    }).onError((error, stackTrace) {
      print("Error ${error.toString()}");
    });
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => start_conf_hub()));
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
            backgroundColor: Color.fromARGB(255, 255, 255, 255),
            drawer: Drawer(
              elevation: 16,
            ),
            body: SingleChildScrollView(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Center(
                    child: Padding(
                      padding: const EdgeInsets.only(
                        top: 20,
                        bottom: 20,
                      ),
                      child: Text(r'''IOT''',
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
                          textDirection: TextDirection.ltr,
                          maxLines: 1),
                    ),
                  ),
                  Center(
                    child: Padding(
                      padding: const EdgeInsets.only(
                        bottom: 30,
                      ),
                      child: Text(r'''Datos Ambientales''',
                          style: GoogleFonts.outfit(
                            textStyle: TextStyle(
                              color: const Color(0xFF000000),
                              fontWeight: FontWeight.w600,
                              fontSize: 24,
                              fontStyle: FontStyle.normal,
                              decoration: TextDecoration.none,
                            ),
                          ),
                          textAlign: TextAlign.center,
                          textDirection: TextDirection.ltr,
                          maxLines: 1),
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
                    child: Center(
                      child: Padding(
                        padding: const EdgeInsets.only(
                          left: 10,
                          right: 10,
                        ),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: [
                            CustomPaint(
                              foregroundPainter:
                                  CircleProgress(double.parse(_humid), 100),
                              child: Container(
                                width: 170,
                                height: 170,
                                child: Center(
                                  child: Column(
                                    mainAxisAlignment: MainAxisAlignment.center,
                                    children: <Widget>[
                                      Text('Humedad'),
                                      Text(
                                        '${_humid_t}',
                                        style: TextStyle(
                                            fontSize: 50,
                                            fontWeight: FontWeight.bold),
                                      ),
                                      Text(
                                        '%', //°C
                                        style: TextStyle(
                                            fontSize: 20,
                                            fontWeight: FontWeight.bold),
                                      ),
                                    ],
                                  ),
                                ),
                              ),
                            ),
                            CustomPaint(
                              foregroundPainter:
                                  CircleProgress(double.parse(_tempe), 100),
                              child: Container(
                                width: 170,
                                height: 170,
                                child: Center(
                                  child: Column(
                                    mainAxisAlignment: MainAxisAlignment.center,
                                    children: <Widget>[
                                      Text('Temperatura'),
                                      Text(
                                        '${_tempe_t}',
                                        style: TextStyle(
                                            fontSize: 50,
                                            fontWeight: FontWeight.bold),
                                      ),
                                      Text(
                                        '°C', //°C
                                        style: TextStyle(
                                            fontSize: 20,
                                            fontWeight: FontWeight.bold),
                                      ),
                                    ],
                                  ),
                                ),
                              ),
                            ),
                          ],
                        ),
                      ),
                    ),
                  ),
                  Center(
                    child: Padding(
                      padding: const EdgeInsets.only(
                        top: 20,
                        bottom: 20,
                      ),
                      child: Text(r'''Smart Switch''',
                          style: GoogleFonts.outfit(
                            textStyle: TextStyle(
                              color: const Color(0xFF000000),
                              fontWeight: FontWeight.w600,
                              fontSize: 24,
                              fontStyle: FontStyle.normal,
                              decoration: TextDecoration.none,
                            ),
                          ),
                          textAlign: TextAlign.center,
                          textDirection: TextDirection.ltr,
                          maxLines: 1),
                    ),
                  ),
                  Center(
                    child: Padding(
                      padding: const EdgeInsets.only(
                        top: 0,
                        bottom: 5,
                      ),
                      child: ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          onSurface: Colors.blue,
                        ),
                        onPressed: buttonactive
                            ? () async {
                                quitar_disp();
                              }
                            : null,
                        child: Text("Quitar dispositivo"),
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
                        left: 40,
                        right: 40,
                      ),
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Text(r'''Lámpara 1''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFF000000),
                                      fontWeight: FontWeight.w600,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.left,
                                  textDirection: TextDirection.ltr,
                                  maxLines: 1),
                              ElevatedButton(
                                style: ElevatedButton.styleFrom(
                                  onSurface: Colors.blue,
                                ),
                                onPressed: buttonactive
                                    ? () async {
                                        send_data();
                                      }
                                    : null,
                                child: Text(_buttontext),
                              ),
                            ],
                          ),
                          Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Text(r'''Ventilador 1''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFF000000),
                                      fontWeight: FontWeight.w600,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.left,
                                  textDirection: TextDirection.ltr,
                                  maxLines: 1),
                              ElevatedButton(
                                style: ElevatedButton.styleFrom(
                                  onSurface: Colors.blue,
                                ),
                                onPressed: buttonactive
                                    ? () async {
                                        donoting();
                                      }
                                    : null,
                                child: Text("Configurar"),
                              ),
                            ],
                          ),
                        ],
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
                            color: Color(0xFFFFFFFF)),
                        top: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFFFFFFFF)),
                        right: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFFFFFFFF)),
                        bottom: BorderSide(
                            width: 0,
                            style: BorderStyle.none,
                            color: Color(0xFFFFFFFF)),
                      ),
                    ),
                    child: Padding(
                      padding: const EdgeInsets.only(
                        left: 40,
                        top: 40,
                        right: 40,
                      ),
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Text(r'''Radio 1''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFF000000),
                                      fontWeight: FontWeight.w600,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.left,
                                  textDirection: TextDirection.ltr,
                                  maxLines: 1),
                              ElevatedButton(
                                style: ElevatedButton.styleFrom(
                                  onSurface: Colors.blue,
                                ),
                                onPressed: buttonactive
                                    ? () async {
                                        configure();
                                      }
                                    : null,
                                child: Text("Configurar"),
                              ),
                            ],
                          ),
                          Column(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Text(r'''Lampara 2''',
                                  style: GoogleFonts.outfit(
                                    textStyle: TextStyle(
                                      color: const Color(0xFF000000),
                                      fontWeight: FontWeight.w600,
                                      fontSize: 20,
                                      fontStyle: FontStyle.normal,
                                      decoration: TextDecoration.none,
                                    ),
                                  ),
                                  textAlign: TextAlign.left,
                                  textDirection: TextDirection.ltr,
                                  maxLines: 1),
                              ElevatedButton(
                                style: ElevatedButton.styleFrom(
                                  onSurface: Colors.blue,
                                ),
                                onPressed: buttonactive
                                    ? () async {
                                        donoting();
                                      }
                                    : null,
                                child: Text("Configurar"),
                              ),
                            ],
                          ),
                        ],
                      ),
                    ),
                  ),
                ],
              ),
            )));
  }

  @override
  void deactivate() {
    print("cancelando");
    _streaming.cancel();
    _stream.cancel();
    super.deactivate();
  }
}
