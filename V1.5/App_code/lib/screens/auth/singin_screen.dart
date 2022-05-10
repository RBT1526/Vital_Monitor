import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/functions_data.dart';
import 'package:flutter_app1/reusable_widgets/reusable_widget.dart';
import 'package:flutter_app1/screens/New_data_hub/Start_connect_data_hub.dart';
import 'package:flutter_app1/screens/auth/forgot_pass_screen.dart';
import 'package:flutter_app1/screens/auth/sing_up_screen.dart';
import 'package:flutter_app1/screens/home_screens/home_screen.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:firebase_database/firebase_database.dart';

class SignInscreen extends StatefulWidget {
  const SignInscreen({Key? key}) : super(key: key);

  @override
  State<SignInscreen> createState() => _SignInscreenState();
}

class _SignInscreenState extends State<SignInscreen> {
  final FirebaseAuth auth = FirebaseAuth.instance;
  final GlobalKey<FormState> _key = GlobalKey<FormState>();
  TextEditingController password_text_controller = TextEditingController();
  TextEditingController email_text_Controller = TextEditingController();
  String errorMessage = '';
  void singin_func() async {
    if (_key.currentState!.validate()) {
      try {
        await auth
            .signInWithEmailAndPassword(
                email: email_text_Controller.text,
                password: password_text_controller.text)
            .then((value) async {
          final database = FirebaseDatabase.instance.ref();

          final user = await auth.currentUser;
          final uid = user?.uid;
          errorMessage = '';
          final udatabase = database
              .child("VitalMonitor")
              .child("Tokens")
              .child(uid.toString())
              .child("Dir");
          await udatabase.get().then((value) {
            String? _Dir = value.value.toString();
            if (_Dir == "False") {
              Navigator.push(context,
                  MaterialPageRoute(builder: (context) => start_conf_hub()));
            } else {
              Navigator.push(context,
                  MaterialPageRoute(builder: (context) => home_page()));
            }
          }).onError((error, stackTrace) {
            print("Error ${error.toString()}");
          });
        }).onError((error, stackTrace) {
          print("Error ${error.toString()}");
        });
      } on FirebaseAuthException catch (error) {
        errorMessage = error.message!;
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Container(
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
      child: SingleChildScrollView(
          child: Form(
        key: _key,
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
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
              child: Padding(
                padding: const EdgeInsets.only(
                  left: 10,
                  top: 80,
                  right: 20,
                  bottom: 30,
                ),
                child: Center(
                  child: Text(r'''Iniciar sesión''',
                      style: GoogleFonts.poppins(
                        textStyle: TextStyle(
                          color: const Color(0xFF000000),
                          fontWeight: FontWeight.w700,
                          fontSize: 36,
                          fontStyle: FontStyle.normal,
                          decoration: TextDecoration.none,
                        ),
                      ),
                      textAlign: TextAlign.left,
                      maxLines: 2),
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
                  left: 15,
                  bottom: 40,
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(r'''Bienvenido''',
                        style: GoogleFonts.poppins(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w400,
                            fontSize: 28,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.left,
                        maxLines: 1),
                    Text(r'''Empieza a monitorear''',
                        style: GoogleFonts.poppins(
                          textStyle: TextStyle(
                            color: const Color(0xFF000000),
                            fontWeight: FontWeight.w400,
                            fontSize: 28,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.left,
                        maxLines: 1),
                  ],
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
              child: Container(
                margin: const EdgeInsets.only(
                  left: 16,
                  top: 8,
                  right: 16,
                  bottom: 8,
                ),
                width: double.maxFinite,
                decoration: const BoxDecoration(
                  color: Color(0xFFFFFFFF),
                  border: null,
                ),
                child: reusabletextfield(
                    "Email", false, email_text_Controller, validateEmail),
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
              child: Container(
                margin: const EdgeInsets.only(
                  left: 16,
                  top: 8,
                  right: 16,
                  bottom: 8,
                ),
                width: double.maxFinite,
                decoration: const BoxDecoration(
                  color: Color(0xFFFFFFFF),
                  border: null,
                ),
                child: reusabletextfield(
                    "Contraseña", true, password_text_controller, validatePass),
              ),
            ),
            Center(
              child: Text(errorMessage),
            ),
            Container(
              margin: const EdgeInsets.only(
                left: 16,
                right: 16,
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
              child: Align(
                alignment: Alignment.topRight,
                child: TextButton(
                  onPressed: () async {
                    /*
                    await Future<void>.delayed(Duration(milliseconds: 100));
                    await Navigator.push<void>(
                      context,
                      MaterialPageRoute(
                        builder: (context) => forgot_pass(),
                      ),
                    );
                    */
                  },
                  child: Text(r'''¿Olvidaste tu contraseña?''',
                      style: GoogleFonts.poppins(
                        textStyle: TextStyle(
                          color: const Color(0xFF000000),
                          fontWeight: FontWeight.w400,
                          fontSize: 16,
                          fontStyle: FontStyle.normal,
                          decoration: TextDecoration.none,
                        ),
                      ),
                      textAlign: TextAlign.left,
                      maxLines: 1),
                ),
              ),
            ),
            Container(
              margin: const EdgeInsets.only(
                left: 16,
                right: 16,
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
              child: Padding(
                padding: const EdgeInsets.only(
                  top: 20,
                  bottom: 20,
                ),
                child: GestureDetector(
                  child: singInsingUpButton(context, "Iniciar sesión", () {
                    singin_func();
                  }),
                ),
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(
                bottom: 30,
              ),
              child: Container(
                margin: const EdgeInsets.only(
                  left: 29,
                  right: 29,
                ),
                padding: EdgeInsets.zero,
                width: double.maxFinite,
                height: 1,
                decoration: const BoxDecoration(
                  color: Color(0xFFFFFFFF),
                  border: Border(
                    left: BorderSide(width: 0, color: Color(0xFF000000)),
                    top: BorderSide(width: 0, color: Color(0xFF000000)),
                    right: BorderSide(width: 0, color: Color(0xFF000000)),
                    bottom: BorderSide(width: 0, color: Color(0xFF000000)),
                  ),
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
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Text(r'''¿No tienes cuenta?''',
                      style: GoogleFonts.poppins(
                        textStyle: TextStyle(
                          color: const Color(0xFF000000),
                          fontWeight: FontWeight.w400,
                          fontSize: 16,
                          fontStyle: FontStyle.normal,
                          decoration: TextDecoration.none,
                        ),
                      ),
                      textAlign: TextAlign.left,
                      maxLines: 1),
                  TextButton(
                    onPressed: () async {
                      await Future<void>.delayed(Duration(milliseconds: 100));
                      await Navigator.push<void>(
                        context,
                        MaterialPageRoute(
                          builder: (context) => Sing_up_page(),
                        ),
                      );
                    },
                    child: Text(r'''Registrarse''',
                        style: GoogleFonts.poppins(
                          textStyle: TextStyle(
                            color: const Color(0xFF010101),
                            fontWeight: FontWeight.w700,
                            fontSize: 16,
                            fontStyle: FontStyle.normal,
                            decoration: TextDecoration.none,
                          ),
                        ),
                        textAlign: TextAlign.left,
                        maxLines: 1),
                  ),
                ],
              ),
            ),
          ],
        ),
      )),
    ));
  }
}
