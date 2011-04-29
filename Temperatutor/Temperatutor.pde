/*
* Temperatutor: LED adjustable thermometer
 *
 * Purpose:
 * Read the temperature and display the tempeature via two LEDs
 *
 * Features:
 * Adjustable "ranges" and sensitivity via two potentiometers
 * 
 * Operating range:
 * Thermometer works from -13 F to 185F (don't be dumb)
 * 
 * Potentiometer operation:
 * - Potentiometer 1: Adjusts the "spread" (range) of temperatures to measure given the hardware's design range
 * --> EG: A settinng of 50% will narrow the response of the LEDs to 50% of the hardware's range (.50*-13 && .50*185)
 * - Potentiometer 2: Adjusts the low and high bounds of the range set by P1
 * --> EG: A setting of 0% moves the lower bound to -13F, a setting of 100%% sets the low bound to (185 - the spread range)
 *
 * Notes:
 * This is my first Arduino sketch.
 *
 */

//Includes
#include <Wire.h> // Needed to read the temperature

//Constants for components
#define POTENTIOMETER1 0
#define POTENTIOMETER2 1
#define BLUELED 10
#define REDLED 11

//Constants
const int highTempVal = 185; //Temperature sensor H/W sensor max
const int lowTempVal = -13; //Temperature sensor H/W sensor low
const boolean debug = true; //Turns debugging on and off
const int tmp102Address = 0x48; //Temperature sensor address

//Veriables
int blueColor = 0; // Color for blue LED
int redColor = 0; //Color for red LDED
int pot1Val = 0; //potentiometer 1 value
int pot2Val = 0; //potentiometer 2 value


/* Setup function */
void setup()
{
  //Set the LEDs to output
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);

  //Begin wire comm. to read temp
  Wire.begin();

  //Begin the serial connection if debugging is enabled
  if (debug)
  {
    Serial.begin(9600);
  }
}

/* Loop function */
void loop()
{
  Serial.println(" ");

  //Read the potentiometers
  pot1Val = analogRead(POTENTIOMETER1);
  pot2Val = analogRead(POTENTIOMETER2);

  //Calculate the "temperature spread" to measure
  int defaultTempSpread = (highTempVal + abs(lowTempVal));
  float spreadBias = map(pot1Val, 0, 1023, 0, 100);
  float calculatedSpread = ((spreadBias/100) * defaultTempSpread);

  //Calculate temperature bias
  int defaultTempMean = ((highTempVal + lowTempVal) / 2);
  float temperatureMeanBias = map(pot2Val, 0, 1023, 0, 100);

  //Calculate the new temperature range based on pot vals
  float navRoom = (defaultTempSpread - calculatedSpread);
  float lowBound = (navRoom*(temperatureMeanBias/100) + lowTempVal);
  float highBound = (lowBound + calculatedSpread);

  //Read the temperature
  Wire.requestFrom(tmp102Address,2); 
  byte MSB = Wire.receive();
  byte LSB = Wire.receive();

  //Calculate the temperature
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative
  float fahrenheit = (TemperatureSum*0.1125) + 32;  

  //Calculate PWM values for LEDs
  int blueVal = constrain(map(fahrenheit, int(lowBound), int(highBound), 255, 0), 0, 250);
  int redVal = constrain(map(fahrenheit, int(lowBound), int(highBound), 0,255), 0, 250);

  /*Set the LED color based on temp*/
  //FLASH LEDs if the temp is out of bounds
  if ((fahrenheit < lowBound) || (fahrenheit > highBound))
  {
    for (int i=0; i <3; i++)
    {
      analogWrite(BLUELED, 250);
      analogWrite(REDLED, 0);
      delay(100);
      analogWrite(BLUELED, 00);
      analogWrite(REDLED, 250);
      delay(100);
    } 
  } 
  else
  {
    //Temp is not out of bounds, show color calc
    analogWrite(BLUELED, blueVal);
    analogWrite(REDLED, redVal);
  }

  //If debugging is turned on, show values
  if (debug)
  {
    Serial.println("--Potentiometer calculations--");
    Serial.print("Spread bias: ");
    Serial.println(spreadBias);
    Serial.print("Calculated spread: ");
    Serial.println(calculatedSpread);
    Serial.print("Temperature mean bias:");
    Serial.println(temperatureMeanBias);
    Serial.print("Spread movement room:");
    Serial.println(navRoom);
    Serial.print("Spread low bound:");
    Serial.println(lowBound);
    Serial.print("Spread high bound:");
    Serial.println(highBound);
    Serial.println("--Temperature calculations--");
    Serial.print("Spread bias: ");
    Serial.println(spreadBias);
    Serial.print("Calculated spread: ");
    Serial.println(calculatedSpread);
    Serial.print("Temperature mean bias:");
    Serial.println(temperatureMeanBias);
    Serial.print("Fahrenheit: ");
    Serial.println(fahrenheit);
    Serial.println("--LED PWN PWM values--");
    Serial.print("Blue: ");
    Serial.println(blueVal);
    Serial.print("Red: ");
    Serial.println(redVal);
  }

  //Delay 1S until next loop
  delay(1000);
}










