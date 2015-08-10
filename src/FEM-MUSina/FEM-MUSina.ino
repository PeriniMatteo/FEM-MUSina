/*
#                     FEM-MUSina IRRIGATOR
#
# Software to manage FEM-MUSina irrigator
#
# Copyright (c) 2015 Tiziana Dallapè
# Copyright (c) 2015 Matteo Perini
# Copyright (c) 2015 Giambattista Toller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation
#
# See LICENSE file for details
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc.,59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# See https://github.com/musefablab/FEM-MUSina for detailed project
*/



#include <Servo.h>   //add libraries


/*----------------------------------------------------------------------
 objects and variables declarations and definitions list
----------------------------------------------------------------------*/
  Servo myservo;          //create a Servo object
  int ServoPin = 10;      //attach servo to pin 10
  int motorPinDirA = 12;  //attach motor to pins 12 13 3 11
  int motorPinDirB = 13;
  int motorPinPwmA = 3;
  int motorPinPwmB = 11;
  int EVPin = 4;          //attach electro valve to pin 4
  int stopPin = 2;        //attach switch to pin 2
//service variables
  int index = 0;          //vase index 
  int dir = 1;            //default motor direction
  int nStep = 0;          //definition of motor steps
  long int T0;            //variable to store Arduino's timer value [ms]
//variables for the selection of input pin at the 4051 multiplexer
  int r0 = 0;     //bit value of the selected input pin at the 4051 (s0)
  int r1 = 0;     //bit value of the selected input pin at the 4051 (s1)
  int r2 = 0;     //bit value of the selected input pin at the 4051 (s2)
//declaration and initialization of array of motor step
  boolean PwmA[8] = {1,1,1,0,1,1,1,0};
  boolean PwmB[8] = {1,0,1,1,1,0,1,1};
  boolean DirA[8] = {1,1,1,1,0,0,0,0};
  boolean DirB[8] = {1,1,0,0,0,0,1,1};
//user customizable variables
  const int n_pots = 8;     /*change value of index according to number
                              of pots you have; default: 8 pots*/
  int resistance[n_pots] = {0,0,0,0,0,0,0,0};/*declaration and
                                               initialization at 1023
                                               points of array, where
                                               resistance values
                                               (humidity proxy) will be
                                               checked*/
  int threshold[n_pots] = {900,900,900,900,900,900,900,900};/*declara-
                                                              tion and
                                                              initiali-
                                                              zation of
                                                              array of
                                                              water
                                                              threshold
                                                              of every
                                                              pot*/
  int steps = 195;          /*number of motor steps between two pipes;
                              default: 195 steps for our motor*/
  int water_time = 25000;   //valve opening time [ms]
  int emptying_delay = 2000;/*water complete emptying time through
                              pipes; default: 2000ms*/
  int check_delay = 180000; /*time between the end of one watering cycle
                              and the next one*/



void setup() {
/*----------------------------------------------------------------------
 SETUP FUNCTION
   this function runs once at the start of the program which is used to
   define initial environmental settings
----------------------------------------------------------------------*/
  pinMode(motorPinDirA, OUTPUT);  //motor "A" channel direction
  pinMode(motorPinDirB, OUTPUT);  //motor "B" channel direction
  pinMode(motorPinPwmA, OUTPUT);  //motor "A" impulse
  pinMode(motorPinPwmB, OUTPUT);  //motor "A" impulse
  pinMode(7, OUTPUT);             //s0 multiplexer byte index
  pinMode(6, OUTPUT);             //s1 multiplexer byte index
  pinMode(5, OUTPUT);             //s2 multiplexer byte index
  pinMode(EVPin, OUTPUT);         //electro valve control
  pinMode(A5, INPUT);             //ground resistance reading
  pinMode(stopPin, INPUT);        //switch reading
  Serial.begin(9600);             //serial initialization
  T0 = millis();                  //set T0 to millis()
  myservo.attach(ServoPin);       //initialize Servo motor
  myservo.write(0);               //set Servo position to 0deg
}


void step(int dir) {
/*----------------------------------------------------------------------
 motor moving function
----------------------------------------------------------------------*/
  if(dir == 1) {                                //move forward
    nStep = (nStep+1)%8;
    digitalWrite(motorPinDirA, DirA[nStep]);
    digitalWrite(motorPinDirB, DirB[nStep]);
    digitalWrite(motorPinPwmA, PwmA[nStep]);
    digitalWrite(motorPinPwmB, PwmB[nStep]);
  }else {                                       //backward
    nStep = (nStep+1)%8;
    digitalWrite(motorPinDirA, DirA[7-nStep]);     
    digitalWrite(motorPinDirB, DirB[7-nStep]);
    digitalWrite(motorPinPwmA, PwmA[7-nStep]);
    digitalWrite(motorPinPwmB, PwmB[7-nStep]);
  }
}


