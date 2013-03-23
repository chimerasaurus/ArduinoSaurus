
/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */
#include <IRremote.h>

IRsend irsend;
const int buttonPin = 12; 
int buttonState = 0; 
unsigned int BOTH_YELLOW[14] = {2085,417,834,834,2502,834,417,417,834,834,834,417,417,834};
//unsigned int LEFT_YELLOW[32 = {2085,417,834,834,834,834,834,834,417,417,417,834,417,1668,417,417,2085,-417,834,-834,1668,-417,417,-834,417,-417,1251,-417,834,-834,417,-417}
unsigned int LEFT_CYAN[32] = {2085,417,834,834,417,834,1251,834,417,417,1251,417,1251,417,417,417,2085,417,834,834,1668,417,417,834,417,417,1251,417,834,834,417,417};
unsigned int RIGHT_INACTIVE[14] = {};
void setup()
{
    pinMode(buttonPin, INPUT); 
}

void loop() {
    irsend.sendRaw(BOTH_YELLOW,14, 38);
    //irsend.sendRaw(RIGHT_INACTIVE,14, 38);
      delay(100000);

}

