#define DEBUG 0

//  default speeds
#define SPEED1 250
#define SPEED2 100
#define SPEED3 200

//  define distances
#define FRONT20 1300
#define FRONT30 2000
#define LEFT20 250
#define LEFT30 80
#define RIGHT20 225
#define RIGHT30 50

//  pin numbers L/R stands for left/right, F/B for forward/backward
const int lpwm_pin 6  // ENA
const int rpwm_pin 3  // ENB
const int pinLB = 5;  // IN1 
const int pinLF = 7;  // IN2 
const int pinRB = 2;  // IN3 
const int pinRF = 4;  // IN4
const int trigPin = A1;
const int echoPin = A0;
const unsigned int left_ir = A5;                 
const unsigned int right_ir = A4;

//  init distances
unsigned int left_dis = 0;
unsigned int right_dis = 0;
unsigned int front_dis = 0;

void setup_motors(void)
{
  pinMode(pinLB, OUTPUT); 
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT); 
  pinMode(pinRF, OUTPUT);
  pinMode(lpwm_pin, OUTPUT);
  pinMode(rpwm_pin, OUTPUT);
}

//  Sets the trigPin as an Output and the echoPin as an Input
void  setup_ultrasonic(void)
{
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

void set_speed(unsigned char left, unsigned char right)
{
  analogWrite(lpwm_pin,left);
  analogWrite(rpwm_pin,right);
} 

void forward(void)
{
  set_speed(SPEED1, SPEED1);
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,LOW); 
  digitalWrite(pinLF,HIGH);
}

void back(void)
{
  if (left_dis / 10 < right_dis / 10)
    set_speed(SPEED1, SPEED2);
  else if (left_dis / 10 > right_dis / 10)
    set_speed(SPEED2, SPEED1);
  else
    set_speed(SPEED1, SPEED1);
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW); 
  delay(150);
}

void turnL(void)
{ 
  set_speed(SPEED2, SPEED1);
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,LOW); 
  digitalWrite(pinLF,HIGH);
}

void turnR(void)
{
  set_speed(SPEED1, SPEED2);
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,LOW); 
  digitalWrite(pinLF, HIGH);
}

void read_sensors(void)
{
  //  Clears the values
  front_dis = 0;
  left_dis = 0;
  right_dis = 0;

  //  Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  //  Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //  Reads the echoPin, returns the sound wave travel time in microseconds
  front_dis = pulseIn(echoPin, HIGH);

  //  Reads the value of the left sensor
  left_dis = analogRead(left_ir);

  //  Reads the value of the right sensor     
  right_dis = analogRead(right_ir);
}

void  move(void)
{
  if (front_dis < FRONT20 && (left_dis > LEFT20 || right_dis > RIGHT30))
    back();
  else if (left_dis / 10 > right_dis / 10)
    turnL();
  else if (left_dis / 10 < right_dis / 10)
    turnR();
  else
    forward();
}

void debug_function(void)
{
  //  Starts the serial communication
  Serial.begin(9600); 
  setup_ultrasonic();

  while (1)
  {
    read_sensors();
    if (front_dis < FRONT20 && (left_dis > LEFT20 || right_dis > RIGHT30))
      Serial.println("back");
    else if (left_dis / 10 > right_dis / 10)
      Serial.println("turnLS");
    else if (left_dis / 10 < right_dis / 10)
      Serial.println("turnRS");
    else
      Serial.println("forward");

    Serial.print("Front: ");
    Serial.println(front_dis);
    Serial.print("left: ");
    Serial.println(left_dis);
    Serial.print("right: ");
    Serial.println(right_dis);
    delay(500);
  }
}


void setup() {
  if (DEBUG)
  {
    debug_function();
    exit (0);
  }
  setup_motors();
  setup_ultrasonic();
  set_speed(SPEED1, SPEED1);
  delay(5000);
}

void loop() {
  read_sensors();
  move();
  delay(10);
}

