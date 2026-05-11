// Devastator_Robot_8.ino
#include <Servo.h>
#include <NewPing.h>  //https://bitbucket.org/teckel12/arduino-new-ping/downloads/
#include "DFRobot_HuskylensV2.h"
#include "Wire.h"

// constants won't change. Used here to set a pin number:
const int ledPin = LED_BUILTIN;  // the number of the LED pin

// Variables will change:
int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 1000;  // interval at which to blink (milliseconds)

// Create object
HuskylensV2 huskylens;

// #define TRIG_PIN A4   // Change to A2 to prevent conflict with Husky Lens camera
// #define ECHO_PIN A5   // Chamge to A3 to prevent conflict with Husky Lens camera
#define TRIG_PIN A2
#define ECHO_PIN A3

#define MAX_DISTANCE 200

int E1 = 5;  //M1 Speed Control
int E2 = 6;  //M2 Speed Control
int M1 = 4;  //M1 Direction Control
int M2 = 7;  //M1 Direction Control
char input;
bool flag = 0;
int centerThreshold = 50;  // Tolerance for steering
int xCenter = 320;
int distance = 100;
int Item = 0;
unsigned long currentMillis = millis();

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

void setup(void) {
  /* Setup baud rate for serial communication */
  
  Serial.begin(115200);
  delay(1000);
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
 //set the digital pin as output:
  pinMode(ledPin, OUTPUT);
//  Serial.flush();
  Serial.println("Hello World");
  
}


void loop() {

  if (Serial.available() > 0) {
    input = Serial.read();
    /* Manual mode */
    if (input == 'f' || input == 'b' || input == 'l' || input == 'r' || input == 's') {

      manual();
    }
    /* Automatic mode */
    else if (input == 'A') {
      flag = 1;
      automatic();
    } else if (input == 'L') {
      bootLeft();
    } else if (input == 'R') {
      bootRight();
    } else if (input == 'C') {
      camera();
    }
  }

  // Code to flash LED using a timer

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}


/* Reading char 'M' on serial for stopping current mode */
void modeChange() {
  if (Serial.available() > 0) {
    input = Serial.read();

    if (input == 'M') {
      Serial.println("Changing Mode ");
      flag = 0;
      stop();
    }
  }
}

/*Turn 180 Right */
void bootRight() {
  Serial.println("Hard right turn");
  hardRight();
  stop();
}


/*Turn 180 Left */
void bootLeft() {

  Serial.println("Hard left turn");
  hardLeft();
  stop();
}

/********************************************* MANUAL MODE ********************************************/

void manual() {
  if (input == 'f') {
    moveForward();
  } else if (input == 'b') {
    moveBackward();
  } else if (input == 'l') {
    turnLeft();
  } else if (input == 'r') {
    turnRight();
  } else if (input == 's') {
    stop();
  }
}

/********************************************* AUTOMATIC MODE ********************************************/

void automatic(void) {

  Serial.println(distance);
  Serial.println("Automatic");
  int distanceR = 0;
  int distanceL = 0;
  delay(40);


  while (flag == 1) {
    moveForward();
    modeChange();
    distance = readPing();
    delay(100);

    if (distance <= 20) {
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

      if (distanceR >= distanceL) {
        Serial.println("Turning Right");
        turnRight();
        moveForward();
      } else {
        Serial.println("Turning Left");
        turnLeft();
        moveForward();
      }
    }
    distance = readPing();
  }
}

void turnRight() {

  analogWrite(E1, 155);
  analogWrite(E2, 155);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  delay(1200);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}

void hardRight() {
  analogWrite(E1, 155);
  analogWrite(E2, 155);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  delay(2400);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}

void turnLeft() {
  analogWrite(E1, 155);
  analogWrite(E2, 155);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  delay(1200);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}

void hardLeft() {
  analogWrite(E1, 155);
  analogWrite(E2, 155);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  delay(2400);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
}

void stop(void)  //Stop
{
  Serial.println("Stoping");
  digitalWrite(E1, 0);
  digitalWrite(M1, LOW);
  digitalWrite(E2, 0);
  digitalWrite(M2, LOW);
}

void moveBackward() {
  Serial.println("Moving backwards");
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  analogWrite(E2, 200);
  analogWrite(E1, 200);
  delay(5);
}

void moveForward() {
  //Serial.println("Moving forwards");
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E2, 200);  //PWM Speed Control
  analogWrite(E1, 200);
  delay(5);
}
int lookRight() {
  myservo.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(120);
  Serial.print("Looking right. Distance:");
  Serial.println(distance);
  return distance;
}

int lookLeft() {
  myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo.write(120);
  Serial.print("Looking left. Distance:");
  Serial.println(distance);
  return distance;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

/********************************************* Camera AI MODE ********************************************/


void camera(void) {

  while (!huskylens.begin(Wire)) {
    Serial.println("Waiting ... ");
    Serial.println("Huskylens not found! Check wiring.");
    delay(100);
  }

  // Switch to Object Tracking mode
  huskylens.switchAlgorithm(ALGORITHM_OBJECT_TRACKING);
  Serial.println("Switching to tracking mode");
  flag = 1;

  while (flag == 1) {

    modeChange();  // Check for 'M' character

    // Request results from the sensor
    huskylens.getResult(ALGORITHM_OBJECT_TRACKING);

    if (huskylens.available(ALGORITHM_OBJECT_TRACKING)) {
      //Serial.println("Tracking ...");
      // Get the object closest to the center crosshair
      auto target = huskylens.getCachedCenterResult(ALGORITHM_OBJECT_TRACKING);

      // Check if object is left, right, or center
      Serial.print(RET_ITEM_NUM(target, Result, xCenter));
      Serial.print(" : ");
      Serial.println(RET_ITEM_NUM(target, Result, yCenter));

      if ((RET_ITEM_NUM(target, Result, xCenter)) < (320 - centerThreshold)) {
        //Object on left: Turn Left
        Serial.println(" Turning Left");
        modeChange();
        turnLeft();
      }

      else if ((RET_ITEM_NUM(target, Result, xCenter)) > (320 + centerThreshold)) {
        //Object on right: Turn Right
        Serial.println(" Turning Right");
        modeChange();
        turnRight();
      }
      
      else {

        //Move Forward
        Serial.println(" Moving Forward");
        modeChange();
        moveForward();
      }
    }

    else {
      // Target Lost Stop
      Serial.println(" No Target");
      digitalWrite(E1, 0);
      digitalWrite(M1, LOW);
      digitalWrite(E2, 0);
      digitalWrite(M2, LOW);
    }
  }
}