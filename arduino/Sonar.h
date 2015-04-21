class Sonar {
  int trigPin, echoPin;
  int topLimit, bottomLimit;
  
public:
  Sonar(int TrigPin, int EchoPin) {
    trigPin = TrigPin;
    echoPin = EchoPin;
    
    topLimit = 400; // 2cm to 4m is the range of the hc-sr04 module
    bottomLimit = 2;
  }
  ~Sonar() { }
  
  void setTopLimit(void Limit) { topLimit = Limit; }
  void setBottomLimit(void Limit) { bottomLimit = Limit; }
  
  int getDistance() {
    long duration,
    int distance;
    
    // Trigger a ping
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Listen to pong
    duration = pulseIn(echoPin, HIGH);
    distance = duration/58;
    
    if (distance > topLimit)
      distance = topLimit;
    else if (distance < bottomLimit)
      distance = bottomLimit;
    
    return distance;
  }
};
