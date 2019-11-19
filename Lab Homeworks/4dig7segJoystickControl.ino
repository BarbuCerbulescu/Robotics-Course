const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 7;
const int pinE = 8;
const int pinF = 11;
const int pinG = 12;
const int pinDP = 13;
const int pinD1 = 5;
const int pinD2 = 6;
const int pinD3 = 9;
const int pinD4 = 10;
const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;
int dpState = LOW;

const int pinSW=1;
const int Xpin=A0;
const int Ypin=A2;
int xValue=0;
int yValue=0;
int swState=0;
int lastSwState=0;
bool joyMoved=false;
int minThreshold=400;
int maxThreshold=600;

int CurrentDisplay=0;
bool DisplayLocked[noOfDisplays]={
  false,false,false,false
};
byte DigitOnDisplay[noOfDisplays]={
  0,0,0,0
};//memorizes a digit for each particular display

int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int Display[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

int LastXChange=0;
int LastYChange=0;
int LastDisp=0;
int firstStep=0;

void displayDigit(byte digit, byte decimalPoint) {
for (int i = 0; i < segSize - 1; i++) {
digitalWrite(segments[i], digitMatrix[digit][i]);
}
digitalWrite(segments[segSize - 1], decimalPoint);
}

void clearDigits()
{
  //this function makes it so that the digits from the other
  //displays won't show up on the current one while changes
  //are being made
  
  
  for(int i=0;i<segSize-1;i++)
  {
    digitalWrite(segments[i],LOW);
  }
}
void activateDisplay(int num) {
for (int i = 0; i < noOfDisplays; i++) {
digitalWrite(Display[i], HIGH);
}
digitalWrite(Display[num], LOW);
}

void setup() {
  for (int i = 0; i < segSize - 1; i++)
  {
       pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++)
  {
       pinMode(Display[i], OUTPUT);
  }
  pinMode(1,INPUT_PULLUP);
}

void loop() {
  
  for(int i=0;i<noOfDisplays;i++)
  {
    //always shows the digit memorized for a particular display if it is locked
    if(DisplayLocked[i]==true)
    {
       clearDigits();
       activateDisplay(i);
       displayDigit(DigitOnDisplay[i],HIGH);
    }
  }
  if(DisplayLocked[CurrentDisplay]==false && millis()-LastXChange>=100){
    //change the value on the current display if it's not locked
    xValue=analogRead(Xpin);
    if (xValue < minThreshold && joyMoved == false) {
      if (DigitOnDisplay[CurrentDisplay] > 0) {
           DigitOnDisplay[CurrentDisplay]--;
      } 
      else {
         DigitOnDisplay[CurrentDisplay] = 9;
      }
      joyMoved = true;
    }
    if (xValue > maxThreshold && joyMoved == false) {
      if (DigitOnDisplay[CurrentDisplay] < 9) {
          DigitOnDisplay[CurrentDisplay]++;
      } 
      else {
          DigitOnDisplay[CurrentDisplay] = 0;
      }
      joyMoved = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold) {
        joyMoved=false;
    }
    LastXChange=millis();
   }
  if(millis()-LastDisp>=100){
  //the delay makes it so that the display will flicker if
  //it is not locked.If it is locked it will be displayed
  //normally in the for at the beggining of loop()
  clearDigits();
  activateDisplay(CurrentDisplay);
  displayDigit(DigitOnDisplay[CurrentDisplay],HIGH);
  LastDisp=millis();
  }
  if(millis()-LastYChange>=300)
  {
    //change current display using the y axis
    //note:the displays are indexed from right to the left
      yValue=analogRead(Ypin);
      if (yValue > maxThreshold && joyMoved == false) {
        //change the current display to the one on the right
          if (CurrentDisplay > 0) {
           CurrentDisplay--;
          } 
          else {
           CurrentDisplay = 3;
          }
          joyMoved = true;
      }
      if (yValue < minThreshold && joyMoved == false) {
        //change current display to the one on the left
        if (CurrentDisplay < 3) {
            CurrentDisplay++;
        } 
        else {
            CurrentDisplay = 0;
        }
        joyMoved = true;
      }
      if (yValue >= minThreshold && yValue <= maxThreshold) {
          joyMoved=false;
      }
     LastYChange=millis();
  }
    swState = digitalRead(pinSW);
    if (swState != lastSwState) {
         if (swState == LOW) {
                 //if the switch is pressed lock or unlock the selected display
                 if(DisplayLocked[CurrentDisplay]==false)DisplayLocked[CurrentDisplay] = true;
                 else DisplayLocked[CurrentDisplay]=false;
         }
    }
    lastSwState = swState;
}
