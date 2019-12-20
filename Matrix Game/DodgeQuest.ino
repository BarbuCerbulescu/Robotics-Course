#include<LedControl.h>
LedControl lc=LedControl(12,11,10,1);

#include <LiquidCrystal.h>
#include<EEPROM.h>

LiquidCrystal lcd(8,9,5,4,3,2);

const int pinX=A0;
const int pinY=A1;
const int pinSW=7;
const int pinContrast=6;
const int minThreshold=400;
const int maxThreshold=600;

bool attackMatrix[10][10];
const bool GameOverDisplay[8][8]={
  {1,1,0,0,0,0,1,1},
  {1,1,1,0,0,1,1,1},
  {0,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,0,0},
  {0,0,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,0},
  {1,1,1,0,0,1,1,1},
  {1,1,0,0,0,0,1,1},
};

int pl=0;
int pc=7;
int rowTopArrow=0;
int colTopArrow=pc;
int rowLatArrow=pl;
int colLatArrow=0;
int colSpikes=0;
int rowSpikes=0;
bool StateSpikes=1;
bool StateLatArrow=1;
bool ShowSpikes=0;
unsigned long TimeWave=0;
int rowFrontWave=0;
int colFrontWave=3;
int rowLeftWave=0;
int colLeftWave=0;
int rowRightWave=0;
int colRightWave=0;
bool waveState=0;
struct poz{
  int l;
  int c;
}meteorQueue[13];
int pr=0,ul=0;
int meteorState=4;
unsigned long LastMeteorChange=0;
int addx[4]={1,-1,0,0};
int addy[4]={0,0,1,-1};
int meteorLowerIndex[3]={0,1,5};
int meteorUpperIndex[3]={1,5,13};
bool stopTopArrow=0;
bool stopLatArrow=0;


unsigned long TimeTopArrow=0;
unsigned long TimeLatArrow=0;
unsigned long TimeSpikes=0;
unsigned long LastDisp=0;
unsigned long LastXChange=0;
unsigned long LastYChange=0;
unsigned long LastLifeLost=0;
unsigned long SetupEnds=0;
bool invulState=1;

const int eeAddress=0;
const int maxScore=2046;

const char MainMenuOption[4][16]={
  "Start Game","HighScores","Settings","Info"
};
const char SettingsMenuOption[3][16]={
  "Starting Level","Bonus Lives","Back to Menu" 
};
const char InfoMenuRow1Option[4][16]={
  "Made by Barbu","Code available","BarbuCerbulescu/","Matrix Game/"
};
const char InfoMenuRow2Option[4][16]={
  "Cebulescu","on my Github","Robotics-Course/","DodgeQuest.ino"
};
const int NrOfOptions[8]={4,0,0,3,4,2,0,4};
//Nr of options for each submenu

int CurrentSubMenu=0;
int SelectedOption=0;
int level=0;
int StartingLevel=1;
int score=0;
int prevLives=3;
int lives=3;
int HighScore;
bool GameOver=0;
int bonusLivesModifier=1;
unsigned long countdown;

unsigned long LevelTime[5]={0,10000,20000,30000,30000};
unsigned long LevelBegins=0;
unsigned long LastLcdClear=0;
bool swState=1;
bool lastSwState=1;

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


void handleMovement(){
 int xValue = analogRead(pinY);
 int yValue= analogRead(pinX);
 if(millis()-LastDisp>1000){
 LastDisp=millis();
 }
 if(millis()-LastXChange>=200){
  if(xValue<minThreshold && pc<7){
    pc++;
    LastXChange=millis();
  }
  else if(xValue>maxThreshold && pc>0){
    pc--;
    LastXChange=millis();
  }
 }
 if(millis()-LastYChange>=200){
  if(yValue<minThreshold && pl<7){
    pl++;
    LastYChange=millis();
  }
  else if(yValue>maxThreshold && pl>0){
    pl--;
    LastYChange=millis();
  }
 }
}

