//--------------------------------------------------------------------------
//MeetingCostMeter
//--------------------------------------------------------------------------
// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
//using VL53L0X library for Arduino(https://github.com/pololu/vl53l0x-arduino)
#include "VL53L0X.h"
//#include <VL53L0X.h>

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

//コメント
#define VER_COMENT "MeetingCostMeter V1.0.0"

//--------------------------------------------------------------------------
//setup()
//--------------------------------------------------------------------------
void InitVL53L0X()
{
  //ToF init
  sensor.init();
  sensor.setTimeout(300); //unit is ms

  //long range
  {
    /*
      // lower the return signal rate limit (default is 0.25 MCPS)
      sensor.setSignalRateLimit(0.1);	//0.1
      // increase laser pulse periods (defaults are 14 and 10 PCLKs)
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);	//18
      sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);	//14
    */
    //high accuracy
    //sensor.setMeasurementTimingBudget(200000);	//default 33000 = 33ms
    //sensor.setMeasurementTimingBudget(40000);		//default 33000 = 33ms
    //sensor.setMeasurementTimingBudget(60000);	//20[ms] + 3.5[ms](処理にかかる時間)
  }
  //sensor.startContinuous();
  delay(100);
}

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
    delay(100);
  }

  //init serial
  {
    Serial.begin(9600);
    Serial.println("===System Initialize...===");
    delay(100);
  }

  //init I2C
  {
    Wire.begin();
    InitVL53L0X();
  }

  //init sw(3,4,5)
  {
    pinMode(SWPin_1, INPUT_PULLUP);
    pinMode(SWPin_2, INPUT_PULLUP);
    pinMode(SWPin_3, INPUT_PULLUP);
    delay(100);
  }

  Serial.println("===System Initialize done.===");
  lcd.setCursor(0, 0);
  lcd.print(VER_COMENT);
  lcd.setCursor(0, 1);
  lcd.print("Initialize done.");
  delay(100);
}

//--------------------------------------------------------------------------
//functions and variables
//--------------------------------------------------------------------------
//settings for loop
#define	ANGLE_START			20
#define	ANGLE_END			175
#define	ANGLE_PERIOD		(ANGLE_END-ANGLE_START)
#define	ANGLE_RESOLUTION	40	//分解能これで確保するサイズ（ALLOCATE）が動的に変わる
#define ALLOCATE			int(ANGLE_PERIOD / int(ANGLE_PERIOD/ANGLE_RESOLUTION))
#define AVERAGING			2	//計測を何回行うか
#define MOVINGAVG			3	//移動平均

//variable for loop
bool			oneShot	= true;
unsigned int	angle	= 0;
unsigned int	data[ALLOCATE] = {0};

void SetServo(unsigned int angle, unsigned int delaytime)
{
  myservo.attach(ServoPin);
  delay(5);
  myservo.write(angle);
  delay(delaytime);
  myservo.detach();
  delay(5);
}

void debugAngle(int button1, int button2, int button3)
{
  if ( button1 == 0 )
  {
    angle++;
  }
  else if ( button2 == 0 )
  {
    angle--;
  }
  else if ( button3 == 0 )
  {
    SetServo(angle, 300);
  }

  //scan
  unsigned int readData = sensor.readRangeSingleMillimeters();

  //output lcd
  char msg[17] = {0};
  lcd.setCursor(0, 0);
  sprintf(msg, "%6d", angle);
  lcd.print(msg);
  lcd.setCursor(0, 1);
  sprintf(msg, "%6d[mm]", readData);
  lcd.print(msg);

  delay(20);
}

unsigned int ReadData(int avg)
{
  unsigned int readData = 0;
  for (int i = 0; i < avg; i++)
  {
    delay(1);
    readData +=  sensor.readRangeSingleMillimeters();
    delay(1);
    if (sensor.timeoutOccurred())
    {
      Serial.println("TIMEOUT");
      return 0;
    }
  }
  unsigned int temp = readData / avg;
  return temp;
}

