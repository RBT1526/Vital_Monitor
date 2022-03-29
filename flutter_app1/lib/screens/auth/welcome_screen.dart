import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/reusable_widget.dart';
import 'package:flutter_app1/screens/auth/singin_screen.dart';
import 'package:google_fonts/google_fonts.dart';

class welcome_page extends StatefulWidget {
  const welcome_page({Key? key}) : super(key: key);

  @override
  State<welcome_page> createState() => _welcome_pageState();
}

class _welcome_pageState extends State<welcome_page> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: const Color(0xFF000000),
        body: Stack(children: [
          Container(
              margin: EdgeInsets.zero,
              padding: EdgeInsets.zero,
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
              child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Container(
                      margin: EdgeInsets.zero,
                      padding: EdgeInsets.zero,
                      width: double.maxFinite,
                      height: 300,
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
                          top: 40,
                        ),
                        child: Image.asset(
                          "assets/images/image1.png",
                          width: double.maxFinite,
                          height: 150,
                          fit: BoxFit.scaleDown,
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
                          top: 25,
                          bottom: 14,
                        ),
                        child: Text(r''' Bivenvenido a VitalMonitor''',
                            style: GoogleFonts.poppins(
                              textStyle: TextStyle(
                                color: const Color(0xFF000000),
                                fontWeight: FontWeight.w600,
                                fontSize: 34,
                                fontStyle: FontStyle.normal,
                                decoration: TextDecoration.none,
                              ),
                            ),
                            textAlign: TextAlign.center,
                            maxLines: 3),
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
                          left: 5,
                          right: 5,
                          bottom: 40,
                        ),
                        child: Text(
                            r'''VitalMonitor puede ser el dispositivo que salve la vida de algun familiar suyo,  este sistema inovador lo puede empezar a usar ahora mismo.''',
                            style: GoogleFonts.poppins(
                              textStyle: TextStyle(
                                color: const Color(0xFF000000),
                                fontWeight: FontWeight.w400,
                                fontSize: 16,
                                fontStyle: FontStyle.normal,
                                decoration: TextDecoration.none,
                              ),
                            ),
                            textAlign: TextAlign.center,
                            maxLines: 5),
                      ),
                    ),
                    Container(
                      margin: const EdgeInsets.only(
                        left: 10,
                        right: 10,
                      ),
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
                      alignment: Alignment.center,
                      child: GestureDetector(
                        onTap: () async {
                          await Future<void>.delayed(Duration(milliseconds: 1));
                          await Navigator.push<void>(
                            context,
                            MaterialPageRoute(
                              builder: (context) => SignInscreen(),
                            ),
                          );
                        },
                        child: Container(
                          width: double.maxFinite,
                          height: 48,
                          decoration: BoxDecoration(
                            color: const Color(0xFF3285FF),
                            borderRadius: BorderRadius.only(
                              topLeft: Radius.circular(8),
                              topRight: Radius.circular(8),
                              bottomRight: Radius.circular(8),
                              bottomLeft: Radius.circular(8),
                            ),
                            border: null,
                          ),
                          alignment: Alignment.center,
                          child: Text(
                            'Empiece ahora',
                            style: GoogleFonts.poppins(
                              textStyle: TextStyle(
                                color: const Color(0xFFFFFFFF),
                                fontWeight: FontWeight.w400,
                                fontSize: 22,
                                fontStyle: FontStyle.normal,
                                decoration: TextDecoration.none,
                              ),
                            ),
                            textAlign: TextAlign.center,
                          ),
                        ),
                      ),
                    ),
                  ]))
        ]));
  }
}
