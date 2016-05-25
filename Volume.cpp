/*
  Volume.cpp - Library for tone() with 8-bit volume control.
  Created by Connor Nishijima, May 25th 2016.
  Released under the GPLv3 license.
*/

#include "Volume.h"

bool _toneEnable = false;
bool _toneState = false;
byte _toneVol = 0;
int _p;

float _masterVol = 1.00;

Volume::Volume(int pin)
{
  _p = pin;
  pinMode(_p, OUTPUT);
}

void Volume::begin(){
	// TIMER 1 for interrupt frequency 1000 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 1000 Hz increments
  OCR1A = 16000000 / (1 * 10000) - 1;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 1 prescaler
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts
  
  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
}

void Volume::end(){
  noTone();
  TCCR0B = (TCCR0B & 0b11111000) | 0x03;
}

void Volume::tone(int frequency, byte volume)
{
  
  _toneEnable = true;
  long _clk = 16000000 / (1 * frequency*2) - 1;
  if (_clk >= 65536) {
    _clk = 65536 - 1;
  }

  cli(); // stop interrupts
  OCR1A = _clk;
  sei(); // allow interrupts

  _toneVol = volume;
  return;
}

void Volume::noTone()
{
  _toneEnable = false;
  _toneState = false;
  _toneVol = 0;
  return;
}

void Volume::delay(unsigned long d) {
  ::delay(d * 64);
  return;
}

void Volume::delayMicroseconds(unsigned long du) {
  ::delayMicroseconds(du*64);
  return;
}

void Volume::setMasterVolume(float mv) {
  _masterVol = mv;
  return;
}

ISR(TIMER1_COMPA_vect) {
  if (_toneEnable == true) {
    if (_toneState == false) {
      _toneState = !_toneState;
      analogWrite(_p, _toneVol*_masterVol);
    }
    else if (_toneState == true) {
      _toneState = !_toneState;
      analogWrite(_p, 0);
    }
  }
}
