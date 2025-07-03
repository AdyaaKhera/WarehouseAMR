#include <Servo.h>
#include <Wire.h>

#define SERVO_ARM 4   // Arm rotation servo pin
#define SERVO_GRIP 2  // Gripper servo pin

Servo armServo;
Servo gripServo;

#define ENA 3
#define IN1 5
#define IN2 6
#define ENB 11
#define IN3 9
#define IN4 10

#define R_S 13  // Right line sensor
#define L_S 12  // Left line sensor

#define TRIG 7
#define ECHO 8

#define LED LED_BUILTIN

unsigned long lastSeenTime = 0;

void setup() {
    Serial.begin(9600);

    //setting motor pins as output
    pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

    //setting line sensor and ultrasonic pins
    pinMode(R_S, INPUT); pinMode(L_S, INPUT);
    pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  
    pinMode(LED, OUTPUT);

    //attaching servos
    armServo.attach(SERVO_ARM);
    gripServo.attach(SERVO_GRIP);

    //setting intial positions
    armServo.write(0);
    gripServo.write(0);
    delay(1000);

    grabLoad();  //grabbing load when AMR is switched on
}

void loop() {
    int distance = getDistance();

    if ((digitalRead(R_S) == 1) || (digitalRead(L_S) == 1)) {
        lastSeenTime = millis();
    }

    unsigned long timeOffline = millis() - lastSeenTime;

    if (distance < 5) {
        Serial.println("Obstacle detected!");
        digitalWrite(LED, HIGH);
        avoidObstacle();
    } 
    else if (timeOffline > 60000) { //offline for 1 minute -> drop load
        dropLoad();
    } 
    else {
        digitalWrite(LED, LOW);
        followLine();
    }
}

//movement functions

void moveForward() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, 75); analogWrite(ENB, 75);
}

void moveBackward() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENA, 75); analogWrite(ENB, 75);
}

void turnRight() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnLeft() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void stopMotors() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void rotateLeft() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void rotateRight() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

//obstacle avoidance function

void avoidObstacle() {
    Serial.println("Avoiding obstacle...");

    moveBackward(); delay(1000);
    rotateLeft(); delay(700);
    moveForward(); delay(1700);
    rotateRight(); delay(750);
    moveForward(); delay(2000);
    rotateRight(); delay(600);

    unsigned long startTime = millis();
    while ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) {
        moveForward();
        if (millis() - startTime > 5000) {
            Serial.println("Fail-safe: Line not found.");
            stopMotors();
            return;
        }
    }

    moveForward(); delay(350);

    startTime = millis();
    while (digitalRead(R_S) != 1) {
        rotateRight();
        if (millis() - startTime > 3000) {
            Serial.println("Fail-safe: Sensor not aligned.");
            stopMotors();
            return;
        }
    }

    Serial.println("Line reacquired.");
}

void followLine() {
    if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 1)) {
        moveForward();
    } 
    else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) {
        turnRight();
    } 
    else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) {
        turnLeft();
    } 
    else {
        stopMotors();  //line lost or endpoint reached
    }
}

//calculating distance

int getDistance() {
    digitalWrite(TRIG, LOW); delayMicroseconds(2);
    digitalWrite(TRIG, HIGH); delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    long duration = pulseIn(ECHO, HIGH);
    return duration * 0.034 / 2;
}

//pick and place functions

void grabLoad() {
    gripServo.write(90); delay(500);  
    armServo.write(90); delay(500);  
}

void dropLoad() {
    armServo.write(0); delay(500);    
    gripServo.write(0); delay(500);  
}