int ChangeOption()
{
  //detects if the user has moved the joystick vertically
  //and changes the selected option on the main menu accordingly
  int maxThreshold=600;
  int minThreshold=400;
  if(millis()-LastYChange>=200){
  int yValue=analogRead(pinX);
   if (yValue > maxThreshold) {
    //moves to the option above
      if (SelectedOption > 0) {
       SelectedOption--;
      } 
      else {
        SelectedOption=NrOfOptions[CurrentSubMenu]-1;
      }
      lcd.clear();
  }
  if (yValue < minThreshold) {
    //moves to the option below
    if (SelectedOption < NrOfOptions[CurrentSubMenu]-1 ) {
        SelectedOption++;
    } 
    else {
        SelectedOption = 0;
    }
    lcd.clear();
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
                    prevLives=3;
                    lives=3;
                    score=-(int(pow(4,level))+1)*lives*bonusLivesModifier;//it will become 0 when level becomes Starting level
                    GameOver=0;
                    LastLifeLost=millis();
                    for(int i=0;i<10;i++){
                      for(int j=0;j<10;j++)attackMatrix[i][j]=0;
                    }
                    CurrentSubMenu=1;
                  }
                  else if(SelectedOption==1)CurrentSubMenu=2;
                  //enter the highscore submenu
                  else if(SelectedOption==2)CurrentSubMenu=3;
                  //enter the settings submenu
                  else CurrentSubMenu=7;
                }
                else if (CurrentSubMenu==2)CurrentSubMenu=0;
                  //the highscore submenu
                else if(CurrentSubMenu==3) {
                  //the settings submenu
                  if(SelectedOption==0)CurrentSubMenu=4;
                  //enter the starting level selection sub-menu
                  else if(SelectedOption==1)CurrentSubMenu=5;
                  //enter the bonus lives submenu
                  else CurrentSubMenu=0;
                  //go back to the main-menu
                }
                else if(CurrentSubMenu==4){
                  //set the starting level to the
                  //selected option and go back to
                  //settings menu
                  StartingLevel=SelectedOption+1;
                  if(StartingLevel==4)BonusLivesModifier=1;
                  CurrentSubMenu=3;
                }
                else if(CurrentSubMenu==5){
                  //the bonus lives submenu
                  if(SelectedOption==1 && StartingLevel<4)bonusLivesModifier=2;
                  //if "No" is selected lose Bonus Lives and get double points
                  else bonusLivesModifier=1;//if "Yes" is selected enable Bonus Lives
                  CurrentSubMenu=3;
                }
                else CurrentSubMenu=0;
                //go back to menu upon game over or if in the Info subemnu
                SelectedOption=0;
                lcd.clear();
         }
    }
    lastSwState = swState;
}



