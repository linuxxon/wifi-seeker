#define right_forward  9       
#define right_backward 3     
#define left_forward   10           
#define left_backward  11  
#include <PWM.h>

int32_t frequency = 500; //frequency (in Hz)

void setup() {
  
 pinMode(right_forward, OUTPUT); 
 pinMode(right_backward, OUTPUT);
 pinMode(left_forward, OUTPUT);
 pinMode(left_backward, OUTPUT);
 
 InitTimersSafe();  //init Timers except Timer0
 
 // might not need to set both these pins, due to being on the same timer
 SetPinFrequencySafe(right_forward, frequency); 
 SetPinFrequencySafe(left_forward, frequency);
}



void rightForward () {
  
  digitalWrite(right_forward, LOW);
  
}      
void leftForward () {
    digitalWrite(left_forward, LOW);

}
void rightBackward () {
    digitalWrite(right_backward, LOW);
}
void leftBackward () {
    digitalWrite(left_backward, LOW);
}

void stanna(){
   digitalWrite(right_forward, HIGH);
   digitalWrite(left_forward, HIGH);
   digitalWrite(right_backward, HIGH);
   digitalWrite(left_backward, HIGH);
}


void forward() {
 //digitalWrite(right_forward, HIGH);
 //digitalWrite(left_forward, HIGH);
 //delay(1000);
 //rightForward();
 //leftForward();
 //delay(1000);
 
//  analogWrite(right_forward, 64);
//  analogWrite(left_forward, 64);
//pwmWrite(right_forward, )
pwmWrite(right_forward, 127);
pwmWrite(left_forward, 127);
}

void backward() {
   digitalWrite(right_backward, HIGH);
   digitalWrite(left_backward, HIGH);
  rightBackward();
  leftBackward();
}
void left() {
   digitalWrite(right_forward, HIGH);
   digitalWrite(right_backward, HIGH);
   digitalWrite(left_forward, LOW);
   digitalWrite(left_backward, LOW);
}
void right() {
   digitalWrite(right_forward, LOW);
   digitalWrite(right_backward, LOW);
   digitalWrite(left_forward, HIGH);
   digitalWrite(left_backward, HIGH);
}



void loop() {
  
  forward();
}
