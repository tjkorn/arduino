/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

#define BUTTON_DOWN_LEVEL LOW
#define BUTTON_UP_LEVEL   HIGH
#define LED_ON_LEVEL      LOW
#define LED_OFF_LEVEL     HIGH

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

bool isLedOn = false;
bool isButtonDown = false;
void loop() {
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
  /*
  else 
  {
    // turn LED off:
    isLedOn = false;
    digitalWrite(ledPin, HIGH);
  }
  */
}
