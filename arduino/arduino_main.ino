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
 start(250);
}


void slowRight (){
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(left_power,speed*2);
  pwmWrite(right_power,speed*2);  
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
  pwmWrite(right_power, speed*3);
  pwmWrite(left_power, speed*3);
}

void fastRight() {
  digitalWrite(right_direction,LOW);
  digitalWrite(left_direction,HIGH);
  pwmWrite(right_power, speed*3);
  pwmWrite(left_power, speed*3);
}

void loop() {   
  distanceFront = getDistanceFront();  
  if (distanceFront>0 && distanceFront<30) {
    stop();
    delay(2000);
    //start(250);
  }
  
  speed = analogRead(5)/4;
  
  delay(50); // Wait for echoes
  
  distanceLeft = getDistanceLeft(); 
  if (distanceLeft>0 && distanceLeft < 20) {
    //spinRight();
    fastRight();
    delay(100);
    forward();
    delay(500);
  }  
  if (distanceLeft> 0 && distanceLeft > 30) {
    //spinRight();
    fastLeft();
    delay(100);
    forward();
    delay(500);
  } 
  
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
  distance = pulseIn(ultraSound0in, HIGH,5000);
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
  distance = pulseIn(ultraSound1in, HIGH,8000);
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















