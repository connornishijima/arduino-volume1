#Arduino Volume Library
**Arduino tone() just got 8-bit volume control!***

**with no extra components!*

Ever needed a project to play a tone through a speaker or piezo that *wasn't* blisteringly loud? You can bring the volume down with a resistor, but what if you needed a loud alert beep at some point as well?

**Now it's no longer an issue.** :) Using ultra-fast PWM, the Volume library allows the speaker itself to act as an RC filter to produce *smooth* (8-bit) analog-like volume control.

----------
# Contents
- [Installation](#installation)
- [Usage](#usage)
- [Functions](#functions)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

----------
# Installation

1. Click "Clone or Download" above to get an "arduino-volume-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "arduino-volume-master" to "Volume".

----------
# Usage

Using the volume-controlled `vol.tone()` function looks very similar to the Arduino `tone()`, but the function arguments are very different:

**Arduino:**
 - **tone**(unsigned int **pin**, unsigned int **frequency**);

**Volume:**
 - vol.**tone**(unsigned int **frequency**, byte **volume**);

Volume control is limited to pins 5 & 6. See [Limitations](#limitations).

----------
Here is what you need to get started with the bare minimum:

    #include "Volume.h" // Include the Volume library
    
    const int speakerPin = 5; // Only pins 5 or 6 may be used
    Volume vol(speakerPin);   
    
    void setup(){
	    vol.begin();
	}
	void loop(){
		vol.tone(1000,255); // 100% volume
		vol.delay(500);
		vol.tone(1000,192); // 75% volume
		vol.delay(500);
		vol.tone(1000,127); // 50% volume
		vol.delay(500);
		vol.tone(1000,64); // 25% volume
		vol.delay(500);
		vol.tone(1000,12); // 5% volume
		vol.delay(500);
	}

Of course, you can set the volume to any value between 0 and 255 you'd like, for full 8-bit volume fades.

----------
# Functions

**Volume vol**(unsigned int **speakerPin**);

This initializes the Volume library after import. "vol" can be any word you want, as long as it's reflected in the rest of your code.

**vol.begin**();

This sets up a Timer Compare Interrupt on Timer1 for the tone frequencies. (You won't hear anything until a `vol.tone()` is called.)

**vol.setMasterVolume**(float **percentage**);

This is a multiplier applied to the volume of any tones played. By default this is 1.00 - a value of 0.34 would make all tones 34% of their programmed volume;

**vol.tone**(unsigned int **frequency**, byte **volume**);

*This is where the magic happens.* At the frequency you specify, your Arduino will analogWrite(**volume**) to the speaker with a PWM frequency of 62.5KHz, for half the duration of a single period of the **frequency** before pulling it `LOW`. (Using Timer1 compare-match interrupts to maintain the input frequency) This high-speed PWM is beyond your range of hearing, (and probably the functioning range of your speaker) so it will just sound like a quieter or louder version of the input frequency!

**vol.noTone**();

This is identical in function to the standard `noTone()` function, this stops any currently playing tones.

**vol.delay**(); & **vol.delayMicroseconds**();

These are replacements to the standard delay() and delayMicroseconds() Arduino functions designed to compensate for the changes in the Timer0 clock divisor. See [Limitations](#limitations).

**vol.end**();

This stops any currently playing tones, and resets Timer0 to it's default functionality. Creative use of `vol.begin()` and `vol.end()` can usually resolve conflicts with other libraries or functions that might need Timer0 (volume) or Timer1 (frequency) to be in their usual settings.

----------
# Limitations
Unfortunately, cheating the Arduino's normal functions in this way means we'll lose some of them. This is also still a proof-of-concept library at this point, so it may break more functionality than I'm aware of. Sorry!

**16MHz Only:**

I haven't programmed in options for 8MHz boards yet, though if you want to use one, just replace all occurrences of "16000000" in the library files with "8000000" and it may work.

**ATmega* Only:**

I don't know if I'll have this working on ATTiny*5 boards any time soon, though it's theoretically possible on any AVR with >= 2 timers. For now, it's only confirmed working on Arduino Uno (ATMega168/328) and Mega. (ATMega1280/2560)

**Volume is limited to pins 5 & 6:**

This is because only pins 5 & 6 are driven by Timer0, *which can do PWM at a frequency higher than your hearing range!* This is the main trick behind the volume function. It also means that while you're using Volume, normal `analogWrite()` use probably won't work on these two pins.

**Volume alters Timer0 for 62.5KHz PWM:**

Speaking of Timer0 - it's normally used for the `delay()`, `delayMicroseconds()`, `millis()` and `micros()` functions. Normally with Timer0 set with a divisor of 64, `delay(1000)` would wait for 1 second - but because Volume sets Timer0 with a divisor of 1, `delay(1000)` will now only wait for 15.625ms! But don't worry. Volume provides alternative `vol.delay(time)` and `vol.delayMicroseconds(time)` functions with the math fixed for you. This new divisor is necessary to drive PWM on pins 5 & 6 at 62.5KHz, faster than you can hear.

**Volume does not yet offer fixed millis() or micros() functions:**

I haven't gotten around to toying with these yet. If you need to use `millis()` or `micros()` BETWEEN playing sounds, just use a `vol.end()` to reset Timer0 to it's default function, and `vol.begin()` to use it for Volume again after you're done.

----------
# Contributing
As I've only written one library before for my own use, I'm still new to this! Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2016)**

**Special Thanks to Andrew Neal** (For putting up with the incessant and inconsistent artificial "cricket-duino" hidden in his vent that I developed this library for.)

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
