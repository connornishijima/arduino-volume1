/*
  Volume.cpp - Library for tone() with 8-bit volume control.
  Created by Connor Nishijima, May 25th 2016.
  Updated June 14, 2016.
  Released under the GPLv3 license.
*/

#include "Volume.h"

bool _toneEnable = false;
bool _toneState = false;
byte _toneVol = 0;

unsigned int _freq = 0;

float _masterVol = 1.00;

bool _fadeOut = false;
float _fadeVol = 1.00;
float _fadeAmount = 0.01;
byte _fadeCounter = 0;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
byte _SPEAKER = 4;
#endif

#if defined(__AVR_ATmega32U4__)
byte _SPEAKER = 9;
#endif

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
byte _SPEAKER = 5;
#endif

void Volume::alternatePin(bool enabled) {
  if (enabled == true) {
	  
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    _SPEAKER = 13;
#endif

#if defined(__AVR_ATmega32U4__)
    _SPEAKER = 10;
#endif

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
    _SPEAKER = 6;
#endif

  }
}

Volume::Volume()
{
}

void Volume::throwError(byte _errorNum) {
  end();
  while (true) {
    byte _beeps = _errorNum;
    while (_beeps > 0) {
      ::tone(_SPEAKER, 500, 100);
      ::delay(200);
      _beeps--;
    }
    ::delay(1000);
  }
}

void Volume::begin() {
  pinMode(_SPEAKER, OUTPUT);

  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 10000 Hz increments
  OCR1A = F_CPU / (2 * 1 * 10000) - 1;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 1 prescaler
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
}

void Volume::end() {
  noTone();
  TCCR0B = (TCCR0B & 0b11111000) | 0x03;
}

void Volume::fadeOut(int duration){
  _fadeOut = true;
  _fadeAmount = (1/float(_freq*2*(duration/1000.0)))*10;
  _fadeCounter = 0;
}

void Volume::tone(int frequency, byte volume)
{
  _freq = frequency;
  _fadeOut = false;
  _fadeVol = 1.00;
  _toneEnable = true;
  long _clk = F_CPU / (1 * frequency * 2) - 1;
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

unsigned long Volume::millis() {
  return ::millis() / 64;
}

unsigned long Volume::micros() {
  return ::micros() / 64;
}

void Volume::delayMicroseconds(unsigned long du) {
  ::delayMicroseconds(du * 64);
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
      analogWrite(_SPEAKER, _toneVol * _masterVol * _fadeVol);
    }
    else if (_toneState == true) {
      _toneState = !_toneState;
      analogWrite(_SPEAKER, 0);
    }
	if(_fadeOut == true){
		_fadeCounter++;
		if(_fadeCounter >= 10){
			_fadeCounter = 0;
			if(_fadeVol > 0){
				_fadeVol-=_fadeAmount;
				if(_fadeVol < 0){
					_fadeVol = 0;
				}
			}
			else{
				_fadeOut = false;
				_toneEnable = false;
				_toneState = false;
				_toneVol = 0;
			}
		}
	}
  }
}
