//--------------------------------------------------------------------------
//MeetingCostMeter
//--------------------------------------------------------------------------

// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
//using VL53L0X library for Arduino(https://github.com/pololu/vl53l0x-arduino)
#include "VL53L0X.h"

//lcd init pin
const int rs = 7, rw = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, rw , en, d4, d5, d6, d7);

//length sensor
VL53L0X sensor;

//servo object
Servo myservo;

//pin setting
const int ServoPin = 2;
const int SWPin_1 = 3;
const int SWPin_2 = 4;
const int SWPin_3 = 5;

#define VER_COMENT "MeetingCostMeter V1.0.0"

//--------------------------------------------------------------------------
//setup()
//--------------------------------------------------------------------------
void setup() {
  //init LCD
  {
    //set up the LCD's number of columns and rows:
    lcd.begin(16, 2);

    //write lcd
    lcd.setCursor(0, 0);
    lcd.print(VER_COMENT);
    lcd.setCursor(0, 1);
    lcd.print("Initialize...");
  }

  //init serial
  {
    Serial.begin(9600);
    Serial.println("===System Initialize...===");
  }
  //init I2C
  {
    Wire.begin();
    //ToF init
    sensor.init();
    sensor.setTimeout(500);
    /*
      //set long rangemode
      // lower the return signal rate limit (default is 0.25 MCPS)
      sensor.setSignalRateLimit(0.1);
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    */
    //HIGH_ACCURACY default 200ms 200000
    sensor.setMeasurementTimingBudget(50000);
  }

  //init sw(3,4,5)
  {
    pinMode(SWPin_1, INPUT_PULLUP);
    pinMode(SWPin_2, INPUT_PULLUP);
    pinMode(SWPin_3, INPUT_PULLUP);
  }

  //for servo
  {
    pinMode(ServoPin, OUTPUT);
    myservo.attach(ServoPin);
    myservo.write(0);
    delay(3000);
    myservo.detach();
  }

  Serial.println("===System Initialize done.===");
  lcd.setCursor(0, 0);
  lcd.print("Meeting Counter Ver1.0.0");
  lcd.setCursor(0, 1);
  lcd.print("Initialize done.");
  delay(1000);
}
//--------------------------------------------------------------------------
//make functions
//--------------------------------------------------------------------------
void BackServo()
{
  myservo.attach(ServoPin);
  myservo.write(0);
  delay(1500);
  myservo.detach();
}
//--------------------------------------------------------------------------
//loop()
//--------------------------------------------------------------------------
// the loop function runs over and over again forever
void loop() {
  int  button1 = digitalRead(SWPin_1);
  int  button2 = digitalRead(SWPin_2);
  int  button3 = digitalRead(SWPin_3);

  if ( button1 == 0 )
  {
    Serial.println("-------------------------------------");
    myservo.attach(ServoPin);
    myservo.write(60);
    delay(500);
    for (int i = 60; i <= 120; i += 2)
    {
      //read length
      float length = sensor.readRangeSingleMillimeters();
      Serial.println(length);

      char s[16];
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(dtostrf(length, 5, 1, s));

      //move position
      myservo.write(i);
      delay(50);
    }
    myservo.detach();
  }
  else if ( button2 == 0 )
  {
    BackServo();
  }
  else if ( button3 == 0 )
  {
    unsigned int length = sensor.readRangeSingleMillimeters() - 10;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(length);
  }
  /*
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(button1);
    lcd.setCursor(2, 1);
    lcd.print(button2);
    lcd.setCursor(4, 1);
    lcd.print(button3);
    delay(10);
     lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(button1);

    //Read Meter
    float length = sensor.readRangeSingleMillimeters();

    //output LCD
    lcd.setCursor(0, 1);
    if (sensor.timeoutOccurred()) {
      lcd.print("TimeOut");
    }
    else
    {
      //success
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      // print the number of seconds since reset:
      //lcd.print(millis() / 10);
      char s[16];
      lcd.print(dtostrf(length, 5, 1, s));
    }
  */
}


