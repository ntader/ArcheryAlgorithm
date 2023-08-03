#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//define values for constants
float DrawWeight = 22;
float DrawLength = 2.33333;
float ArrowMass = 0.00123253;
float AccelA = -32.2;
float g = 32.2;
//define State Var (for button)
int StateY;
int StateX;
//gyro calc
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;
 
void setup(){
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
//button setup
{
    Serial.begin(9600);
  pinMode(4,INPUT);
}
//new
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
}}
void loop(){
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI)-97.5;
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");
// add delay if needed (testing only)
delay(0);
//button
{
    int ButtonPress = digitalRead(4);
    if (ButtonPress==0)
    {
      StateY=StateY+1; 
      Serial.println(StateY);
      delay(0);
    if (StateY  >= 1001)
{
      StateY=0;
}
    }
}
//buttonX
{
    int ButtonPress = digitalRead(3);
    if (ButtonPress==0)
    {
      StateX=StateX+1;
      Serial.println(StateX);
      delay(90);
    if (StateX  >= 47)
{
      StateX=0;
}}}
    
  //x and y increments

float xDis = 5 * StateX;
float yDis = -0.01 * StateY; //remove the negative if target is above the arrow; for most cases, the target will be below (so keep the negative, for a negative y variable)

 //display items and objectives (angles)
 display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Display distance text
  display.print("Range:(");
  display.print(xDis,0);
  display.print(",");
   display.print(yDis);
  display.print(")ft");
  display.setCursor(0, 6);
  display.println("=-=---------------=-=");
  
  // Display Current Angle
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 16);
    display.println("Current Angle:");
  display.setCursor(0, 28);
  display.setTextSize(2);
  display.println(y,0);
 
  //Math for target angle
  float v = sqrt((DrawWeight*DrawLength)/ArrowMass);
  float TargetAngle = atan((sq(v)-sqrt(pow(v,4)-g*(g*sq(xDis)+2*yDis*sq(v))))/(g*xDis))/PI*180;
  display.setCursor(0, 56);
  display.setTextSize(1);
  display.print("Target Angle:");
  display.print(TargetAngle,2);
  Serial.print(v);

  //display up or down arrows
  int imageUP = 24;
  int imageDOWN = 25;
  int imageOK = 174;

  if (y < TargetAngle-0.51)
    {
      display.setTextSize(2);  //display up arrow
      display.setTextColor(WHITE);
      display.setCursor(40, 28);
      display.println(imageUP,0);
    } else if (y > TargetAngle+0.51)
    {
      display.setTextSize(2);  //display down arrow
      display.setTextColor(WHITE);
      display.setCursor(40, 28);
      display.println(imageDOWN,0);
     } else
    {
      display.setTextSize(2);  //display OK
      display.setTextColor(WHITE);
      display.setCursor(40, 28);
      display.println(imageOK,0);
    }
  // display OK
   display.display(); 
}