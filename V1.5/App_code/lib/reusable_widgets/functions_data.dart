String? validateEmail(String? formemail) {
  if (formemail == null || formemail.isEmpty) return 'Se necesita un Email';

  return null;
}

String? validatePass(String? formemail) {
  if (formemail == null || formemail.isEmpty)
    return 'Se necesita una contraseña';

  return null;
}

String? validateName(String? formemail) {
  if (formemail == null || formemail.isEmpty) return 'Se necesita un nombre';

  return null;
}
