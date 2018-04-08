/*
   Controlling LEDs with bluetooth from a android phone

   Use the Arduino Bluetooth Controller app that is availble on the Google Play store
   download it with the link below.

    https://play.google.com/store/apps/details?id=com.AppDroidRM.arduinobluetoothcontroller

*/

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
int Ttime = 150;
int TTime = 450;
int fireL = 17;
int fireN = 16;


void setup() {
  Serial.begin(9600);
  //set the digital pins
  //  pinMode(led1, OUTPUT);
  //  pinMode(led2, OUTPUT);
  //  pinMode(led3, OUTPUT);
  //start communication with bluetooth module
  bluetoothSerial.begin(9600);
  //set all pins to off as default to start with
  //  digitalWrite(led1, LOW);
  //  digitalWrite(led2, LOW);
  //  digitalWrite(led3, LOW);
  pinMode(A3, INPUT);
  pinMode(fireL, OUTPUT);
  pinMode(fireN, OUTPUT);
  USini();
}
void HC()
{ if ( bluetoothSerial.available()) {
    while ( bluetoothSerial.available()) {
      char inChar = (char) bluetoothSerial.read(); //read the input

      bluetoothSerial.println(int(inChar) + 1);
      BTdat = int(inChar);
      Serial.println(BTdat);
    }
  }
}




//INTS to live by--------------------------------


const int usT[6] = {5, 14, 8, 6, 3, 31}; //TRIG Pins
const int usE[6] = {13, 9, 15, 7, 2, 30}; //Echo Pins
const int motors[4] = {4, A9, A11, A10}; //LF,RR,LR,RF
/*
  Yellow - left forward
  Green- right forward
  White-  left rev
  Orange- right rev
*/
long distance[6], duration[6];

//INTS That Matter-----------------------------

int nos = 6;
int dis = 25;
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
    if (distance[i] > 250)
      Serial.print(-1);
    else
      Serial.print(distance[i]);
    Serial.print("<-");
    if (i == 0)
      Serial.print(" Front\t");
    else if (i == 1)
      Serial.print(" R-Front\t");
    else if (i == 2)
      Serial.print(" Right\t");
    else if (i == 3)
      Serial.print(" Rear\t");
    else if (i == 4)
      Serial.print(" Left\t");
    else
      Serial.println(" L-Front\t");
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
void Straight(int i = 10)
{
  USdisini();
  int noc = 0;
  while (distance[0] > dis && distance[1] > dis && distance[5] > dis && noc < i)
  {
    Serial.println("**Straight");
    forward();
    USdisini();
    noc++;
  }
  motoroff();
}

void reverse(int tim = 5,int rsp = 210, int lsp = 140)
{
  Serial.println("--Reverse");
  analogWrite(motors[1], rsp);
  analogWrite(motors[2], lsp);
  delay(tim);
  //  motoroff();
}

void rStraight(int i = 2)
{
  USdisini();
  int noc = 0;
  while (distance[3] > rdis && noc < i)
  {
    USdisini();
    if (distance[3] < rdis)
      break;
    Serial.println("**rStraight");
    reverse();
    Serial.print(".................................");
    USdisini();
    noc++;
  }
  motoroff();
}

void infra()
{
  int infraval = analogRead(A3);
  Serial.print("FireStat");
  Serial.println(infraval);
  if (infraval <= 50)
  {
    motoroff();
    digitalWrite(fireL, HIGH);
    digitalWrite(fireL, LOW);
    BTdat = 100;
  }
  else
    digitalWrite(fireN, HIGH);
}

void turn(String j, int tim = Ttime) //LF,RR,LR,RF
{
  motoroff();
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

}
void fturn(String j, int tim = TTime) //LF,RR,LR,RF
{
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

}
void BTcontrol()
{
  {
    int i;
    HC();
    infra();
    if (BTdat == 100)
    {
      delay(1000000);
    }
    if (BTdat == 53) {
      i = 0;
      Serial.println("-------------Power Off---------------");
    }
    else if (BTdat == 54) {
      i = -1;
      Serial.println("-------------Power On---------------");
    }
    while (i == -1)   ///locha
    {
      infra();
      if (BTdat == 100)
      {
        delay(1000000);
      }
      else if (BTdat == 52)
      {
        Straight();
      }
      else if (BTdat == 48)
      {
        rStraight();
        BTdat = 0;
      }
      else if (BTdat == 51)      //right
      {
        Serial.println("===============right");
        turn("r");           //add args
        BTdat = 52;
      }
      else if (BTdat == 49)
      {
        Serial.println("=================left");
        turn("l");           //add args
        BTdat = 52;
      }
      else if (BTdat == 53) {
        i = 0;
        Serial.println("-------------Power Off---------------");
        break;
      }
      else if (BTdat == 54) {
        i = -1;
        Serial.println("-------------Power On---------------");
      }
      else if (BTdat == 55) {
        fturn("l");
        BTdat = 52;
      }
      else if (BTdat == 56) {
        fturn("r");
        BTdat = 52;
      }

      if (BTdat == 51 || BTdat == 49)
        BTdat = 1;
      HC();
    }// end while
  }


}


void autono(int i=20)
{
   infra();
   Straight();
   int dist=10;
   if(distance[0] > dist && distance[1] > dist && distance[5] > dist)
   {
      fturn("r");    //right
      if(distance[0] > dist && distance[1] > dist && distance[5] > dist)
      {
        fturn("r");    //another right
      }
   }
   
}



void loop() {
  HC();
  BTcontrol();
  //BTcontrol();
  // reverse();
  //rStraight();

}


