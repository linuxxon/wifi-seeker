/*  This file is part of Wifi Seeker.
 *
 *  Copyright (C) 2015  Niklas Bergh, Rasmus Linusson, Araxi Mekhitarian,
 *                      Sebastian Törnqvist, Max von Zedtwitz-Liebenstein
 *
 *  Wifi Seeker is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Wifi Seeker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Wifi Seeker.  If not, see <http://www.gnu.org/licenses/>.
 */

#define left_power 3
#define left_direction 4 
#define right_power  10
#define right_direction 12
#define ultraSound0in 9
#define ultraSound0out 13
#define ultraSound1in 5
#define ultraSound1out 6
#define ultraSound2in 7
#define ultraSound2out 8
#define button A0
#define modeLED 11
#define SCANNINGMODE 0
#define HOTSPOTMODE 1

#include <PWM.h>

int32_t frequency = 500; //frequency (in Hz)
int speed=0,distanceFront=0,oldDistanceFront=0,distanceLeftFront=0,oldDistanceLeftFront=0,distanceLeft=0,oldDistanceLeft=0;
bool isColliding=false;

int MODE = HOTSPOTMODE;
bool lastButtonValue = false;

void setup() {
 pinMode(right_power, OUTPUT); 
 pinMode(right_direction, OUTPUT);
 pinMode(left_power, OUTPUT); 
 pinMode(left_direction, OUTPUT);
 pinMode(ultraSound0in, INPUT);
 pinMode(ultraSound0out,OUTPUT); 
 pinMode(ultraSound1in, INPUT);
 pinMode(ultraSound1out,OUTPUT);
 pinMode(ultraSound2in, INPUT);
 pinMode(ultraSound2out,OUTPUT);
 pinMode(button, INPUT);
 pinMode(modeLED, OUTPUT);
 InitTimersSafe();  //init Timers except Timer0
 
 // Set LED off as we start in normal mode
 digitalWrite(modeLED, HIGH);
 
 Serial.begin(9600);
 
 // might not need to set both these pins, due to being on the same timer
 SetPinFrequencySafe(right_power, frequency); 
 SetPinFrequencySafe(left_power, frequency); 
}


void slowRight (){
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(left_power,200);
  pwmWrite(right_power,200);  
}
void slowLeft (){
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,LOW);
  pwmWrite(left_power,speed);
  pwmWrite(right_power,speed);  
}


void stop(){
  digitalWrite(right_direction,LOW); // Brake
  digitalWrite(left_direction,LOW); // Brake
  delay(600);
  pwmWrite(right_power, 0);
  pwmWrite(left_power, 0);
}

void start(int time){  
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, 255);
  pwmWrite(left_power, 255);
  delay(time);
}
void forward() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, speed);
  pwmWrite(left_power, speed);
}

void fastLeft() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,LOW);
  pwmWrite(right_power, 255);
  pwmWrite(left_power, 255);
}

void fastRight() {
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, 255);
  pwmWrite(left_power, 255);
}

void loop() {
  bool buttonValue = analogRead(button) < 512 ? false : true;
  if (buttonValue && !lastButtonValue) {
    if (MODE == HOTSPOTMODE) {
      digitalWrite(modeLED, HIGH);
      Serial.print("scanningMode");
      Serial.write(0);
      MODE = SCANNINGMODE;
    }
    else if (MODE == SCANNINGMODE) {
      digitalWrite(modeLED, LOW);
      Serial.print("hotspotMode");
      Serial.write(0);
      stop();
      MODE = HOTSPOTMODE;
    }
  }
  lastButtonValue = buttonValue;
  
  if (MODE == HOTSPOTMODE) {
    // Add code here if you want to
  }
  else if (MODE == SCANNINGMODE) {
    oldDistanceFront = distanceFront;
    distanceFront = getDistanceFront(); 
    
    delay(50); // Wait for echoes
    oldDistanceLeftFront = distanceLeftFront;
    distanceLeftFront = getDistanceLeftFront(); 
    
    delay(50); // Wait for echoes
    oldDistanceLeft=distanceLeft;
    distanceLeft = getDistanceLeft(); 
    
    speed = analogRead(5)/4;
    
    if ((distanceFront>0 && oldDistanceFront>0 && distanceFront<70)) {
      //stop();
      //delay(2000);
      fastRight();
      delay(100);
      fastLeft();
      delay(15); // Brake the turn
      //forward();
      //delay(50);
      //start(250);
    }
    else if (distanceLeftFront>0 && oldDistanceLeftFront>0 && distanceLeftFront<70) {
      fastRight();
      delay(70);
      fastLeft();
      delay(15); // Brake the turn
      //forward();
      //delay(100);
    }
    else {        
      if (distanceLeft>0 && oldDistanceLeft>0 && distanceLeft < 25) {
        fastRight();
        delay(30);
        forward();
        delay(100);
      }  
      if (distanceLeft > 30 && distanceLeft < 60) {
        fastLeft();
        delay(30);
        forward();
        delay(100);
      } 
      if (distanceLeft ==0 || distanceLeft > 60) {
        fastLeft();
        delay(100);
        fastRight();
        delay(15); // Brake the turn
        forward();
        delay(100);
      }
    }
    forward(); 
  }
}


int getDistanceFront() { 
  int distance=0;
  digitalWrite(ultraSound0out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound0out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound0out, LOW);  
  distance = pulseIn(ultraSound0in, HIGH,8000);
  distance = microsecondsToCentimeters(distance);
  return distance;
}

int getDistanceLeftFront() { 
  int distance=0;
  digitalWrite(ultraSound1out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound1out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound1out, LOW);  
  distance = pulseIn(ultraSound1in, HIGH,8000);
  distance = microsecondsToCentimeters(distance);
  return distance;
}

int getDistanceLeft() { 
  int distance=0;
  digitalWrite(ultraSound2out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound2out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound2out, LOW);  
  distance = pulseIn(ultraSound2in, HIGH,8000);
  distance = microsecondsToCentimeters(distance);
  return distance;
}
  
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


