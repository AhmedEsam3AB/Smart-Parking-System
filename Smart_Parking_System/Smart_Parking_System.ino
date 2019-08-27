#include <Wire.h>

#include <LiquidCrystal.h>

#include <Keypad.h>

#include <Servo.h>


LiquidCrystal lcd(53, 52, 48, 49, 50, 51);
LiquidCrystal lcd2(12, 11, 2, 3, 4, 5);

const int buzzerPin = 6;
const int flamePin = 36;
int Flame = HIGH;


const int ledPin = 7;
const int ldrPin = A7;

int led1 = 40;
int led2 = 41;
int led3 = 42;
int led4 = 43;
int led5 = 44;
int led6 = 45;

const byte rows = 4;
const byte columns = 4;
char buttons[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowP[rows] = {29, 28, 27, 26};
byte columnsP[columns] = {25, 24, 23, 22};
Keypad pad = Keypad( makeKeymap(buttons), rowP, columnsP, rows, columns );

Servo servo[6];

int timecar[6] = {0, 0, 0, 0, 0, 0};
int cost[6] = {0, 0, 0, 0, 0, 0};

int pos[6];
int start[6];

Servo servoIN;
Servo servoEX;

int  Available = 6;
#define ServoEX    13
#define ServoM    10
#define Exit     9
#define In        8
#define BarLow   90
#define BarUp     0
#define CAPACITY  6



const int place[] = {A5, A4, A3, A2, A1, A0};
byte val[6];

unsigned long millistime;

void setup() {
  for (int r = 0; r < 6; r++)
  {
    pinMode(place[r], INPUT);
  }
  servo[0].attach(35);
  servo[1].attach(34);
  servo[2].attach(33);
  servo[3].attach(32);
  servo[4].attach(31);
  servo[5].attach(30);

  servo[0].write(90);
  servo[1].write(90);
  servo[2].write(90);
  servo[3].write(90);
  servo[4].write(90);
  servo[5].write(90);

  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.print("Calculate Budget Of Parking ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);

  servoIN.attach(ServoM);
  servoEX.attach(13);

  lcd2.begin(16, 2);
  lcd2.print("Space left for");
  lcd2.setCursor(0, 1);

  pinMode(Exit, INPUT);
  pinMode(In, INPUT);

  servoIN.write(BarLow);
  servoEX.write(BarLow);
  pinMode(buzzerPin, OUTPUT);
  pinMode(flamePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode (led1, OUTPUT);
  pinMode (led2, OUTPUT);
  pinMode (led3, OUTPUT);
  pinMode (led4, OUTPUT);
  pinMode (led5, OUTPUT);
  pinMode (led6, OUTPUT);
}

void loop() {


  if (Available >= 1) {
    lcd2.clear();
    lcd2.setCursor(1, 0);
    lcd2.print("Space left for");
    lcd2.setCursor(0, 1);
    lcd2.print(Available);
    lcd2.print(" cars");
  } else {
    lcd2.clear();
    lcd2.setCursor(1, 0);
    lcd2.print("Sorry!");
    lcd2.setCursor(0, 1);
    lcd2.print("No place left!");
  }



  if (digitalRead(In) == 0)
  {
    if (Available != 0) {
      Available--;
      servoIN.write(BarUp);
      delay(2000);
      servoIN.write(BarLow);
    }
  }
  if (digitalRead(Exit) == 0)
  {
    if (Available != CAPACITY) {
      Available++;
      servoEX.write(BarUp);
      delay(2000);
      servoEX.write(BarLow);

    }

  }


  for (int c = 0; c < 6; c++) {
    val[c] = digitalRead(place[c]);
  }
  millistime = millis() / 1000 ;

  for (int x = 0; x < 6; x++) {
    if (val[x] == 0) {
      if (pos[x] != 181) {
        closeServo(x);
      }
      if (!start[x]) {
        start[x] = millistime ;
      }
      timecar[x] = millistime -  start[x] ;
      cost[x] = timecar[x] / 2 ;


    }
  }

  lcd.setCursor(0, 0);
  lcd.print("1-");
  lcd.print(timecar[0]);
  lcd.print("M-");
  lcd.print(cost[0]);
  lcd.print("EGP");
  lcd.setCursor(0, 1);
  lcd.print("2-");
  lcd.print(timecar[1]);
  lcd.print("M-");
  lcd.print(cost[1]);
  lcd.print("EGP");
  lcd.setCursor(0, 2);
  lcd.print("3-");
  lcd.print(timecar[2]);
  lcd.print("M-");
  lcd.print(cost[2]);
  lcd.print("EGP");
  lcd.setCursor(0, 3);
  lcd.print("4-");
  lcd.print(timecar[3]);
  lcd.print("M-");
  lcd.print(cost[3]);
  lcd.print("EGP");

  lcd.setCursor(14, 0);
  lcd.print("5-");
  lcd.print(timecar[4]);
  lcd.print("M");
  lcd.setCursor(15, 1);
  lcd.print(cost[4]);
  lcd.print("EGP");
  lcd.setCursor(14, 2);
  lcd.print("6-");
  lcd.print(timecar[5]);
  lcd.print("M");
  lcd.setCursor(15, 3);
  lcd.print(cost[5]);
  lcd.print("EGP");

  delay(500);


  char key = pad.getKey();
  if (key == '1') {
    openServo(0);
  }
  else if (key == '2') {
    openServo(1);
  }
  else if (key == '3') {
    openServo(2);
  }
  else if (key == '4') {
    openServo(3);
  }
  else if (key == '5') {
    openServo(4);
  }
  else if (key == '6') {
    openServo(5);
  }
  Flame = digitalRead(flamePin);
  if (Flame == LOW)
  {
    digitalWrite(buzzerPin, HIGH);
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
  }
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus <= 300) {

    digitalWrite(ledPin, HIGH);
  }

  else {

    digitalWrite(ledPin, LOW);
  }

  if (analogRead(A0) < 500)
  {
    digitalWrite(led1, LOW);
  }
  else
  {
    digitalWrite(led1, HIGH);
  }

  if (analogRead(A1) < 500)
  {
    digitalWrite(led2, LOW);
  }
  else
  {
    digitalWrite(led2, HIGH);
  }

  if (analogRead(A2) < 500)
  {
    digitalWrite(led3, LOW);
  }
  else
  {
    digitalWrite(led3, HIGH);
  }

  if (analogRead(A3) < 500)
  {
    digitalWrite(led4, LOW);
  }
  else
  {
    digitalWrite(led4, HIGH);
  }

  if (analogRead(A4) < 500)
  {
    digitalWrite(led5, LOW);
  }
  else
  {
    digitalWrite(led5, HIGH);
  }

  if (analogRead(A5) < 500)
  {
    digitalWrite(led6, LOW);
  }
  else
  {
    digitalWrite(led6, HIGH);
  }
}

void openServo(int i)
{

  for (pos[i] = 180; pos[i]  >= 90; pos[i]  -= 1) {
    servo[i].write(pos[i]);
    delay(15);
  }
  delay (2000);

  timecar[i] = 0;
  cost[i] = 0;
  start[i] = !start[i];
  lcd.clear();

}

void closeServo(int x)
{
  for (pos[x] = 90; pos[x] <= 180; pos[x] += 1) {
    servo[x].write(pos[x]);
    delay(15);
  }
  delay (100);

}