//--------------------------------------------------------------------------
//loop()
//--------------------------------------------------------------------------
int state = 0;
void loop() {
  //read button
  int  button1 = digitalRead(SWPin_1);
  int  button2 = digitalRead(SWPin_2);
  int  button3 = digitalRead(SWPin_3);

  //init
  if ( oneShot == true)
  {
    lcd.clear();
    oneShot = false;

    lcd.setCursor(0, 0);
    lcd.print("Push Retd button.");
  }

  if ( button1 == 0 )
  {
    //--------------------------------------
    //Scanモード
    //--------------------------------------
    InitVL53L0X();

    //reset servo angle
    SetServo(ANGLE_START, 500);
    delay(500);

    //output lcd
    char msg[20] = {0};
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan...");

    /*
      myservo.attach(ServoPin);
      {
      //scan
      unsigned int per = (unsigned int)(ANGLE_PERIOD / ANGLE_RESOLUTION);
      for ( int i = 0; i < ALLOCATE; i++)
      {
        //read
        int tempLength = ReadData(AVERAGING);
        if (tempLength == 0)
        {
          //error
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ERROR");
          lcd.setCursor(0, 1);
          lcd.print("Push Retd button.");
          return;
        }
        else
        {
          data[i] = tempLength;
        }

        //check out of range
        if (data[i] > 3000)
        {
          if (i > 0 && i < (ALLOCATE - 1))
          {
            data[i] = data[i - 1];
          }
          else
          {
            data[i] = 30;
          }
        }

        //move
        unsigned int tempAngle = i * per + ANGLE_START;
        lcd.setCursor(0, 1);
        sprintf(msg, "Angle:%3d L:%4d", tempAngle, data[i]);
        lcd.print(msg);
        myservo.write(tempAngle);
      }
      }
      myservo.detach();
    */

    //scan
    unsigned int per = (unsigned int)(ANGLE_PERIOD / ANGLE_RESOLUTION);
    for ( int i = 0; i < ALLOCATE; i++)
    {
      //read
      int tempLength = ReadData(AVERAGING);
      if (tempLength == 0)
      {
        //error
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ERROR");
        lcd.setCursor(0, 1);
        lcd.print("Push Retd button.");
        return;
      }
      else
      {
        data[i] = tempLength;
      }

      //check out of range
      if (data[i] > 3000)
      {
        if (i > 0 && i < (ALLOCATE - 1))
        {
          data[i] = data[i - 1];
        }
        else
        {
          data[i] = 30;
        }
      }

      //move
      unsigned int tempAngle = i * per + ANGLE_START;
      lcd.setCursor(0, 1);
      sprintf(msg, "Angle:%3d L:%4d", tempAngle, data[i]);
      lcd.print(msg);
      SetServo(tempAngle, 75);
    }

    //output
    unsigned int lengthMax = 0;
    unsigned long sum = 0;
    unsigned int avg = 0;
    for (int i = 0; i < ALLOCATE; i++)
    {
      if (data[i] > 3000)
      {
        if (i > 0 && i < (ALLOCATE - 1))
        {
          data[i] = data[i - 1];
        }
        else
        {
          data[i] = 30;
        }
      }
      //max
      if (i == 0)
      {
        lengthMax = data[i];
      }
      else if (lengthMax < data[i])
      {
        lengthMax = data[i];
      }
      //Serial.println(data[i]);
      sum += data[i];
    }
    avg = sum / ALLOCATE;	//平均値

    //signal processing
    int sCount = 0;
    for (int i = 0; i < ALLOCATE; i++)
    {
      if (i == (ALLOCATE - MOVINGAVG + 1))
      {
        break;
      }
      unsigned int tempVal;
      for (int j = 0; j < MOVINGAVG; j++)
      {
        tempVal += data[i + j];
      }
      tempVal = tempVal / MOVINGAVG;
      Serial.println(tempVal);
      sCount++;
    }

    //debug
    sprintf(msg, "Allocate:%d, SCount:%d", ALLOCATE, sCount);
    Serial.println(msg);

    //状態を次へ
    state = 1;

    Serial.println("--- END ---");
  }

  if ( state == 1 )
  {
  }
}


