import 'package:flutter/material.dart';
import 'package:flutter_app1/reusable_widgets/functions_data.dart';
import 'package:google_fonts/google_fonts.dart';

TextFormField reusabletextfield(String text, bool isPasswordType,
    TextEditingController controller, var validator) {
  return TextFormField(
    controller: controller,
    obscureText: isPasswordType,
    enableSuggestions: !isPasswordType,
    autocorrect: !isPasswordType,
    cursorColor: Colors.black,
    validator: validator,
    style: GoogleFonts.outfit(
      textStyle: TextStyle(
        color: const Color(0xFF000000),
        fontWeight: FontWeight.w400,
        fontSize: 16,
        fontStyle: FontStyle.normal,
        decoration: TextDecoration.none,
      ),
    ),
    decoration: InputDecoration(
      hintText: text,
      border: OutlineInputBorder(),
      enabledBorder: OutlineInputBorder(),
      focusedBorder: OutlineInputBorder(),
      contentPadding: const EdgeInsets.only(
        left: 16,
      ),
    ),
    keyboardType: isPasswordType
        ? TextInputType.visiblePassword
        : TextInputType.emailAddress,
    textAlign: TextAlign.left,
    maxLines: 1,
    minLines: 1,
    maxLength: null,
  );
}

Container singInsingUpButton(
    BuildContext context, String texts, Function onTap) {
  return Container(
    width: double.maxFinite,
    height: 49,
    decoration: BoxDecoration(
      color: const Color(0xFF000000),
      borderRadius: BorderRadius.only(
        topLeft: Radius.circular(20),
        topRight: Radius.circular(20),
        bottomRight: Radius.circular(20),
        bottomLeft: Radius.circular(20),
      ),
      border: null,
    ),
    child: ElevatedButton(
      onPressed: () {
        onTap();
      },
      child: Text(
        texts,
        style: GoogleFonts.outfit(
          textStyle: TextStyle(
            color: const Color(0xFFFFFFFF),
            fontWeight: FontWeight.w400,
            fontSize: 20,
            fontStyle: FontStyle.normal,
            decoration: TextDecoration.none,
          ),
        ),
        textAlign: TextAlign.center,
      ),
      style: ButtonStyle(
        backgroundColor: MaterialStateProperty.resolveWith((states) {
          if (states.contains(MaterialState.pressed)) {
            return Colors.white;
          }
          return Colors.black;
        }),
      ),
    ),
  );
}
