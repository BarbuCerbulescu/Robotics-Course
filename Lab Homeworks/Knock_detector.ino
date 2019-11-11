const int detectorPin=A0;
const int buzzerPin=11;
const int buttonPin=2;
const int threshold=1;

int detectorValue=0;
int buttonState=0;
int prevButtonState=0;
int ShutUpSignal=1;
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
   if(buttonState==0 && prevButtonState==1 && millis() - LastButtonPress>100)
   {
     ShutUpSignal=1;
     LastButtonPress=millis();
   }
   if(detectorValue>threshold && millis - LastKnock>=3000)
   {
      Serial.println("Knock");
      ShutUpSignal=0;
      LastKnock=millis();
   }
   Serial.println();
   if(ShutUpSignal==0 && millis() - LastKnock>=5000)
   {
     tone(buzzerPin,500,300);
     tone(buzzerPin,150,300);
     noTone(2000);
   }
}
