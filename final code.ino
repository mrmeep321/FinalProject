#include <Servo.h>

Servo serv;
Servo servb;

Servo inclaw;
Servo outclaw;

int bbut = 2;
int fbut = 8;
int sbut = 7;
int alight = 4;
int blight = 5;
int clight = 6;
int lmot = 3;
int rmot = 9;
int inclawmot = 10;
int outclawmot = 11;

int prevstate;
bool reset = false;

int indeg = 0;
int outdeg = 0;


bool debounce(int btn) {
  bool instate = digitalRead(btn);
  delay(10);
  if(digitalRead(btn) == instate) {
    return instate;
  }else {
    return false;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serv.attach(rmot);
  servb.attach(lmot);
  inclaw.attach(inclawmot);
  outclaw.attach(outclawmot);
  pinMode(bbut, INPUT);
  pinMode(fbut, INPUT);
  pinMode(alight, OUTPUT);
  pinMode(blight, OUTPUT);
  pinMode(clight, OUTPUT);
  pinMode(sbut, INPUT);
  pinMode(A0, INPUT);

  inclaw.write(0);
  outclaw.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(analogRead(A0) + " " + analogRead(A1));
  /*serv.write(10);
  servb.write(10);*/

  if(debounce(7) && reset == false) {
    prevstate++;
    if(prevstate == 4) prevstate = 0;
    reset = true;
  } else if(!debounce(7) && reset) {
    reset = false;
  }

  switch(prevstate) {
    case 0:
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      break;
    case 1:
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      break;
    case 2:
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      break;
    case 3:
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      break;
  }

  //if in move mode - forward btn pushes forwards, back pushes back
  //if in claw modes - forward button advances amount of degrees per ms
  if(prevstate == 0) {
    double scale = ((double)analogRead(A0))/((double)1024);
    if(scale == 0) scale = 0.01;
    Serial.println(scale);
    if(debounce(fbut)) {
      Serial.println(90 + (90*scale));
      serv.write(90 + (90*scale));
      servb.write(90 + (90*scale));
    } else if(debounce(bbut)) {
      Serial.println(90 - (90*scale));
      serv.write(90 - (90*scale));
      servb.write(90 - (90*scale));
    } else {
      serv.write(90);
      servb.write(90);
    }
  } else if(prevstate == 1) {
    double scale = ((double)analogRead(A0))/((double)1024);
    if(scale == 0) scale = 0.01;
    int degchange = 180 * (scale);
    if(debounce(fbut)) {
      indeg += degchange;
      if(indeg >= 180) indeg = 180;
      if(indeg <= 0) indeg = 0;
    }
    if(debounce(bbut)) {
      indeg -= degchange;
      if(indeg >= 180) indeg = 180;
      if(indeg <= 0) indeg = 0;
    }
    inclaw.write(indeg);
    delay(1000/degchange);
  } else if(prevstate == 2) {
    double scale = ((double)analogRead(A0))/((double)1024);
    if(scale == 0) scale = 0.01;
    int degchange = 180 * (scale);
    if(debounce(fbut)) {
      outdeg += degchange;
      if(outdeg >= 180) outdeg = 180;
      if(outdeg <= 0) outdeg = 0;
    }
    if(debounce(bbut)) {
      outdeg -= degchange;
      if(outdeg >= 180) outdeg = 180;
      if(outdeg <= 0) outdeg = 0;
    }
    outclaw.write(outdeg);
    delay(1000/degchange);
  }
}

