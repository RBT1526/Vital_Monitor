import 'dart:async';
import 'package:connectivity_plus/connectivity_plus.dart';
import 'package:esptouch_smartconfig/esptouch_smartconfig.dart';
import 'package:flutter_app1/esplib/wifi_page.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/screens/New_data_hub/Start_connect_data_hub.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';

class ConnectivityPage extends StatefulWidget {
  @override
  _ConnectivityPageState createState() => _ConnectivityPageState();
}

class _ConnectivityPageState extends State<ConnectivityPage> {
  late Connectivity _connectivity;
  late Stream<ConnectivityResult> _connectivityStream;
  late StreamSubscription<ConnectivityResult> _connectivitySubscription;
  ConnectivityResult? result;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _connectivity = Connectivity();
    _connectivityStream = _connectivity.onConnectivityChanged;
    _connectivitySubscription = _connectivityStream.listen((e) {
      setState(() {});
    });
  }

  @override
  void dispose() {
    // TODO: implement dispose
    _connectivitySubscription.cancel();
    super.dispose();
  }

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
                  onTap: () async {
                    await Navigator.push<void>(
                      context,
                      MaterialPageRoute(
                        builder: (context) => start_conf_hub(),
                      ),
                    );
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
      body: FutureBuilder(
          future: _connectivity.checkConnectivity(),
          builder: (context, snapshot) {
            if (!snapshot.hasData)
              return Center(
                child: CircularProgressIndicator(),
              );
            else if (snapshot.data == ConnectivityResult.wifi) {
              return FutureBuilder<Map<String, String>?>(
                  future: EsptouchSmartconfig.wifiData(),
                  builder: (context, snapshot) {
                    if (snapshot.connectionState == ConnectionState.done) {
                      return WifiPage(snapshot.data!['wifiName']!,
                          snapshot.data!['bssid']!);
                    } else
                      return Container();
                  });
            } else
              return Center(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(
                      Icons.wifi_off_sharp,
                      size: 200,
                      color: Colors.red,
                    ),
                    Text(
                      "No hay conexion",
                      style: TextStyle(
                          fontSize: 40, color: Color.fromARGB(255, 0, 0, 0)),
                    )
                  ],
                ),
              );
          }),
    );
  }
}
