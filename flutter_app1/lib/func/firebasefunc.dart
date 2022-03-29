import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter_app1/func/json_conv.dart';

class Firebase_utils {
  final FirebaseAuth auth = FirebaseAuth.instance;
  late final DatabaseReference _baseref;

  void Start_root(Convjson msj) async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    print(uid.toString());
    // here you write the codes to input the data into firestore
    this._baseref = FirebaseDatabase.instance.ref().child(uid.toString());
    _baseref.set(msj.toJson());
  }

  void Get_data() async {
    final user = await auth.currentUser;
    final uid = user?.uid;
    print(uid.toString());
    // here you write the codes to input the data into firestore
    this._baseref = FirebaseDatabase.instance.ref().child(uid.toString());

    var datos = _baseref.child("Dir").onValue;
    print(datos.toString());
  }
}
