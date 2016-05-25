/*
  Volume.h - Library for tone() with 8-bit volume control.
  Created by Connor Nishijima, May 25th 2016.
  Released under the GPLv3 license.
*/
#ifndef volume_h
#define volume_h

#include "Arduino.h"

class Volume
{
  public:
    Volume(int pin);
	void begin();
	void end();
    void tone(int frequency, byte volume);
    void noTone();
	void delay(unsigned long d);
	void delayMicroseconds(unsigned long du);
	void setMasterVolume(float mv);
  private:
    int _pin;

};

#endif
