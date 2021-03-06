#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include "Arduino.h"

#define BUTTON_DOWN_LEVEL LOW
#define BUTTON_UP_LEVEL   HIGH
#define LED_ON_LEVEL      LOW
#define LED_OFF_LEVEL     HIGH

// set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

bool isLedOn = false;
bool isButtonDown = false;



//setup lcd
#if defined(ARDUINO_ARCH_SAMD) || defined(__SAM3X8E__)
  // use pin 18 with Due, pin 1 with Zero or M0 Pro 
  #define lcd Serial1 
#else
  #include <SoftwareSerial.h>
  // Create a software serial port!
  SoftwareSerial lcd = SoftwareSerial(0,2);
#endif

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
}

void lcdClear(void)
{
  // clear screen
  lcd.write(0xFE);
  lcd.write(0x58);
  delay(10);   // we suggest putting delays after each command
}

void lcdGoHome(void)
{
  // go 'home'
  lcd.write(0xFE);
  lcd.write(0x48);
  delay(10);   // we suggest putting delays after each command   
}

void initLcd(void)
{
  lcd.begin(9600);  
  
  // set the size of the display if it isn't 16x2 (you only have to do this once)
  lcd.write(0xFE);
  lcd.write(0xD1);
  lcd.write(16);  // 16 columns
  lcd.write(2);   // 2 rows
  delay(10);       
  // we suggest putting delays after each command to make sure the data 
  // is sent and the LCD is updated.

  // set the contrast, 200 is a good place to start, adjust as desired
  lcd.write(0xFE);
  lcd.write(0x50);
  lcd.write(200);
  delay(10);       
  
  // set the brightness - we'll max it (255 is max brightness)
  lcd.write(0xFE);
  lcd.write(0x99);
  lcd.write(255);
  delay(10);       
  
  // turn off cursors
  lcd.write(0xFE);
  lcd.write(0x4B);
  lcd.write(0xFE);
  lcd.write(0x54);

  lcdClear();
  lcdGoHome();
}
void setup() 
{

 // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
 
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Adafruit 10 DOF Pitch/Roll/Heading Example")); Serial.println("");

  //initialize LCD
  initLcd();
  lcd.print("start");
  /* Initialise the sensors */
  initSensors();

}

String toString(float f, int prec)
{
  String str = String(f, prec);

  if(str[0] != '-')
    str = String("+") + str;

  return str;
}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
uint32_t loopIteration = 0;
char buf[16] = "";
float temperature;
void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:

  if(isButtonDown)
  {    
    if(buttonState == BUTTON_UP_LEVEL)
    {
      isButtonDown = false;
    }
  }
  else if (buttonState == BUTTON_DOWN_LEVEL) 
  {
    // turn LED on:
    isButtonDown = true;
    isLedOn = !isLedOn;
    if(isLedOn)
    {
      digitalWrite(ledPin, LED_ON_LEVEL);
    }
    else
    {
      digitalWrite(ledPin, LED_OFF_LEVEL);
      }
  }
  lcdClear();
  lcdGoHome();
  //lcd.print("n=");
  //lcd.print(loopIteration++);

  /* Read the accelerometer and magnetometer */
  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);

  /* Use the new fusionGetOrientation function to merge accel/mag data */  
  if (dof.fusionGetOrientation(&accel_event, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    Serial.print(F("Orientation: "));
    Serial.print(orientation.roll);
    Serial.print(F(" "));
    Serial.print(orientation.pitch);
    Serial.print(F(" "));
    Serial.print(orientation.heading);
    Serial.println(F(""));
        /* 'orientation' should have valid .roll and .pitch fields */
    //lcd.print(orientation.roll); 
    //sprintf(buf, "%f %f %f", orientation.roll, orientation.pitch, orientation.heading);
    //Serial.println(buf);
    //lcd.println(buf);
    //lcd.print(" ");
    //lcd.print(orientation.pitch);
    //lcd.print(" ");
    //lcd.println(orientation.heading);
    //lcd.println(F(""));

  }

  /* Previous code removed handling accel and mag data separately */
  //  /* Calculate pitch and roll from the raw accelerometer data */
  //  Serial.print(F("Orientation: "));
  //  accel.getEvent(&accel_event);
  //  if (dof.accelGetOrientation(&accel_event, &orientation))
  //  {
  //    /* 'orientation' should have valid .roll and .pitch fields */
  //    Serial.print(orientation.roll);
  //    Serial.print(F(" "));
  //    Serial.print(orientation.pitch);
  //    Serial.print(F(" "));
  //  }
  //  
  //  /* Calculate the heading using the magnetometer */
  //  mag.getEvent(&mag_event);
  //  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  //  {
  //    /* 'orientation' should have valid .heading data now */
  //    Serial.print(orientation.heading);
  //  }
  //  Serial.println(F(""));

  /* Calculate the altitude using the barometric pressure sensor */
  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */    
    bmp.getTemperature(&temperature);
    /* Convert atmospheric pressure, SLP and temp to altitude */
    Serial.print(F("Alt: "));
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature)); 
    Serial.println(F(""));
    /* Display the temperature */
    Serial.print(F("Temp: "));
    Serial.print(temperature*9/5+32);
    Serial.println(F(""));
  }

  if(isLedOn)
  {
    String headline = String("ROLL PITCH HEAD");
    String line1 = toString(orientation.roll,0) + " " + toString(orientation.pitch,0) + "  " + toString(orientation.heading,0);
    lcd.println(headline);
    lcd.print(line1);
  }
  else
  {
    String headline = String("TEMP");
    String line1 =  toString(temperature,1) + " C/" + toString(temperature*9/5+32,1) + " F";
    lcd.println(headline);
    lcd.print(line1);
  }
  
  delay(100);
}
