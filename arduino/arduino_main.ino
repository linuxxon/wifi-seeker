#define left_power 3
#define left_direction 4 
#define right_power  10
#define right_direction 12
#define ultraSound0in 5
#define ultraSound0out 6
#define ultraSound1in 7
#define ultraSound1out 8
#define button 0

#include <PWM.h>

int32_t frequency = 500; //frequency (in Hz)
int speed=0,distanceFront=0,distanceLeft=0,errorWall=0,wallDistance=20;

void setup() {
  
 pinMode(right_power, OUTPUT); 
 pinMode(right_direction, OUTPUT);
 pinMode(left_power, OUTPUT); 
 pinMode(left_direction, OUTPUT);
 pinMode(ultraSound0in, INPUT);
 pinMode(ultraSound0out,OUTPUT); 
 pinMode(ultraSound1in, INPUT);
 pinMode(ultraSound1out,OUTPUT); 
 pinMode(8,OUTPUT);
 pinMode(9,OUTPUT);
 InitTimersSafe();  //init Timers except Timer0
 
 Serial.begin(9600);
 
 // might not need to set both these pins, due to being on the same timer
 SetPinFrequencySafe(right_power, frequency); 
 SetPinFrequencySafe(left_power, frequency); 
}


void slowRight (){
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(left_power,speed*0.8);
  pwmWrite(right_power,speed*0.8);
  
}


void stop(){
  digitalWrite(right_direction,LOW); // Brake
  digitalWrite(left_direction,LOW); // Brake
  delay(600);
  pwmWrite(right_power, 0);
  pwmWrite(left_power, 0);
}

void start(){
  
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, speed);
  pwmWrite(left_power, speed);
}
void forward() {
  digitalWrite(right_direction,HIGH);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, (speed/2));
  pwmWrite(left_power, (speed/2));
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

void loop() {   
  distanceFront = getDistanceFront();  
  if (distanceFront<50) {
    stop();
    delay(5000);
    start();
  }
  
  speed = analogRead(5)/4;
  
  //errorWall = wallDistance - distanceLeft;
  delay(300);
  
  distanceLeft = getDistanceLeft(); 
  while (distanceLeft < 15) {
    distanceLeft = getDistanceLeft(); 
    //spinRight();
    slowRight();
    delay(20);
    //foward();
    //delay(500);
  }
  //else if (errorWall < -15) {
    //spinRight();
    //delay(500); 
    //forward();
    //delay(500);
  //} 
  
  //delay(50);
  
  Serial.print("Speed: ");
  Serial.print(speed);  
  Serial.print(" Front sensor: ");
  Serial.print(distanceFront);
  Serial.print(" Left sensor: ");
  Serial.print(distanceLeft);
  Serial.print("\n");  

  forward(); 

}

int getDistanceFront() { 
  int distance=0;
  digitalWrite(ultraSound0out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound0out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound0out, LOW);  
  distance = pulseIn(ultraSound0in, HIGH);
  distance = microsecondsToCentimeters(distance);
  return distance;
}

int getDistanceLeft() { 
  int distance=0;
  digitalWrite(ultraSound1out, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSound1out, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSound1out, LOW);  
  distance = pulseIn(ultraSound1in, HIGH);
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















