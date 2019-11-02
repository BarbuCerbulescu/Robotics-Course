int potPin1 = A0;
int potPin2=A2;
int potPin3=A4;    
int rPin =9;
int bPin=10;
int gPin=11;     

void setup() {
  pinMode(rPin, OUTPUT);
  pinMode(bPin,OUTPUT);
  pinMode(gPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
 int potVal1 = analogRead(potPin1);
 Serial.print("Red Value=");
 Serial.print(potVal1 /4);
 Serial.print("\n");
  int potVal2=analogRead(potPin2);
  Serial.print("Blue Value=");
 Serial.print(potVal2 /4);
 Serial.print("\n");
  int potVal3=analogRead(potPin3);
  Serial.print("Green Value=");
 Serial.print(potVal3 /4);
 Serial.print("\n");
 Serial.println();
 
   analogWrite(rPin,potVal1 /4);
   analogWrite(bPin,potVal2 /4);
   analogWrite(gPin,potVal3 /4);
}