void handleAttacks(){

 if(level<=3 && stopTopArrow==0){
  //Top Arrow
  if(rowTopArrow>=8){
        if(millis()-LastMeteorChange<5000 || level!=3 ||(pc<2 || pc>=6|| pl<2 || pc>=6)){
        attackMatrix[colTopArrow+1][(rowTopArrow-1)+1]=0;
        colTopArrow=pc;
        rowTopArrow=0;
        }
        else stopTopArrow=1;
      }
   lc.setLed(0,colTopArrow,rowTopArrow-1,true);
   lc.setLed(0,colTopArrow,rowTopArrow,true);
   if(millis()-TimeTopArrow>325){
      lc.clearDisplay(0);
      TimeTopArrow=millis();
      attackMatrix[colTopArrow+1][(rowTopArrow-1)+1]=0;
      rowTopArrow++;
      attackMatrix[colTopArrow+1][rowTopArrow+1]=1;
   }
 }
 else if(level==4){
  //Wave:
  if(pc>colFrontWave+1 ||pc<colFrontWave-1)waveState=1;
  lc.setLed(0,colFrontWave,rowFrontWave,true);
  if(colLeftWave>=0 && colLeftWave<8 && rowLeftWave>=0 && rowLeftWave<8) lc.setLed(0,colLeftWave,rowLeftWave,true);
  if(colRightWave>=0 && colRightWave<8 && rowRightWave>=0 && rowRightWave<8)lc.setLed(0,colRightWave,rowRightWave,true);
  if(millis()-TimeWave>=500){
    lc.clearDisplay(0);
    TimeWave=millis();
    attackMatrix[colFrontWave+1][rowFrontWave+1]=0;
    attackMatrix[colLeftWave+1][rowLeftWave+1]=0;
    attackMatrix[colRightWave+1][rowRightWave+1]=0;
    rowFrontWave++;
    rowLeftWave++;
    rowRightWave++;
    if(waveState==1){ 
      if(colLeftWave>=0 && colLeftWave<8)colLeftWave--;
      if(colRightWave>=0 && colRightWave<8)colRightWave++;
    }
    attackMatrix[colFrontWave+1][rowFrontWave+1]=1;
    attackMatrix[colLeftWave+1][rowLeftWave+1]=1;
    attackMatrix[colRightWave+1][rowRightWave+1]=1;
    if(rowFrontWave>=8){
      attackMatrix[colFrontWave+1][(rowFrontWave-1)+1]=0;
      colFrontWave=pc;
      colRightWave=pc+1;
      colLeftWave=pc-1;
      rowFrontWave=0;
      rowLeftWave=-1;
      rowRightWave=-1;
      waveState=0;
    }
  }
 }
 
 //Lateral Arrow:
    if(level>=2 && stopLatArrow==0){
     if(colLatArrow==9 || colLatArrow==0){
      if(millis()-LastMeteorChange<5000 || level!=3 ||(pc<2 || pc>=6|| pl<2 || pc>=6)){
          if(colLatArrow==9) attackMatrix[(colLatArrow-1)+1][(rowLatArrow)+1]=0;
          else  attackMatrix[(colLatArrow+1)+1][rowLatArrow+1]=0;
          if(7-pc<pc){
            colLatArrow=8;
            StateLatArrow=0;
            }
          else{  
            colLatArrow=0;
            StateLatArrow=1;
          }
          rowLatArrow=pl;
        }
       else stopLatArrow=1;
     } 
     lc.setLed(0,colLatArrow-1,rowLatArrow,true);
     lc.setLed(0,colLatArrow,rowLatArrow,true);
     if(millis()-TimeLatArrow>325){
        lc.clearDisplay(0);
        TimeLatArrow=millis();
        if(StateLatArrow==1){
          attackMatrix[(colLatArrow-1)+1][rowLatArrow+1]=0;
          colLatArrow++;
          attackMatrix[colLatArrow+1][rowLatArrow+1]=1;
        }
        else{
           attackMatrix[(colLatArrow+1)+1][rowLatArrow+1]=0;
           colLatArrow--;
           attackMatrix[colLatArrow+1][rowLatArrow+1]=1;
        }
     }
   }
   //Spikes:
   if(pl>3 && ShowSpikes==0){
    ShowSpikes=1;
    colSpikes=pc-3;
   }
   if(ShowSpikes==1){
     for(int i=0;i<3;i++)
     {
      for(int j=1;j<=rowSpikes+1;j++){
        lc.setLed(0,colSpikes+(2*i+1),8-j,true);
      }
     }
     if(millis()-TimeSpikes>=1000){
      if(StateSpikes==1){
        rowSpikes++;
        for(int i=0;i<3;i++){
        attackMatrix[colSpikes+(2*i+1)+1][(8-rowSpikes)+1]=1;
        }
      }
      else{
          for(int i=0;i<3;i++){
            attackMatrix[colSpikes+(2*i+1)+1][(8-rowSpikes)+1]=0;
          }
          rowSpikes--;
        }
        if(rowSpikes==2 && StateSpikes==1)StateSpikes=0;
        else if(rowSpikes==-1 && StateSpikes==0){
          rowSpikes=0;
          StateSpikes=1;
          ShowSpikes=0;
          Serial.println('0');
        }
        TimeSpikes=millis();
        lc.clearDisplay(0);
       }
     }

//Meteor:
if(level==3){
  if(millis()-LastMeteorChange>=350 && meteorState<4){
    for(int i=meteorLowerIndex[meteorState-1];i<meteorUpperIndex[meteorState-1];i++)attackMatrix[meteorQueue[i].l +1][meteorQueue[i].c +1]=0;
    meteorState++;
    if(meteorState==4){
      stopTopArrow=0;
      stopLatArrow=0;
    }
    LastMeteorChange=millis();
    lc.clearDisplay(0);
  }
  else if(meteorState==4 && stopTopArrow==1 && stopLatArrow==1 && pl>=2 && pl<=6 && pc>=2 && pc<=6){
    meteorState=0;
    for(int i=0;i<13;i++){
        meteorQueue[i].l=-10;
        meteorQueue[i].c=-10;
      }
  }
  if(meteorState==0){
      int auxl;
      int auxc;
      if(random(2)%2==0)auxl=1;
      else auxl=-1;
      if(random(2)%2==0)auxc=1;
      else auxc=-1;
      pr=0;
      ul=0;
      meteorQueue[pr].l=pl+auxl;
      meteorQueue[pr].c=pc+auxc;
      while(pr<5){
        int x=meteorQueue[pr].l;
        int y=meteorQueue[pr].c;
        for(int i=0;i<4;i++){
          int c=0;
          for(int j=0;j<13 && c==0;j++){
            if(x+addx[i]==meteorQueue[j].l && y+addy[i]==meteorQueue[j].c)c=1;
            }
            if(c==0){
              ul++;
              meteorQueue[ul].l=x+addx[i];
              meteorQueue[ul].c=y+addy[i];
            }
        }
        pr++;
      } 
    }
  if(meteorState>=1 && meteorState<=3){
    for(int i=meteorLowerIndex[meteorState-1];i<meteorUpperIndex[meteorState-1];i++){
      lc.setLed(0,meteorQueue[i].c,meteorQueue[i].l,true);
      attackMatrix[meteorQueue[i].l +1][meteorQueue[i].c +1]=1;
    }
  }
 }

}



