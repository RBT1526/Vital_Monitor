class Convjson {
  final String texto;

  Convjson(this.texto);

  Convjson.fromJson(Map<dynamic, dynamic> json)
      : texto = json['texto'] as String;

  Map<dynamic, dynamic> toJson() => <dynamic, dynamic>{
        'Dir': texto,
        "active": {"Press": "False"},
        "datos": {"start": "start"}
      };
}
