import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/func/firebasefunc.dart';
import 'package:flutter_app1/func/json_conv.dart';

import 'package:flutter_app1/screens/auth/forgot_pass_screen.dart';

void prueba_func(
    var password_text_controller, var email_text_Controller, var context) {
  final firebase_utils = Firebase_utils();
  FirebaseAuth.instance
      .signInWithEmailAndPassword(
          email: email_text_Controller.text,
          password: password_text_controller.text)
      .then((value) {
    firebase_utils.Get_data();
    Navigator.push(
        context, MaterialPageRoute(builder: (context) => forgot_pass()));
  }).onError((error, stackTrace) {
    print("Error ${error.toString()}");
  });
}
