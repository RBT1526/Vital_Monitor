import 'package:flutter/material.dart';
import 'package:flutter_app1/screens/home_screens/Camera_screen.dart';
import 'package:flutter_app1/screens/home_screens/pressure_screen.dart';
import 'package:flutter_app1/screens/home_screens/vital_sign_screen.dart';
import 'dart:io';

import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class home_page extends StatefulWidget {
  const home_page({Key? key}) : super(key: key);

  @override
  State<home_page> createState() => _home_pageState();
}

class _home_pageState extends State<home_page> {
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

  @override
  void initState() {
    super.initState();
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
          body: Stack(
            children: [
              Container(
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
                child: Column(
                  children: [
                    Padding(
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
                                      builder: (context) =>
                                          vital_sign_screen()));
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
                            onTap: () {
                              Navigator.push(
                                  context,
                                  MaterialPageRoute(
                                      builder: (context) => cam_screen()));
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
            ],
          ),
        ));
  }
}
