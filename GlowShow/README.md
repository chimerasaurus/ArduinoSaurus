#GlowShow#

Arduino library for communicating with fashionable headwear.

##Description##
This is an Arduino library for communicating, via infrared, with some fashionable headwear. This library implements the timing needed to change the state of said headwear.

##Contents##
This package contains two components.

###Glowshow library###
`Glowshow.h` - Library with data needed to change the colors of the headwear. The data are IR pulse timings for use with the `sendRaw` method in the `IRremote` library.

This library should be useful with any `C` project which uses the `IRremote` library or something similar.

###Glowshow Arduino example###
`Glowshow example` - A sample Arduino sketch showing how to use the `Glowshow.h` library.

##License##
See the included **LICENSE** file.

##Legal disclaimer##
All intellectual property, copyrights, and other assets are property of their respective and legal owners.