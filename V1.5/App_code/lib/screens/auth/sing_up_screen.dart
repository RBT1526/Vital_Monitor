import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/functions_data.dart';
import 'package:flutter_app1/reusable_widgets/reusable_widget.dart';
import 'package:flutter_app1/screens/auth/forgot_pass_screen.dart';
import 'package:flutter_app1/screens/auth/singin_screen.dart';
import 'package:google_fonts/google_fonts.dart';

class Sing_up_page extends StatefulWidget {
  const Sing_up_page({Key? key}) : super(key: key);

  @override
  State<Sing_up_page> createState() => _Sing_up_pageState();
}

class _Sing_up_pageState extends State<Sing_up_page> {
  final FirebaseAuth auth = FirebaseAuth.instance;

  TextEditingController name_text_controller = TextEditingController();
  TextEditingController email_text_Controller = TextEditingController();
  TextEditingController password_text_controller = TextEditingController();
  TextEditingController passwordconfirm_text_Controller =
      TextEditingController();

  void register() async {
    await auth
        .createUserWithEmailAndPassword(
            email: email_text_Controller.text,
            password: password_text_controller.text)
        .then((value) async {
      final database = FirebaseDatabase.instance.ref();
      final vdatabase = database.child("VitalMonitor");
      final user = await auth.currentUser;
      final uid = user?.uid;
      final sing_up_data = <String, dynamic>{
        uid.toString(): {
          "Dir": "False",
          "Data": {
            "BPM": "0",
            "Dis": "0",
            "SPO2": "0",
            "Sys": "200",
            "Temp": "0",
            "date": "0"
          },
          "Iot": {"Press": "False", "Switch": "False", "Cam": "False"},
          "User": {"name": name_text_controller.text}
        }
      };
      await vdatabase.update(sing_up_data).then((value) {
        Navigator.push(
            context, MaterialPageRoute(builder: (context) => SignInscreen()));
      }).onError((error, stackTrace) {
        print("Error ${error.toString()}");
      });
    }).onError((error, stackTrace) {
      print("Error ${error.toString()}");
    });
  }

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
              child: SingleChildScrollView(
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
                          child: Text(r'''Regístrate''',
                              style: GoogleFonts.outfit(
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
                      child: Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Text(r''' Empieza a utilizar''',
                              style: GoogleFonts.outfit(
                                textStyle: TextStyle(
                                  color: const Color(0xFF000000),
                                  fontWeight: FontWeight.w400,
                                  fontSize: 28,
                                  fontStyle: FontStyle.normal,
                                  decoration: TextDecoration.none,
                                ),
                              ),
                              textAlign: TextAlign.center,
                              maxLines: 1),
                          Text(r'''VitalMonitor ahora.''',
                              style: GoogleFonts.outfit(
                                textStyle: TextStyle(
                                  color: const Color(0xFF000000),
                                  fontWeight: FontWeight.w400,
                                  fontSize: 28,
                                  fontStyle: FontStyle.normal,
                                  decoration: TextDecoration.none,
                                ),
                              ),
                              textAlign: TextAlign.center,
                              maxLines: 1),
                        ],
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
                        child: reusabletextfield("Nombre", false,
                            name_text_controller, validateName),
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
                        child: reusabletextfield("Email", false,
                            email_text_Controller, validateEmail),
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
                        child: reusabletextfield("Contraseña", true,
                            password_text_controller, validatePass),
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
                        child: reusabletextfield("Repite la contraseña", true,
                            passwordconfirm_text_Controller, validatePass),
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
                          child: singInsingUpButton(context, "Registrarse", () {
                            register();
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
                            left:
                                BorderSide(width: 0, color: Color(0xFF000000)),
                            top: BorderSide(width: 0, color: Color(0xFF000000)),
                            right:
                                BorderSide(width: 0, color: Color(0xFF000000)),
                            bottom:
                                BorderSide(width: 0, color: Color(0xFF000000)),
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
                          Text(r'''¿Ya tienes cuenta?''',
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
                              await Future<void>.delayed(
                                  Duration(milliseconds: 100));
                              await Navigator.push<void>(
                                context,
                                MaterialPageRoute(
                                  builder: (context) => SignInscreen(),
                                ),
                              );
                            },
                            child: Text(r'''Iniciar sesíon''',
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
                  ])))
        ]));
  }
}
