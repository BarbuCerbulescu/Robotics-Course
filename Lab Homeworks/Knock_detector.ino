const int detectorPin=A0;
const int buzzerPin=11;
const int buttonPin=2;
const int threshold=1;

int detectorValue=0;
int buttonState=0;
int prevButtonState=0;
int ShutUpSignal=1;
int FirstKnock=0;
//FirstKnock stores the time of the first knock after the button was pressed
//or the first actual knock initially
int LastKnock=0;
int LastButtonPress=0;

void setup() {
  pinMode(detectorPin,INPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  detectorValue=analogRead(detectorPin);
   Serial.println(detectorValue);
   buttonState=digitalRead(buttonPin);
  //due to using the Arduino PULLUP resistance the values for the button are switched:1 when the button is not pressed,0 when the button is pressed
   if(buttonState==0 && prevButtonState==1 && millis() - LastButtonPress>100)
   {
     ShutUpSignal=1;
     LastButtonPress=millis();
   }
   if(detectorValue>threshold && millis - LastKnock>=1000)
   {
      LastKnock=millis();
      Serial.println("Knock");
      if(ShutUpSignal==1)FirstKnock=LastKnock;
      ShutUpSignal=0;
   }
   Serial.println();
   if(ShutUpSignal==0 && millis() - FirstKnock>=5000)
   {
     tone(buzzerPin,500,300);
     tone(buzzerPin,150,300);
     noTone(2000);
   }
}
