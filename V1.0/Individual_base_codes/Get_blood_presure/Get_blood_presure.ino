//Get_diastolic_and_systolic atmega328p
//
//
//
//Gerardo Fregoso Jimenez Code
//
//Code for manipulate a comercial arm blood pressure monitor
//The arm blood pressure monitor that i used is
//https://bit.ly/3rllZEi
//
//---------------------------------------------------------------------------
const int start = 10;  //declare the variable for the start button
const int memory = 9;  //start is a cable on pin 10 to a transistor conected to the buttons, same with memory
String data = "";  //variable to save the data of the serial communication
int HR= 0; // variable to save the bpm
int diastolic = 0;//variable to save the diastolic
int systolic= 0;//variable to save the systolic
int flag = 0; //a flag
//---------------------------------------------------------------------------
//Simulate a clik function
void button_function(int delaytime, int mode, int individualmode ) { //request a 3 variable (delaytime,mode,individualmode)
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (mode) { // if the mode variable is true so it click the two buttons
    digitalWrite(start, HIGH);//activate the start button
    digitalWrite(memory, HIGH); //activate the memory button
    delay(delaytime);// delay time of the delaytime variable
    digitalWrite(memory, LOW); // turn off the memory button
    delay(100);// 100 miliseconds to simulate that first i leave the memory button
    digitalWrite(start, LOW);// turn off the memory button

  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else {
    //////////////////////////////////////////////////////////////////////////////////
    if (individualmode) { // When the mode is 0 and the individualmode is 1 activate only start button
      digitalWrite(start, HIGH);// activate start
      delay(delaytime); // delay of the delaytime variable
      digitalWrite(start, LOW);// turn off the start button
    }
    //////////////////////////////////////////////////////////////////////////////////
    else {//when mode is 0 and individualmode is 0 activate only memory button
      digitalWrite(memory, HIGH);// activate memory button
      delay(delaytime);// delay time of the delaytime variable
      digitalWrite(memory, LOW);// turn off the memory button
    }
    //////////////////////////////////////////////////////////////////////////////////
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//---------------------------------------------------------------------------
//function to get data
void get_data() { 
  button_function(200, 0, 1); // Start the start button for 200 miliseconds
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  while (data.substring(0, 9) != "measuring") { // wait until the proccess finish
    if (Serial.available()) { // check the serial data
      data = Serial.readStringUntil('\n');// in the variable data we store the data
    }
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  delay(100); // 100 miliseconds delay
  button_function(200, 0, 0); // start the memory button
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  while (flag == 0) { // while the flag is empty
    if (Serial.available()) { // if there is data on the serial readings
      data = Serial.readStringUntil('\n'); // its stored on the data variable
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " sys") { // search for the systolic un the string
        systolic = data.substring(7, data.length()).toInt(); // store the value on a separate variable
      }
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " dia") { // search for the diastolic un the string
        diastolic = data.substring(7, data.length()).toInt();  // store the value on a separate variable
      }
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " pul") { //search for the pulse un the string
        HR = data.substring(9, data.length()).toInt();  // store the value on a separate variable
        flag = 1;
      }
      //////////////////////////////////////////////////////////////////////////////////
    }
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  button_function(5500, 1, 0); // start the two buttons to reset the memory data
}
//---------------------------------------------------------------------------
//SETUP
void setup() {
  Serial.begin(115200);//Start the serial communication at 115200 bps
  pinMode(start, OUTPUT); // declare start and memory pins
  pinMode(memory, OUTPUT);
  delay(4000);// 4 seconds of delay to start
  //note you have to push the start button manualy after apply voltage for first time to the blood presure monitor
}
//---------------------------------------------------------------------------
//main loop
void loop() {
  get_data();// call the get_data function
  //Print the data
  Serial.print("SYS =");
  Serial.println(systolic);
  Serial.print("Dia =");
  Serial.println(diastolic);
  Serial.print("ritmo =");
  Serial.println(HR);
  //infinite loop to finish the program
  while (1);

}
//--------------------------------------–----––--––---------------------------
//Gerardo Fregoso Jiménez 2022
