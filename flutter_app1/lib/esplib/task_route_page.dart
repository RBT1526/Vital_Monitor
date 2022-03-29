import 'dart:async';
import 'package:esptouch_smartconfig/esp_touch_result.dart';
import 'package:esptouch_smartconfig/esptouch_smartconfig.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class TaskRoute extends StatefulWidget {
  TaskRoute(
      this.ssid, this.bssid, this.password, this.deviceCount, this.isBroadcast);
  final String ssid;
  final String bssid;
  final String password;
  final String deviceCount;
  final bool isBroadcast;
  @override
  State<StatefulWidget> createState() {
    return TaskRouteState();
  }
}

class TaskRouteState extends State<TaskRoute> {
  late Stream<ESPTouchResult>? _stream;

  @override
  void initState() {
    _stream = EsptouchSmartconfig.run(
        ssid: widget.ssid,
        bssid: widget.bssid,
        password: widget.password,
        deviceCount: widget.deviceCount,
        isBroad: widget.isBroadcast);
    super.initState();
  }

  @override
  dispose() {
    super.dispose();
  }

  Widget waitingState(BuildContext context) {
    return Center(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.center,
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          CircularProgressIndicator(
            valueColor: AlwaysStoppedAnimation(Color.fromARGB(255, 0, 0, 0)),
          ),
          SizedBox(height: 16),
          Text(
            'Configurando ...',
            style: TextStyle(fontSize: 24),
          ),
        ],
      ),
    );
  }

  Widget error(BuildContext context, String s) {
    return Center(
      child: Text(
        s,
        style: TextStyle(color: Colors.red),
      ),
    );
  }

  Widget noneState(BuildContext context) {
    return Container(
      margin: EdgeInsets.zero,
      padding: EdgeInsets.zero,
      width: double.maxFinite,
      decoration: const BoxDecoration(
        color: Color(0xFFFFFFFF),
        border: Border(
          left: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          top: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          right: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          bottom: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
        ),
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Image.network(
              r'''https://i.dlpng.com/static/png/6671722_preview.png''',
              width: double.maxFinite,
              height: 200,
              fit: BoxFit.scaleDown,
            ),
            Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                top: 40,
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
              child: Text(r'''Whoops!''',
                  style: GoogleFonts.outfit(
                    textStyle: TextStyle(
                      color: const Color(0xFF000000),
                      fontWeight: FontWeight.w700,
                      fontSize: 40,
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
                left: 18,
                top: 20,
                right: 18,
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
                  r'''El dispositivo no se pudo conectar por algún error, vuelva a intentarlo''',
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
                  maxLines: 3),
            ),
            Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                left: 40,
                top: 30,
                right: 40,
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
              child: GestureDetector(
                onTap: () {
                  Navigator.of(context).pop(_results);
                },
                child: Container(
                  width: double.maxFinite,
                  height: 48,
                  decoration: BoxDecoration(
                    color: const Color(0xFF166DFF),
                    borderRadius: BorderRadius.only(
                      topLeft: Radius.circular(96),
                      topRight: Radius.circular(96),
                      bottomRight: Radius.circular(96),
                      bottomLeft: Radius.circular(96),
                    ),
                    border: null,
                  ),
                  alignment: Alignment.center,
                  child: Text(
                    'Intentar de nuevo',
                    style: GoogleFonts.poppins(
                      textStyle: TextStyle(
                        color: const Color(0xFFFFFFFF),
                        fontWeight: FontWeight.w400,
                        fontSize: 26,
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
    );
  }

  Widget resultList(BuildContext context, ConnectionState state) {
    return Container(
      margin: EdgeInsets.zero,
      padding: EdgeInsets.zero,
      width: double.maxFinite,
      decoration: const BoxDecoration(
        color: Color(0xFFFFFFFF),
        border: Border(
          left: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          top: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          right: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
          bottom: BorderSide(
              width: 0, style: BorderStyle.none, color: Color(0xFF000000)),
        ),
      ),
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Image.network(
              r'''https://www.jing.fm/clipimg/full/270-2708225_green-right-tick.png''',
              width: double.maxFinite,
              height: 200,
              fit: BoxFit.scaleDown,
            ),
            Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                top: 40,
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
              child: Text(r'''Conectado''',
                  style: GoogleFonts.outfit(
                    textStyle: TextStyle(
                      color: const Color(0xFF000000),
                      fontWeight: FontWeight.w700,
                      fontSize: 40,
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
                left: 18,
                top: 20,
                right: 18,
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
                  r'''El dispositivo se vinculó satisfactoriamente y quedo registrado en la cuenta''',
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
                  maxLines: 3),
            ),
            Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                left: 40,
                top: 30,
                right: 40,
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
              child: GestureDetector(
                onTap: () {
                  final result = _results.toList(growable: false)[0];
                  print(result.ip);
                  Navigator.of(context).pop(_results);
                },
                child: Container(
                  width: double.maxFinite,
                  height: 48,
                  decoration: BoxDecoration(
                    color: const Color(0xFF166DFF),
                    borderRadius: BorderRadius.only(
                      topLeft: Radius.circular(96),
                      topRight: Radius.circular(96),
                      bottomRight: Radius.circular(96),
                      bottomLeft: Radius.circular(96),
                    ),
                    border: null,
                  ),
                  child: Text(
                    'Terminar',
                    style: GoogleFonts.poppins(
                      textStyle: TextStyle(
                        color: const Color(0xFFFFFFFF),
                        fontWeight: FontWeight.w400,
                        fontSize: 30,
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
    );
  }

  final Set<ESPTouchResult> _results = Set();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: PreferredSize(
          preferredSize: const Size.fromHeight(60),
          child:
              Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
            Container(
              margin: EdgeInsets.zero,
              padding: const EdgeInsets.only(
                left: 25,
                top: 40,
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
              child: Align(
                alignment: Alignment.centerLeft,
                child: GestureDetector(
                  onTap: () {
                    Navigator.of(context).pop(_results);
                  },
                  child: Icon(
                    MdiIcons.fromString('arrow-left'),
                    size: 40,
                    color: Color(0xFF000000),
                  ),
                ),
              ),
            ),
          ])),
      body: Container(
        child: StreamBuilder<ESPTouchResult>(
          stream: _stream,
          builder: (context, AsyncSnapshot<ESPTouchResult> snapshot) {
            if (snapshot.hasError) {
              return error(context, 'Error in StreamBuilder');
            }
            switch (snapshot.connectionState) {
              case ConnectionState.active:
                _results.add(snapshot.data!);
                return resultList(context, ConnectionState.active);
              case ConnectionState.none:
                return noneState(context);
              case ConnectionState.done:
                if (snapshot.hasData) {
                  _results.add(snapshot.data!);
                  return resultList(context, ConnectionState.done);
                } else
                  return noneState(context);
              case ConnectionState.waiting:
                return waitingState(context);
            }
          },
        ),
      ),
    );
  }
}
