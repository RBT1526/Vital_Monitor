import 'package:flutter/material.dart';
import 'package:flutter_app1/esplib/connectivity_page.dart';
import 'package:google_fonts/google_fonts.dart';

class start_conf_hub extends StatefulWidget {
  const start_conf_hub({Key? key}) : super(key: key);

  @override
  State<start_conf_hub> createState() => _start_conf_hubState();
}

class _start_conf_hubState extends State<start_conf_hub> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: const Color(0xFF000000),
        body: Stack(
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
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Container(
                    margin: EdgeInsets.zero,
                    padding: const EdgeInsets.only(
                      top: 10,
                    ),
                    width: double.maxFinite,
                    height: 350,
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
                    child: Image.asset(
                      "assets/images/image2.png",
                      width: double.maxFinite,
                      height: 150,
                      fit: BoxFit.scaleDown,
                    ),
                  ),
                  Container(
                    margin: const EdgeInsets.only(),
                    padding: const EdgeInsets.only(
                      left: 16,
                      top: 30,
                      right: 16,
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
                    child: Text(r'''Añade un DataHub''',
                        style: GoogleFonts.outfit(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w600,
                            fontSize: 30,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.center,
                        maxLines: 1),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: const EdgeInsets.only(
                      left: 16,
                      top: 20,
                      right: 16,
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
                    child: Text(
                        r'''Comienza la configuración de tu dispositivo, para esto conectalo a una fuente de energía y presiona comenzar''',
                        style: GoogleFonts.outfit(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w400,
                            fontSize: 20,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.center,
                        maxLines: 4),
                  ),
                  Container(
                    margin: EdgeInsets.zero,
                    padding: const EdgeInsets.only(
                      left: 20,
                      top: 30,
                      right: 20,
                    ),
                    width: double.maxFinite,
                    decoration: BoxDecoration(
                      color: const Color(0xFFFFFFFF),
                      borderRadius: BorderRadius.only(
                        topLeft: Radius.circular(26),
                        topRight: Radius.circular(26),
                        bottomRight: Radius.circular(26),
                        bottomLeft: Radius.circular(26),
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
                    child: GestureDetector(
                      onTap: () async {
                        await Future<void>.delayed(Duration(milliseconds: 100));
                        await Navigator.push<void>(
                            context,
                            MaterialPageRoute(
                                builder: (context) => ConnectivityPage()));
                      },
                      child: Container(
                        width: double.maxFinite,
                        height: 48,
                        decoration: BoxDecoration(
                          color: const Color(0xFF046BFF),
                          borderRadius: BorderRadius.only(
                            topLeft: Radius.circular(89),
                            topRight: Radius.circular(89),
                            bottomRight: Radius.circular(89),
                            bottomLeft: Radius.circular(89),
                          ),
                          border: null,
                        ),
                        alignment: Alignment.center,
                        child: Text(
                          'Comenzar',
                          style: GoogleFonts.outfit(
                            textStyle: TextStyle(
                              color: const Color(0xFFFFFFFF),
                              fontWeight: FontWeight.w400,
                              fontSize: 25,
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
          ],
        ));
  }
}
