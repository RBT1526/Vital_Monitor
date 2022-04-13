import 'package:flutter/material.dart';
import 'package:flutter_app1/screens/home_screens/home_screen.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';
import 'package:webview_flutter/webview_flutter.dart';

class cam_screen extends StatefulWidget {
  const cam_screen({Key? key}) : super(key: key);

  @override
  State<cam_screen> createState() => _cam_screenState();
}

class _cam_screenState extends State<cam_screen> {
  Future<bool> _onWillPop() async {
    deactivate();
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => home_page()));
    return false;
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
                  child: Column(children: [
                    Center(
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
                    )),
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(
                        onSurface: Colors.blue,
                      ),
                      onPressed: () {},
                      child: Text("Eliminar dispositivo"),
                    ),
                  ]),
                ),
              ),
            ),
            backgroundColor: const Color(0xFF000000),
            body: WebView(
              initialUrl: "http://20.94.218.189/client",
              javascriptMode: JavascriptMode.unrestricted,
            )));
  }
}
