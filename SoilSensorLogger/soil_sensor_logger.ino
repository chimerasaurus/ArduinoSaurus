/* Sketch to prototype my plant moisture sensors.
 *  
 *  Will monitor two sensors and log the results to an SD card.
 *  Lots of stuff in here is brittle. Oops.
 *  
 *  Based on: 
 *  https://learn.adafruit.com/adafruit-data-logger-shield/
 *  https://learn.sparkfun.com/tutorials/soil-moisture-sensor-hookup-guide
 *  
 *  James - 2016/12/28
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


/* Definitions for this sketch */
RTC_DS1307 RTC; // define the Real Time Clock object
const int chipSelect = 10; // for the data logging shield, we use digital pin 10 for the SD cs line
File logfile; // File to log into on the SD card
int thresholdUp = 400; // Max "plant is watered OK" threhold
int thresholdDown = 250; // Min "plant needs water" threhold
int sensorPinOne = A0; // First moisture sensor
int sensorPinTwo = A1; // Second moisture sensor
int powerPin = 7; // Digital pin used to provide 5V power
int waterSensorValues[2]; // Array to hold sensor values
int plantLed = 8; // Reporting LED to show plants need water

/* Setup */
void setup(){
  Serial.begin(9600); // set up serial port for 9600 baud (speed)
  // Set the pins used for this sketch
  pinMode(powerPin, OUTPUT); 
  pinMode(plantLed, OUTPUT);
  pinMode(10, OUTPUT); // make sure that the default chip select pin is set to output, even if you don't use it:
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  
  // Create a new log file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    Serial.println("couldnt create file");
    return;
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  // Begin the RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
    Serial.println("RTC failed");
  }

  // Print the headers
  logfile.println("millis,time,sensor1,sensor2");    
  Serial.println("millis,time,sensor1,sensor2");
}

/* Sketch main loop */
void loop(){
  /* All of this below logs the time so the logfile has timestamps */
  DateTime now; // Used to log timestamps
  uint32_t m = millis();
  logfile.print(m);           // milliseconds since start
  logfile.print(", ");    
  Serial.print(m);         // milliseconds since start
  Serial.print(", ");
    
  // fetch the time
  now = RTC.now();
  // log time
  logfile.print(now.unixtime()); // seconds since 1/1/1970
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');
  Serial.print(now.unixtime()); // seconds since 1/1/1970
  Serial.print(", ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
  
  String wateringSuggestion; // Suggestion to water plants (or not)

  // Read the plant moisture sensors
  int sensorValueOne;
  int sensorValueTwo;
  // We only turn on the power to the sensors when we need to and then turn it back off
  // It is my understanding this will slow corrosion of the sensors in moist soils
  digitalWrite(powerPin, HIGH);
  sensorValueOne = analogRead(sensorPinOne);
  waterSensorValues[0] = sensorValueOne;
  sensorValueTwo = analogRead(sensorPinTwo);
  waterSensorValues[1] = sensorValueTwo;
  digitalWrite(powerPin, LOW);

  // Log the values
  logfile.print(", ");    
  logfile.print(sensorValueOne);
  logfile.print(", ");    
  logfile.println(sensorValueTwo);

  // Print values to the console
  Serial.print("Water Level (1): ");
  Serial.println(sensorValueOne); //Using .print instead of .write for values

  Serial.print("Water Level (2): ");
  Serial.println(sensorValueTwo); //Using .print instead of .write for values

  // Loop through the sensor values and determine the watering suggestion
  // and blink an LED if the particular plant needs water
  for(int i = 0; i <  2; i++) {
    if (waterSensorValues[i] <= thresholdDown) {
      wateringSuggestion = "Plant " + String(i + 1) + " is dry, water it!";
      for (int x = 0; x <= i; x++) {
       digitalWrite(plantLed, HIGH);
       delay(500);
       digitalWrite(plantLed, LOW);
       delay(500);
      }
      delay(1000);
      digitalWrite(plantLed, HIGH);
      delay(1000);
      digitalWrite(plantLed, LOW);
      delay(1000);
    } else if (waterSensorValues[i]  >= thresholdUp) {
      wateringSuggestion = "Plant " + String(i + 1) + " is wet, leave it!";
    }

    // Print the suggestion to the console
    Serial.println(wateringSuggestion);
  }
  
  delay(10000); // Wait 10 seconds and repeat
}

