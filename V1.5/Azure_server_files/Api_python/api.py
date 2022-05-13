from flask import *
import pyrebase
import datetime as time
import json 
import pandas as pd
import os.path
import os
import sys
import openpyxl


app = Flask(__name__)



@app.route('/', methods=['GET'])
def home_page():
    data = {'Page': 'Home', 'Message': 'All ok', 'Timestamp': str(time.time())}
    json_dump = json.dumps(data)
    
    return json_dump



@app.route('/reporte/', methods=['GET', 'POST'])
def makereport():
    config = {
  "apiKey": "AIzaSyCKOZaxb6cXjfSdCu8hDgRz59NDJ8hx14Q",
  "authDomain": "medicproyect-d2db9.firebaseapp.com",
  "databaseURL": "https://medicproyect-d2db9-default-rtdb.firebaseio.com",
  "projectId": "medicproyect-d2db9",
  "storageBucket": "medicproyect-d2db9.appspot.com",
  "messagingSenderId": "22664224405",
  "appId": "1:22664224405:web:a2675796eb49a6393f459c"
}
    firebase = pyrebase.initialize_app(config)
    auth = firebase.auth()
    user = auth.sign_in_with_email_and_password("devices@gmail.com", "gerardo02")
    db = firebase.database()
    folderSavePath = "/home/gergames01/api/datos/"
    print(folderSavePath)

    user_query = str(request.args.get('user'))
    users = db.child('VitalMonitor').child(user_query).child("Data").get(user['idToken'])
    jsdat = json.dumps(users.val())
    df_json = pd.read_json(jsdat)
    df1 = df_json.transpose()
    df1.to_excel(folderSavePath+'reporte.xlsx',startcol = 1,columns = ["Spo2","Bpm","Temp","Dia","Sys","Atemp","Ahumi","Date"])
    path = folderSavePath+'reporte.xlsx'
    book = openpyxl.load_workbook(path) 
    sheet = book['Sheet1']
    sheet.delete_cols(2, 1) 
    sheet["B1"] = "SPO2 %"
    sheet["C1"] = "BPM"
    sheet["D1"] = "Temperatura de piel °C"
    sheet["E1"] = "Diastólica mmHg"
    sheet["F1"] = "Sistólica mmHg"
    sheet["G1"] = "Temperatura ambiente °C"
    sheet["H1"] = "Humedad ambiente %"
    sheet["I1"] = "FECHA"
    book.save(path)
    

    return send_from_directory(directory = folderSavePath, path= folderSavePath, filename='reporte.xlsx')





if __name__ == '__main__':
    
    app.run(port=65080,host = "0.0.0.0")