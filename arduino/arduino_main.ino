#define right_power  3
#define right_direction 4
#define left_power   11
#define left_direction 12
#define ultraSound0in 5
#define ultraSound0out 6

#include <PWM.h>

int32_t frequency = 500; //frequency (in Hz)
int PWM_dutyCycle=230;
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



void rightForward () {  
  //digitalWrite(right_forward, LOW);
  
}      
void leftForward () {
    //digitalWrite(left_forward, LOW);

}
void rightBackward () {
    //digitalWrite(right_backward, LOW);
}
void leftBackward () {
    //digitalWrite(left_backward, LOW);
}

void stop(){
  //digitalWrite(right_direction,LOW);
  //digitalWrite(left_direction,LOW);
  //delay(10);
  
  pwmWrite(right_power, 0);
  pwmWrite(left_power, 0);
  digitalWrite(9,HIGH); // Brake
  digitalWrite(8,HIGH); // Brake
}


void forward() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, PWM_dutyCycle);
  pwmWrite(left_power, PWM_dutyCycle);
}

void backward() {
//   digitalWrite(right_backward, HIGH);
//   digitalWrite(left_backward, HIGH);
//  rightBackward();
//  leftBackward();
}
void left() {
//   digitalWrite(right_forward, HIGH);
//   digitalWrite(right_backward, HIGH);
//   digitalWrite(left_forward, LOW);
//   digitalWrite(left_backward, LOW);
}
void right() {
//   digitalWrite(right_forward, LOW);
//   digitalWrite(right_backward, LOW);
//   digitalWrite(left_forward, HIGH);
//   digitalWrite(left_backward, HIGH);
}



void loop() {   
  sendUltrasoundPulse(); 
  distance = pulseIn(ultraSound0in, HIGH);
  distance = microsecondsToCentimeters(distance);
  
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  
  if (distance<50) {
    stop();
    delay(5000);
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















