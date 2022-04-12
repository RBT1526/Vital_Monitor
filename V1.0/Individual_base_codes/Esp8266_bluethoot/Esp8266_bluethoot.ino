//------------------––-------------------------------------------------------------
//bluethoot communication basic example
//
//
//Gerardo Fregoso Jiménez Code bluethoot communication basic example
//
//
//
//------------------––-------------------------------------------------------------
#include <SoftwareSerial.h>// include serial library
//------------------––-------------------------------------------------------------
SoftwareSerial miBt(5,16);// Declare the (tx and rx) pines
char Dato = 0;// create a char variable 
//------------------––-------------------------------------------------------------
void setup()
{
    pinMode(2,OUTPUT);//Declare the led pin as output
    Serial.begin(9600);// Start the serial communication on 9600bps
    Serial.println("Ready!");// Give the anouncement
    miBt.begin(38400);// start the Bluethooth communication
    digitalWrite(2,LOW);// put the led on
    
}
//------------------––-------------------------------------------------------------
void loop()
{
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    if(miBt.available()){// if we have a message on bluethoot
        Dato = miBt.read(); // read the message and store on a char variable
        if(Dato=='1')// if the char is 1 turn off the led
        digitalWrite(2,HIGH);

        if(Dato == '2')// if the char is 2 turn on the led
        digitalWrite(2,LOW);

        if(Dato == '3') // if the char is 3 send a message.
        miBt.println("Hi there");

    }
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
 //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&   
//BLINK EXAMPLE
    /*
digitalWrite(2,HIGH);
delay(2000);
digitalWrite(2, LOW);
delay(2000);
*/
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //TO CONFIGURE WITH AT COMMANDS
    /*
 if (miBt.available()){
     Serial.write(miBt.read());
 }   
 if(Serial.available()){
     miBt.write(Serial.read());
 }
 */
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
}
//------------------––-------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022