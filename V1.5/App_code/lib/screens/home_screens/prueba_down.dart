import 'dart:io';

import 'package:dio/dio.dart';

import 'package:flutter/material.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:path_provider/path_provider.dart';

void main() {
  runApp(MaterialApp(home: Home()));
}

class Home extends StatefulWidget {
  @override
  State<Home> createState() => _HomeState();
}

class _HomeState extends State<Home> {
  String fileurl =
      "http://20.120.242.95:65080/reporte/?user=VLDywIaOVvP8SYzPAwtsQNiouql2";
  //you can save other file formats too.
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

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        resizeToAvoidBottomInset: false,
        appBar: AppBar(
          title: Text("Download File from URL"),
          backgroundColor: Colors.deepPurpleAccent,
        ),
        body: Container(
          margin: EdgeInsets.only(top: 50),
          child: Column(
            children: [
              Text("File URL: $fileurl"),
              Divider(),
              ElevatedButton(
                onPressed: () async {
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
                        await Dio().download(fileurl, savePath,
                            onReceiveProgress: (received, total) {
                          if (total != -1) {
                            print((received / total * 100).toStringAsFixed(0) +
                                "%");
                            //you can build progressbar feature too
                          }
                        });
                        print("File is saved to download folder.");
                      } on DioError catch (e) {
                        print(e.message);
                      }
                    }
                  } else {
                    print("No permission to read and write.");
                  }
                },
                child: Text("Download File."),
              )
            ],
          ),
        ));
  }
}
