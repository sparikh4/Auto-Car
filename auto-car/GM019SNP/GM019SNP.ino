#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // Tx pin of the bluetooth module must be connected to Rx pin on arduino
// Rx pin of the bluetooth module must be connected to Tx pin on arduino
//const int led1 = 2;
//const int led2 = 3;
//const int led3 = 4;

//Random Initializations------------------------
String inputString;
String buttonId;
int buttonState = 0;
int pinNum = 0;
int BTdat = -1;
int Ttime = 200;
int TTime = 450;
int fireL = 9;
int fireN = 51;
int ledl=35;
int ledr=7;
int ledrear=37;
int eon=45;
int eoff=40;
int horn=41;
int rhorn=42;
int lrl=9;
int lrr=51;
int fan=22;
int runn=43;

void setup() {
  Serial.begin(9600);
  //start communication with bluetooth module
  bluetoothSerial.begin(9600);
  pinMode(A3, INPUT);
  pinMode(lrl,INPUT);
  pinMode(lrr,INPUT);
  //pinMode(fireL, OUTPUT);
  //pinMode(fireN, OUTPUT);
  pinMode(ledl, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledrear, OUTPUT);
  pinMode(eon, OUTPUT);
  //pinMode(eoff, OUTPUT);
    pinMode(runn, OUTPUT);
  pinMode(horn, OUTPUT);
  //pinMode(rhorn, OUTPUT);
  pinMode(fan, OUTPUT);
  USini();
}
void HC()
{ if ( bluetoothSerial.available()) {
    while ( bluetoothSerial.available()) {
      char inChar = (char) bluetoothSerial.read(); //read the input
      bluetoothSerial.println(int(inChar) + 1);
      BTdat = int(inChar);
      Serial.println(BTdat);
      infra();
    }
  }
}




//INTS to live by--------------------------------


const int usT[4] = {8, 5, 3,31}; //TRIG Pins
const int usE[4] = {15, 13, 2,30}; //Echo Pins
const int motors[4] = {4, A9, A11, A10}; //LF,RR,LR,RF
/*
  Yellow - left forward
  Green- right forward
  White-  left rev
  Orange- right rev
*/
long distance[4], duration[4];

//INTS That Matter-----------------------------

int nos = 4;
int dis = 15;
int dis2=8;
int rdis = 25;
int goFor = 1;
int fsp = 245;
int rfsp = 130;

//Announcements---------------------------------



void USdisini()
{
  infra();
  for (int i = 0; i < nos; i++)
  {
    digitalWrite(usT[i], LOW);
    delayMicroseconds(2);
    digitalWrite(usT[i], HIGH);
    delayMicroseconds(5);
    digitalWrite(usT[i], LOW);
    duration[i] = pulseIn(usE[i], HIGH);
    distance[i] = ((duration[i] / 58.138) * .4); //*2.54 for cm//*.39 for inch
    //send (Message[i]);
//    if (distance[i] > 250)
//      Serial.print(-1);
//    else
      Serial.print(distance[i]);
    Serial.print("<-");
    if (i == 0)
      Serial.print(" RIGHT\t");
    else if (i == 1)
      Serial.print(" FRONT\t");
    else if (i == 2)
      Serial.print(" LEFT\t");
    else if (i == 3)
      Serial.print(" Rear\t");
    delay(50);
  }
  Serial.println("");
}
void USini()
{
  Serial.println("");
  Serial.println("Start");
  for (int i = 0; i < nos; i++)
  {
    pinMode(usE[i], INPUT);         //echo
    pinMode(usT[i], OUTPUT);        //trig
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(motors[i], OUTPUT);
  }
}
void forward(int tim = 3, int rsp = fsp - 44, int lsp = 255)
{
  Serial.println("--Forward");

  //delay(500);
  
  analogWrite(motors[3], rsp);
  analogWrite(motors[0], lsp);
  delay(tim);

  //  //motoroff();

}
void motoroff()
{
  for (int i = 0; i < 4; i++)
  {
    analogWrite(motors[i], 0);
  }
  delay(100);
  Serial.println("MotorsOFF");
}
void Straight(int i = 6)
{
    digitalWrite(runn, HIGH);
  USdisini();
  Serial.println("**StraightINI");
  int noc = 0;
  while (distance[0] > dis2 && distance[1] > dis && distance[2] > dis2 && noc < i)
  {
    digitalWrite(runn, LOW);
   digitalWrite(runn, HIGH); 
    Serial.println("**Straight");
    if (distance[0] > dis2 && distance[1] > dis && distance[2] > dis2 && noc < i)
    {
    forward();
    USdisini();
    noc++;
    }
    else
    break;
  }
  motoroff();
  delay(70);
  digitalWrite(runn, LOW);
}

void reverse(int tim = 5, int rsp = 210, int lsp = 140)
{
  digitalWrite(ledrear,HIGH);
  Serial.println("--Reverse");
  //digitalWrite(rhorn,HIGH);
  //delay(20);
  //digitalWrite(rhorn,LOW);
  analogWrite(motors[1], rsp);
  analogWrite(motors[2], lsp);
  delay(tim);
  //  motoroff();
  digitalWrite(ledr,LOW);
}