void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0,leftHeart);
  lcd.createChar(1,rightHeart);
  
  pinMode(pinContrast,OUTPUT);
  pinMode(7,INPUT_PULLUP);
  analogWrite(pinContrast,110);
  
  lc.shutdown(0,false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
  Serial.begin(9600);

  float f;
  EEPROM.get(eeAddress,f);
  HighScore=(int) f;
  
  SetupEnds=millis();
  while(millis()-SetupEnds<=3000){
    lcd.setCursor(0,0);
    lcd.print("Welcome,");
    lcd.setCursor(0,1);
    lcd.print("brave warrior!");
  }
  lcd.clear();
  while(millis()-SetupEnds<=6000){
    lcd.setCursor(0,0);
    lcd.print("Are you ready");
    lcd.setCursor(0,1);
    lcd.print("to face Bob?");
  }
  lcd.clear();
}


void loop() {
   if(CurrentSubMenu==0)
  {
    //the main menu
    lcd.setCursor(0,0);
    lcd.print("DodgeQuest");
    lcd.setCursor(0,1);
    lcd.write('>');
    lcd.print(MainMenuOption[SelectedOption]);
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
     if(millis()-LastLifeLost>=1000 && invulState==1)invulState=0;//disable invulnerabilty after 1 second
        if(attackMatrix[pc+1][pl+1]==1 && invulState==0){
          //if you get hit lose a life and get invulnerabilty for 1 sec
          lives--;
          invulState=1;
          LastLifeLost=millis();
          lcd.clear();
      }
      if(lives==0){
       for(int i=0;i<8;i++)
       {
         for(int j=0;j<8;j++)
         {
           lc.setLed(0,j,i,GameOverDisplay[j][i]);
         }
       }
      }
     else{
        handleMovement();
        handleAttacks();
        if(millis()%100<50)lc.setLed(0,pc,pl,true);//displays avatar so as to make it flicker
        else lc.setLed(0,pc,pl,false);
     }
     if(millis()-LevelBegins>=LevelTime[level] || level==StartingLevel-1)
    {
      //increases level once the time duration for it has passed
      if(level==3){
        //in the 3rd level the meteor and the arrows will wait for each other to finish so as not to fill the screen with too many attacks
        //once the 3rd level is finished and the meteor no longer appears,arrows are allowed to spawn again if the the meteor 
        //was intrerrupted by the level becoming 4
        stopTopArrow=0;
        stopLatArrow=0;
      }
      if(lives==prevLives && lives<3 && bonusLivesModifier==1)lives++;//if you have bonus lives enabled restore a life if you haven't lost any this lvl
      score=score+(int(pow(4,level))+1)*lives*bonusLivesModifier;//the amount added is actually (4^level)*lives*bonusLivesModifier due to conversion issues
      prevLives=lives;
      level++;
      countdown=1000;//resets the countdown to a big number so that it immediately changes to the corect one
      LevelBegins=millis();
    }
    if(level<=4 && lives>0){
      lcd.setCursor(0,0);
      lcd.print("LVL:");
      lcd.print(level);
      unsigned long aux=(LevelTime[level]-(millis()-LevelBegins))/1000; //seconds untill end of level
      if(countdown>aux){
        lcd.clear();
        countdown=aux;
      }
      lcd.print("  Time ");
      lcd.print(countdown/60); //number of minutes till countdown ends
      lcd.print(':');
      lcd.print(countdown%60);//number of seconds till countdown ends
      lcd.setCursor(0,1);
      lcd.print("Lives: ");
      for(int i=0;i<lives;i++){
        lcd.write(byte(0));
        lcd.write(byte(1));
        lcd.print(" ");
      }
    }
    else if(level>4){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Congratulations!");
      lcd.setCursor(0,1);
      lcd.print("You have won!");
      GameOver=1;
      delay(3000);
      lcd.clear();
      lc.shutdown(0,false);
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Game over!");
      lcd.setCursor(0,1);
      lcd.print("You have died!");
      GameOver=1;
      delay(3000);
      lcd.clear();
    }
    if(GameOver==1){
      if(score==maxScore){
      lcd.setCursor(0,0);
      lcd.print("You've achieved ");
      lcd.setCursor(0,1);
      lcd.print("maximum score!");
      delay(3000);
      lcd.clear();
      }
      else if(score>=HighScore){
         lcd.setCursor(0,0);
         lcd.print("You have the");
         lcd.setCursor(0,1);
         lcd.print("highest score");
         delay(3000);
         lcd.clear();
      }
     if(score>HighScore){
         HighScore=score;
         float f=(float) score;
         EEPROM.put(eeAddress,f);
       }
      lcd.print("Your Score is :");
      lcd.setCursor(0,1);
      lcd.print(score);
      delay(2000);
      lcd.clear();
      CurrentSubMenu=6;
    }   
  }
  if(CurrentSubMenu==2){
    //the highscores submenu
    lcd.setCursor(0,0);
    lcd.print("Highest: ");
    lcd.print(HighScore);
    lcd.setCursor(0,1);
    lcd.print("Maximum: ");
    lcd.print(maxScore);
    ChangeSubMenu();
  }
  if(CurrentSubMenu==3){
    //the settings submenu
    lcd.setCursor(0,0);
    lcd.print("Settings");
    lcd.setCursor(0,1);
    lcd.write('>');
    lcd.write(SettingsMenuOption[SelectedOption]);
    ChangeOption();
    ChangeSubMenu();
  }
  if(CurrentSubMenu==4){
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
  if(CurrentSubMenu==5){
    //the bonus lives sub-menu
    lcd.setCursor(0,0);
    lcd.print("Have bonus lives");
    lcd.setCursor(0,1);
    lcd.print('>');
    if(SelectedOption==0)lcd.print("Yes");
    else lcd.print("No");
    ChangeOption();
    ChangeSubMenu();
    }
  if(CurrentSubMenu==6){
    //the game over submenu
    lc.clearDisplay(0);
    lcd.setCursor(0,0);
    lcd.print("Press the button");
    lcd.setCursor(0,1);
    lcd.print("to exit.");
    ChangeSubMenu();
  }
  if(CurrentSubMenu==7){
    //the info submenu
    lcd.setCursor(0,0);
    lcd.print(InfoMenuRow1Option[SelectedOption]);
    lcd.setCursor(0,1);
    lcd.print(InfoMenuRow2Option[SelectedOption]);
    ChangeOption();
    ChangeSubMenu();
  }
}