void check_ground_resistance() {
/*----------------------------------------------------------------------
 resistance check function
   the ground has variable resistance in function of its humidity.
   Resistance is measured by voltage divider, which is red by AD
   converter at pin A5. Resistance is checked on every pots
----------------------------------------------------------------------*/
  Serial.println("Checking...");
  for(index = 0; index <= (n_pots-1); index++) {   
    r0 = bitRead(index, 0);     //read a number as bits (first digit)
    r1 = bitRead(index, 1);     //read a number as bits (second digit)
    r2 = bitRead(index, 2);     //read a number as bits (third digit)
    digitalWrite(7, r0);        //set multiplexer to read indexth input
    digitalWrite(6, r1);
    digitalWrite(5, r2);
    int multiplexerOutput = analogRead(A5);/*read the multiplexer output
                                             value*/
    resistance[index] = multiplexerOutput; /*set data in resistance
                                             array*/
  }
}


void water(int to_be_watered) {
/*----------------------------------------------------------------------
 watering function
   this function recieves the index (0...n_pots) of the pot to be
   watered, basing on resistance check (humidity proxy); revolver moves
   forward to the right position, reached it, Servo pulls down the
   revolver which joints its gap with the lower part of machine, the
   valve is opened and the water flows through the pipe watering the pot
   for the water_time time; the valve is closed, wait for emptying_delay
   time for water complete emptying, then Servo pulls up the revolver
----------------------------------------------------------------------*/
  Serial.println("Going to the right position...");
  for(index = (to_be_watered * steps * 2); index >= 0; index--) {
    Serial.println("Step");
    step(1);
    delay(2);         //change this value to change speed; default: 2
  }
  Serial.println("Position reached!");
  myservo.write(70);                 //set servo to 70deg
  Serial.println("Pull down...");
  delay(500);
  digitalWrite(EVPin, HIGH);
  delay(water_time);
  Serial.println("Watering...");
  Serial.println("Please wait...");
  digitalWrite(EVPin, LOW);
  Serial.println("Turn off the valve...");
  delay(emptying_delay);
  Serial.println("Please wait a little...");
  myservo.write(0);                  //set servo to starting position
  Serial.println("Pull up...");
}


void stepper_reset() {
/*----------------------------------------------------------------------
 motor backward move function
   function moves backward the motor as long as revolver reach the
   switch to set the home position
----------------------------------------------------------------------*/
  analogWrite(motorPinPwmA, 255);
  analogWrite(motorPinPwmB, 255);
  Serial.println("I'm going...");
  while(!(digitalRead(stopPin) == HIGH)) {
    step(-1);
    delay(2);         //change this value to change speed; default: 2
  }
  Serial.println("Home reached!");
  delay(2000);
}


void loop() {
/*----------------------------------------------------------------------
 LOOP FUNCTION
   watering loop routine runs countless times calling some functions;
   when spended time since last watering is higher than check_delay
   time, watering loop starts checking resistance: if the ground is dry
   (low humidity) tension is high and the ground resistance is high too,
   so if the checked resistance in one pot is higher than threshold,
   that plant will be watered
----------------------------------------------------------------------*/
  analogWrite(motorPinPwmA, 0);
  analogWrite(motorPinPwmB, 0);
  if((millis() - T0) > check_delay) {
    T0 = millis();
    Serial.println("Check resistance...");   
    check_ground_resistance();
    for(index = 0; index <= (n_pots-2); index++) {
      Serial.print(resistance[index]);
      Serial.print(" , ");
    }
    Serial.println(resistance[(n_pots-1)]);
    Serial.println("");
    Serial.println("Controlling values...");
    for(index = 0; index <= (n_pots-1); index++) {  
      if(resistance[index] > threshold[index]) {
        Serial.print("Resistance at the ");
        Serial.print(index+1);
        Serial.println("th vase voltage is too high!");
        Serial.println("Try to water the plant");
        stepper_reset();
        Serial.println("Go to home position...");
        int to_be_watered = index+1;
        water(to_be_watered);
        Serial.println("Water the plant");
        Serial.println("Plant watered!");
        break;
      }else {
        Serial.print("Plant ");
        Serial.print(index+1);
        Serial.println(" doesn't need water...");
      }
     Serial.println("Waiting for new resistance check...");
     Serial.println("");
    }
  }
}