void rStraight(int i = 3)
{
  digitalWrite(ledrear,HIGH);
  USdisini();
  int noc = 0;
  while (distance[3] > dis && noc < i)
  {
    USdisini();
    if (distance[3]<dis)
    break;
    Serial.println("**rStraight");
    reverse();
    Serial.print(".................................");
    USdisini();
    noc++;
  }
  motoroff();
  digitalWrite(ledrear,LOW);
}

void infra()
{
int infral=digitalRead(lrl);
int infrar=digitalRead(lrr);

  Serial.print("FireStat");
  Serial.println(infral);
  Serial.println(infrar);
  if (infral==0 || infrar==0)
  {
    //motoroff();
    //digitalWrite(fireL, HIGH);
    //digitalWrite(fireL, LOW);
    BTdat = 100;
    Serial.println("yo_in_fire");
    digitalWrite(fan, LOW);
    delay(5000);
    digitalWrite(fan, HIGH);
    return;
  }
  else
    return;
}

void turn(String j, int tim = 500) //LF,RR,LR,RF
{
  //motoroff();
  if(j=="l")
    digitalWrite(ledl,HIGH);
  else
    digitalWrite(ledr,HIGH);
  delay(100);
  int r = 100, l = 153;
  if (j == "l") {
    Serial.println("Left");
    l = 100;
    r = 135;
  }
  analogWrite(motors[0], l);
  analogWrite(motors[3], r);
  delay(Ttime);
  motoroff();
  if(j=="l")
    digitalWrite(ledl,LOW);
  else
    digitalWrite(ledr,LOW);
}
void fturn(String j, int tim = TTime) //LF,RR,LR,RF
{
  if(j=="l")
    digitalWrite(ledl,HIGH);
  else
    digitalWrite(ledr,HIGH);
  delay(100);
  motoroff();
  delay(100);
  int r = 100, l = 153;
  if (j == "l") {
    Serial.println(".........-------FULL     Left");
    l = 100;
    r = 135;
  }
  analogWrite(motors[0], l);
  analogWrite(motors[3], r);
  delay(TTime);
  motoroff();
  if(j=="l")
    digitalWrite(ledl,LOW);
  else
    digitalWrite(ledr,LOW);
}


void autono(int i = 20)
{
  infra();
  if (distance[1]>dis)
  Straight();
  int dist = 10;
  if (distance[0] < dist || distance[1] < dist || distance[2] < dist)
  {
    BTcontrol();
    fturn("r");    //right
    Straight();
    if (distance[0] > dist && distance[1] > dist && distance[2] > dist)
    {
      fturn("r");    //another right
      Straight();
    }
  }
  else
  {
    fturn("l");
    rStraight();
  }
}



void BTcontrol()
{
  {
    digitalWrite(fan,HIGH);
    int i;
    HC();
    //infra();
    if (BTdat == 100)
    {
      //delay(1000000);
    }
    if (BTdat == 53) {
      i = 0;
      Serial.println("-------------Power Off---------------");
    }
    else if (BTdat == 54) {
      i = -1;
      Serial.println("-------------Power On---------------");
    }
    while (i == -1)
    {
      Serial.println("ON");
      infra();
      if (BTdat == 100)
      {
        //delay(1000000);
      }
      else if (BTdat == 52) //no4
      {
        
        Straight();
      }
      else if (BTdat == 48)
      {
        //digitalWrite(ledrear,HIGH);
        rStraight();
        BTdat = 0;
        //digitalWrite(ledrear,LOW);
      }
      else if (BTdat == 51)      //right no3
      {
        Serial.println("===============right");
        //digitalWrite(ledr,HIGH);
        turn("r");           //add args
        BTdat = 52;
        //digitalWrite(ledr,LOW);
      }
      else if (BTdat == 49)/// no1
      {
        Serial.println("=================left");
        //digitalWrite(ledl,HIGH);
        turn("l");           //add args
        BTdat = 52;
        //digitalWrite(ledl,LOW);
      }
      else if (BTdat == 53) { //5
        i = 0;
        motoroff();
        Serial.println("-------------Power Off---------------");
        //digitalWrite(eoff,HIGH);
        //delay(20);
        //digitalWrite(eoff,LOW);
        break;
      }
      else if (BTdat == 54) { //6
        i = -1;
        Serial.println("-------------Power On---------------");
        digitalWrite(eon,HIGH);
        delay(500);
        digitalWrite(eon,LOW);
      }
      else if (BTdat == 55) { //7
        //digitalWrite(ledl,HIGH);
        fturn("l");
        BTdat = 0;
        //BTdat = 52;
        //digitalWrite(ledl,LOW);
      }
      else if (BTdat == 56) { //8
        //digitalWrite(ledr,HIGH);
        fturn("r");
        BTdat = 0;
        //BTdat = 52;
        //digitalWrite(ledr,LOW);
      }

      else if (BTdat == 57) { //9
        autono();
      }

      else if (BTdat == 50) {   //2
        digitalWrite(horn,HIGH);
        delay(500);
        digitalWrite(horn,LOW);
        BTdat = 0;
      }
      if (BTdat == 51)
        BTdat = 1;
      HC();
      //motoroff();
    }// end while
  }
}

void loop() {
 // USdisini();
HC();
BTcontrol();
  //BTcontrol();
  // reverse();
  //rStraight();

}
