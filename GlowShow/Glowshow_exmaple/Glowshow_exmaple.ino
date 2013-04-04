/*
* An example of the GlowShow library in use with the IRremote library.
*
* James Malone, 2013
* http://www.chimerasaurus.com
*/

// Includes for this sketch
#include <IRremote.h>
#include <Glowshow.h>

// Variables
IRsend irsend;

void setup()
{
  // Nothing to setup
}

// Loop function which runs over and over
void loop() {
  irsend.sendRaw(BOTH_GREEN,16, 38);
  delay(1500);
  irsend.sendRaw(RIGHT_BLUE,18,38);
  delay(2000);
}
