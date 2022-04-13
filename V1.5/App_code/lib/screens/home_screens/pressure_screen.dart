import 'dart:async';

import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/CircleProgress.dart';
import 'package:flutter_app1/screens/home_screens/home_screen.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:intl/intl.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class pressure_screen extends StatefulWidget {
  const pressure_screen({Key? key}) : super(key: key);

  @override
  State<pressure_screen> createState() => _pressure_screenState();
}

class _pressure_screenState extends State<pressure_screen> {
  final FirebaseAuth auth = FirebaseAuth.instance;
  final database = FirebaseDatabase.instance.ref();
  late StreamSubscription _streaming;
  late StreamSubscription _stream;
  String Dia_last = "0";
  String Sys_last = "0";
  String _date = "Sin datos";
  bool buttonactive = true;
  String _buttontext = "Tomar presion";
  @override
  void initState() {
    super.initState();
    get_values();
    get_values_for_compare();
  }

  Future<bool> _onWillPop() async {
    deactivate();
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => home_page()));
    return false;
  }

  void get_values() async {
    final user = await auth.currentUser;
    final uid = user?.uid;

    _streaming = database
        .child("VitalMonitor")
        .child(uid.toString())
        .child("Data")
        .onValue
        .listen((event) {
      final data = Map<String, dynamic>.from(event.snapshot.value as dynamic);
      final _dia = data["Dia"];
      final _sys = data["Sys"];
      final _dat = data["date"];

      setState(() {
        Dia_last = _dia;
        Sys_last = _sys;
        _date = _dat;
      });
    });
  }

  void get_values_for_compare() async {
    final user = await auth.currentUser;
    final uid = user?.uid;

    _stream = database
        .child("VitalMonitor")
        .child(uid.toString())
        .child("Iot")
        .child("Press")
        .onValue
        .listen((event) {
      final data = event.snapshot.value.toString();
      setState(() {
        if (data == "Tak" || data == "True") {
          buttonactive = false;
          _buttontext = "Tomando presión";
        } else {
          buttonactive = true;
          _buttontext = "Tomar presión";
        }
        print(data);
      });
    });
  }

  void send_data() async {
    print("mandando");

    final user = await auth.currentUser;
    final uid = user?.uid;
    final vdatabase =
        database.child("VitalMonitor").child(uid.toString()).child("Iot");
    final sing_up_data = <String, dynamic>{
      "Press": "True",
    };
    await vdatabase.update(sing_up_data).then((value) {
      print("value");
    }).onError((error, stackTrace) {
      print("Error ${error.toString()}");
    });
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
            body: Center(
                child: SingleChildScrollView(
                    child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                Text(
                  "Última toma de presión",
                  style: GoogleFonts.outfit(
                      textStyle: TextStyle(
                          color: const Color(0xF0000000),
                          fontWeight: FontWeight.w400,
                          fontSize: 30,
                          fontStyle: FontStyle.normal,
                          decoration: TextDecoration.none)),
                ),
                CustomPaint(
                  foregroundPainter:
                      CircleProgress(double.parse(Dia_last), 100),
                  child: Container(
                    width: 200,
                    height: 200,
                    child: Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text('Diastolica'),
                          Text(
                            '${int.parse(Dia_last)}',
                            style: TextStyle(
                                fontSize: 50, fontWeight: FontWeight.bold),
                          ),
                          Text(
                            'mmHg', //°C
                            style: TextStyle(
                                fontSize: 20, fontWeight: FontWeight.bold),
                          ),
                        ],
                      ),
                    ),
                  ),
                ),
                CustomPaint(
                  foregroundPainter:
                      CircleProgress(double.parse(Sys_last), 200),
                  child: Container(
                    width: 200,
                    height: 200,
                    child: Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text('Sistolica'),
                          Text(
                            '${int.parse(Sys_last)}',
                            style: TextStyle(
                                fontSize: 50, fontWeight: FontWeight.bold),
                          ),
                          Text(
                            'mmHg', //°C
                            style: TextStyle(
                                fontSize: 20, fontWeight: FontWeight.bold),
                          ),
                        ],
                      ),
                    ),
                  ),
                ),
                Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text(r'''Fecha''',
                        style: GoogleFonts.outfit(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w500,
                            fontSize: 30,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.center,
                        maxLines: 1),
                    Text(_date,
                        style: GoogleFonts.outfit(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w400,
                            fontSize: 16,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.center,
                        maxLines: 1),
                  ],
                ),
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
            )))));
  }

  @override
  void deactivate() {
    _streaming.cancel();
    _stream.cancel();
    print("cancelando");

    super.deactivate();
  }
}
