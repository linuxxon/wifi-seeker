/*
Fungerande program för demovisning 2

*/

#define left_power 3
#define left_direction 4 
#define right_power  10
#define right_direction 12
#define ultraSound0in 5
#define ultraSound0out 6
#define button 7

#include <PWM.h>

int32_t frequency = 500; //frequency (in Hz)
int speed;
long distance;

void setup() {
  
 pinMode(right_power, OUTPUT); 
 pinMode(right_direction, OUTPUT);
 pinMode(left_power, OUTPUT); 
 pinMode(left_direction, OUTPUT);
 pinMode(ultraSound0in, INPUT);
 pinMode(ultraSound0out,OUTPUT); 
 pinMode(8,OUTPUT);
 pinMode(9,OUTPUT);
 InitTimersSafe();  //init Timers except Timer0
 
 Serial.begin(9600);
 
 // might not need to set both these pins, due to being on the same timer
 SetPinFrequencySafe(right_power, frequency); 
 SetPinFrequencySafe(left_power, frequency); 

}

void stop(){
  digitalWrite(right_direction,LOW); // Brake
  digitalWrite(left_direction,LOW); // Brake
  delay(600);
  pwmWrite(right_power, 0);
  pwmWrite(left_power, 0);
}

void backward() {
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,LOW);
  pwmWrite(right_power, speed);
  pwmWrite(left_power, speed);
  }
  
void forward() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, speed);
  pwmWrite(left_power, speed);
}

void spinLeft() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,LOW);
  pwmWrite(right_power, speed*2);
  pwmWrite(left_power, speed*2);
}

void spinRight() {
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, speed*2);
  pwmWrite(left_power, speed*2);

}

void demo () {
  forward();
  delay(250);
  spinRight();
  delay(5000);
  forward();
  delay(250);
  spinLeft();
  delay(5000);
}
void loop() {   
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  sendUltrasoundPulse(); 
  distance = pulseIn(ultraSound0in, HIGH);
  distance = microsecondsToCentimeters(distance);
  
  speed = analogRead(5)/4;
  
  if (distance<50) {
    stop();
    delay(5000);
    demo();
  }
  
  delay(5);
  
  Serial.print(distance);
  Serial.print("\n");  

  forward(); 

}



void sendUltrasoundPulse() { 
  digitalWrite(ultraSound0out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound0out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound0out, LOW);  
}
  
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}










