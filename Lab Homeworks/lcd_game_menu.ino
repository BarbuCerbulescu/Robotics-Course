#include <LiquidCrystal.h>
#include<EEPROM.h>

LiquidCrystal lcd(12,11,5,4,3,2);

const int Ypin=A1;
const int pinSW=7;
const int pinContrast=10;
const int eeAddress=0;

const char MenuOption[3][11]={
  "Start Game","HighScore","Settings"
};

const int NrOfOptions[5]={3,0,2,5,0};
//Nr of options for each submenu

int CurrentSubMenu=0;
int SelectedOption=0;
int level=0;
int StartingLevel=1;
int score=0;
int lives=3;
int HighScore=0;

int LevelBegins=0;
int LastYChange=0;
bool joyMoved=false;
int swState=1;
int lastSwState=1;

byte leftHeart[] = {
  B00000,
  B01110,
  B01111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000
};
byte rightHeart[] = {
  B00000,
  B01110,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000,
  B00000
};

int ChangeOption()
{
  //detects if the user has moved the joystick vertically
  //and changes the selected option on the main menu accordingly
  int maxThreshold=600;
  int minThreshold=400;
  if(millis()-LastYChange>=200){
  int yValue=analogRead(Ypin);
   if (yValue > maxThreshold && joyMoved == false) {
    //moves to the option above
      if (SelectedOption > 0) {
       SelectedOption--;
      } 
      else {
        SelectedOption=NrOfOptions[CurrentSubMenu]-1;
      }
      joyMoved=true;
      lcd.clear();
  }
  if (yValue < minThreshold && joyMoved == false) {
    //moves to the option below
    if (SelectedOption < NrOfOptions[CurrentSubMenu]-1 ) {
        SelectedOption++;
    } 
    else {
        SelectedOption = 0;
    }
    joyMoved=true;
    lcd.clear();
 }
 if(yValue >= minThreshold && yValue <= maxThreshold) {
         joyMoved=false;
      }
 LastYChange=millis();
 }
}
void ChangeSubMenu(){
  //detects if the user has pressed the button and
  //enters the appropiate submenu if so
   swState = digitalRead(pinSW);
    if (swState != lastSwState) {
         if (swState == LOW) {
                if(CurrentSubMenu==0){
                  //if the user is in main-menu
                  if(SelectedOption==0){
                    //enter the in-game submenu
                    level=StartingLevel-1;
                    //level will become StartingLevel
                    //upon entering 
                    CurrentSubMenu=1;
                  }
                  else if(SelectedOption==2)CurrentSubMenu=2;
                  //enter the settings sub-menu
                }
                else if (CurrentSubMenu==4)CurrentSubMenu=0;
                //returns the user to the main-menu
                //upon finishing the game
                else if(CurrentSubMenu==2) {
                  //the settings submenu
                  if(SelectedOption==0)CurrentSubMenu=3;
                  //enter the starting level selection sub-menu
                  else CurrentSubMenu=0;
                  //go back to the main-menu
                }
                else{
                  StartingLevel=SelectedOption+1;
                  CurrentSubMenu=2;
                }
                SelectedOption=0;
                lcd.clear();
         }
    }
    lastSwState = swState;
}
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0,leftHeart);
  lcd.createChar(1,rightHeart);
  
  pinMode(10,OUTPUT);
  pinMode(7,INPUT_PULLUP);
  
  analogWrite(pinContrast,110);
  
  float f;
  EEPROM.get(eeAddress,f);
  HighScore=(int) f;
}

void loop() {
   if(CurrentSubMenu==0)
  {
    //the main menu
    lcd.setCursor(0,0);
    lcd.print("My Awesome Game");
    lcd.setCursor(0,1);
    lcd.write('>');
    lcd.print(MenuOption[SelectedOption]);
    if(SelectedOption==1){
      lcd.print(':');
      lcd.print(HighScore);
    }
    ChangeOption();
    //despite the name(couldn't come up with a better one)
    //the function doesn't automatically change the 
    //selected option.It only does so if it detects
    // a significant change in the yValue
    ChangeSubMenu();
    //same thing here
  }
  if(CurrentSubMenu==1){
    //the in-game menu
     if(millis()-LevelBegins>=2000 || level==StartingLevel-1)
    {
      level++;
      LevelBegins=millis();
    }
    if(level<=5){
      lcd.setCursor(0,0);
      lcd.print("LEVEL ");
      lcd.print(level);
      score=level*3; 
      lcd.print(" Score:");
      lcd.print(score);
      lcd.setCursor(0,1);
      lcd.print("Lives: ");
      for(int i=0;i<lives;i++){
        lcd.write(byte(0));
        lcd.write(byte(1));
        lcd.print(" ");
      }
    }
    else{
      lcd.clear();
      lcd.print("Congratulations!");
      lcd.setCursor(0,1);
      lcd.print("You have won!");
      delay(5000);
      lcd.clear();
      CurrentSubMenu=4;
    }
    if(score>HighScore){
      HighScore=score;
      float f=(float) score;
      EEPROM.put(eeAddress,f);
    }
  }
  if(CurrentSubMenu==2){
    //the settings submenu
    if(SelectedOption==0)lcd.setCursor(0,0);
    else lcd.setCursor(0,1);
    lcd.write('>');
    lcd.setCursor(1,0);
    lcd.print("Starting Level");
    lcd.setCursor(1,1);
    lcd.print("Back to Menu");
    ChangeOption();
    ChangeSubMenu();
  }
  if(CurrentSubMenu==3){
    //the  starting level setting submenu
    lcd.setCursor(0,0);
    lcd.print("Set starting lvl");
    lcd.setCursor(0,1);
    lcd.write('>');
    lcd.print("Level ");
    lcd.print(SelectedOption+1);
    ChangeOption();
    ChangeSubMenu();
  }
  if(CurrentSubMenu==4){
    //the "you have won.press the button to go
    //back to the main menu" submenu
    lcd.setCursor(0,0);
    lcd.print("Press the button");
    lcd.setCursor(0,1);
    lcd.print("to exit.");
    ChangeSubMenu();
  }
 }
