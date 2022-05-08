import 'dart:async';
import 'dart:convert';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/CircleProgress.dart';
import 'package:flutter_app1/screens/home_screens/home_screen.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class vital_sign_screen extends StatefulWidget {
  const vital_sign_screen({Key? key}) : super(key: key);

  @override
  State<vital_sign_screen> createState() => _vital_sign_screenState();
}

class _vital_sign_screenState extends State<vital_sign_screen> {
  String Spo2_text = "0";
  String Bpm_text = "0";
  String Temp_text = "0";
  String spo2_announce = "N/A";
  String Bpm_announce = "N/A";
  String Temp_announce = "N/A";
  final FirebaseAuth auth = FirebaseAuth.instance;
  final database = FirebaseDatabase.instance.ref();
  late StreamSubscription _vitalsignsub;
  @override
  void initState() {
    super.initState();
    get_values();
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
    print(uid);

    _vitalsignsub = database
        .child("VitalMonitor")
        .child(uid.toString())
        .child("Data")
        .limitToLast(1)
        .onChildAdded
        .listen((event) {
      final data = Map<String, dynamic>.from(event.snapshot.value as dynamic);
      print(data);
      String _spo2 = "0";
      String _spo2_t = "N/A";
      String _bpm = "0";
      String _bpm_t = "N/A";
      if (data["Spo2"] == "O") {
        _spo2 = "0";
        _spo2_t = "N/A";
      } else if (data["Spo2"] == "A") {
        _spo2 = "100";
        _spo2_t = "...";
      } else {
        _spo2 = data["Spo2"];
        _spo2_t = data["Spo2"];
      }
      if (data["Bpm"] == "O") {
        _bpm = "0";
        _bpm_t = "N/A";
      } else if (data["Bpm"] == "A") {
        _bpm = "100";
        _bpm_t = "...";
      } else {
        _bpm = data["Bpm"];
        _bpm_t = data["Bpm"];
      }

      final _Temp = data["Temp"];

      setState(() {
        Spo2_text = _spo2;
        Bpm_text = _bpm;
        Temp_text = _Temp;
        spo2_announce = _spo2_t;
        Bpm_announce = _bpm_t;
        Temp_announce = _Temp;
      });
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
                  "Signos Vitales",
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
                      CircleProgress(double.parse(Spo2_text), 100),
                  child: Container(
                    width: 200,
                    height: 200,
                    child: Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text('Spo2'),
                          Text(
                            '${spo2_announce}',
                            style: TextStyle(
                                fontSize: 50, fontWeight: FontWeight.bold),
                          ),
                          Text(
                            '%', //°C
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
                      CircleProgress(double.parse(Bpm_text), 120),
                  child: Container(
                    width: 200,
                    height: 200,
                    child: Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text('Bpm'),
                          Text(
                            '${Bpm_announce}',
                            style: TextStyle(
                                fontSize: 50, fontWeight: FontWeight.bold),
                          ),
                        ],
                      ),
                    ),
                  ),
                ),
                CustomPaint(
                  foregroundPainter:
                      CircleProgress(double.parse(Temp_text), 42),
                  child: Container(
                    width: 200,
                    height: 200,
                    child: Center(
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          Text('Temperatura'),
                          Text(
                            '${Temp_announce}',
                            style: TextStyle(
                                fontSize: 50, fontWeight: FontWeight.bold),
                          ),
                          Text(
                            '°C',
                            style: TextStyle(
                                fontSize: 20, fontWeight: FontWeight.bold),
                          ),
                        ],
                      ),
                    ),
                  ),
                )
              ],
            )))));
  }

  @override
  void deactivate() {
    _vitalsignsub.cancel();
    print("cancelando");
    super.deactivate();
  }
}
