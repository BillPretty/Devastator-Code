#include <Servo.h> 
#include <NewPing.h>  //https://bitbucket.org/teckel12/arduino-new-ping/downloads/

#define TRIG_PIN A4 
#define ECHO_PIN A5 
#define MAX_DISTANCE 200 

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control
char input;
bool flag = 0;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo myservo;   

int distance = 100;
void setup(void) 
{ 
    /* Setup baud rate for serial communication */
  Serial.begin(115200);
  delay(100);
  myservo.attach(8);  
  myservo.write(120); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);

  Serial.println("Hello World");
} 
void scan() {
 
 if (Serial.available() > 0) {
    input = Serial.read();
           /* Manual mode */
    if (input == 'F' || input == 'B' || input == 'L' || input == 'R' || input == 'S') {
     
      manual();
    }
    /* Automatic mode */
    else if (input == 'A') {
      flag=1;
      automatic();
    }
     
  }

}
 void loop() {
  
  scan();

}

/* Reading char 'M' on serial for stopping current mode */
void modeChange() {
  if (Serial.available() > 0) {
    input = Serial.read();

  if (input == 'M') {
      Serial.println("Changing Mode ");
      flag = 0;
      Serial.println(flag);
      stop();
      
  } 
  }
  }



  /********************************************* MANUAL MODE ********************************************/

void manual() {
 if (input == 'F') {
    moveForward();
  } 
 else if (input == 'B') {
    moveBackward();
  } 
 else if (input == 'L') {
    turnLeft();
  } 
 else if (input == 'R') {
    turnRight();
  } 
 else if (input == 'S') {
    stop();
  }
 }

 /********************************************* AUTOMATIC MODE ********************************************/

void automatic(void) 
{
 
 Serial.println(distance);
 Serial.println("Automatic");   
 int distanceR = 0;
 int distanceL =  0;
 delay(40);

 
    while (flag==1){
    moveForward();
    modeChange(); 
    distance = readPing();
    delay(100);

   if(distance<=20)
  { 
    stop();
    delay(100);
    moveBackward();
    delay(500);
    stop();
    delay(200);
    distanceR = lookRight();
    delay(200);
    distanceL = lookLeft();
    delay(200);

    if(distanceR>=distanceL)
    {
      Serial.println("Turning Right");
      turnRight();
      moveForward();
    }else
    {
      Serial.println("Turning Left");
      turnLeft();
      moveForward();
    }
  }
  distance = readPing();
  }
}

void turnRight() {
 
  analogWrite (E1,155);
  analogWrite (E2,155); 
  digitalWrite(M1,LOW);    
  digitalWrite(M2,HIGH); 
  delay(1400);
  digitalWrite(M1,HIGH);    
  digitalWrite(M2,HIGH);
} 

void turnLeft()
{
  analogWrite (E1,155);
  analogWrite (E2,155); 
  digitalWrite(M1,HIGH);     
  digitalWrite(M2,LOW);
  delay(1600);
  digitalWrite(M1,HIGH);    
  digitalWrite(M2,HIGH);
}

void stop(void)                    //Stop
{ Serial.println("Stoping");
  digitalWrite(E1,0); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,0);   
  digitalWrite(M2,LOW);    
}   

void moveBackward() 
{
  Serial.println("Moving backwards");
  digitalWrite(M1,LOW);    
  digitalWrite(M2,LOW);  
  analogWrite (E2,200);   
  analogWrite (E1,200);
    delay(5);
  }
  

int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(120); 
    Serial.print("Looking right. Distance:");
    Serial.println(distance);
    return distance;
}

int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(120); 
    Serial.print("Looking left. Distance:");
    Serial.println(distance);
    return distance;
}

int readPing() 
{ 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveForward() {
 
     digitalWrite(M1,HIGH);    
     digitalWrite(M2,HIGH); 
     analogWrite (E2,200);      //PWM Speed Control
     analogWrite (E1,200);
     delay(5);
   
  

}


