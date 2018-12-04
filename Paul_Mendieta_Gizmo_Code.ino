#include <Servo.h>
#include <Stepper.h>

// *PIN DEFINITIONS*

// Ultrasonic sensor
const int Trigger = 7;   //Pin digital 2 para el Trigger del sensor
const int Echo = 6;   //Pin digital 3 para el Echo del sensor

// Mic Sensor
int sensorPin=3;

// Stepper motor
const int stepsPerRevolution = 500; 
const int in_pin_1 = 8;
const int in_pin_2 = 10;
const int in_pin_3 = 9;
const int in_pin_4 = 11;

// *VAR DEFINITIONS*

boolean val =0;
int banderastepper=0;
int pos = 0;    
int bandera=0;

// *STARTING MOTORS

Stepper myStepper(stepsPerRevolution, in_pin_1, in_pin_2, in_pin_3, in_pin_4);
Servo myservo;

// *MAIN CODE*

void setup() {
  
  Serial.begin(9600);  // Starts the serial communication
  
  // Ultrasonic Sensor
  pinMode(Trigger, OUTPUT); // Sets the trigPin as an Output
  pinMode(Echo, INPUT); // Sets the echoPin as an Input
  digitalWrite(Trigger, LOW);

  // Stepper Motor
  pinMode(in_pin_1, OUTPUT);
  pinMode(in_pin_2, OUTPUT);
  pinMode(in_pin_3, OUTPUT);
  pinMode(in_pin_4, OUTPUT);
  myStepper.setSpeed(60); // set the speed at 60 rpm:

  // Servo Motor
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  
  // Mic Sensor setup for Interruption 
  attachInterrupt(sensorPin,microfono,HIGH);
}
void loop() {
  
  // Ultrasonic sensor
  long t; // Time
  long d; //distance
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH); 
  d = t/59;             
  Serial.print("Distancia Ultrasonico: ");
  Serial.print(d);      
  Serial.println("cm");
  Serial.print("Valor microfono: ");
  Serial.println (val);
  delay(100); //Pause of 100ms
  
  // Track de distance of the water level
    if (d>8 && bandera==1) {
         for (pos = 83; pos >= 0; pos -= 1) { // goes from 83 degrees to 0 degrees
         myservo.write(pos); // tell servo to go to position in variable 'pos'
         delay(100); // waits 100ms for the servo to reach the position
          }   
          bandera=0;
      }
     if (d<=6 && bandera==0){
        for (pos = 0; pos <= 83; pos += 1) { // goes from 0 degrees to 83 degrees
        // in steps of 1 degree
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(100); // waits 100ms for the servo to reach the position
        }     
        bandera=1; 
        delay(5000); // waits 5000ms for the servo to reach the position.
      }    
}

// Microphone code
void microfono(){
  val =digitalRead(sensorPin);
  if (val==1 && banderastepper==0){
     Serial.println("clockwise");
     myStepper.step(stepsPerRevolution);
     delay(50);
     banderastepper=1;
           }
     delay(500);
     if (val==0 && banderastepper==1) {
     Serial.println("counterclockwise");
     myStepper.step(-stepsPerRevolution);
     delay(50);
     banderastepper=0;
     }
}
