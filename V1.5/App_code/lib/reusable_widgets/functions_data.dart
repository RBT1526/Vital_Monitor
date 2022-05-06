String? validateEmail(String? formemail) {
  if (formemail == null || formemail.isEmpty) return 'Se necesita un Email';

  String pattern = r'\w+@\w+\.\w+';
  RegExp regex = RegExp(pattern);
  if (!regex.hasMatch(formemail)) return 'Escribe un email valido';

  return null;
}

String? validatePass(String? formemail) {
  if (formemail == null || formemail.isEmpty)
    return 'Se necesita una contraseña';

  String pattern =
      r'^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[!@#\$&*~?]).{8,}$';
  RegExp regex = RegExp(pattern);
  if (!regex.hasMatch(formemail))
    return ''' 
La contraseña debe ser de almenos 8 caracteres
incluyendo una Mayuscula, un numero y un signo.
      ''';

  return null;
}

String? validateName(String? formemail) {
  if (formemail == null || formemail.isEmpty) return 'Se necesita un nombre';

  return null;
}
