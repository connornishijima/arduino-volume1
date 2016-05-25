arduino-volume
===========================================
**Arduino tone() now has 8-bit volume control!**

Ever needed a project to play a tone through a speaker or piezo that *wasn't* blisteringly loud? You can bring the volume down with a resistor, but what if you needed a loud alert beep at some point as well?

**Now it's no longer an issue.** :) Using ultra-fast PWM, the Volume library allows the speaker itself to act as an RC filter to produce *smooth* (8-bit) analog-like volume control.

# Contents
- [Installation](#installation)
- [Usage](#usage)
- [Issues and support](#issues-and-support)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

### Installation

1. Click "Clone or Download" above to get an "arduino-volume-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "arduino-volume-master" to "Volume".

### Usage

Before we get into how to use the Volume library, let's quickly talk about it's limitations:

**Volume is limited to pins 5 & 6:**
This is because only pins 5 & 6 are driven by Timer0, *which can do PWM at a frequency higher than your hearing range!*

**Volume alters Timer0 for 62.5KHz PWM:**
Speaking of Timer0 it's normally used for the `delay()`, `delayMicroseconds()`, `millis()` and `micros()` functions. Normally with Timer0 set with a divisor of 64, `delay(1000)` would wait for 1 second - but because Volume sets Timer0 with a divisor of 1, `delay(1000)` will now only wait for 15.625ms! But don't worry. Volume provides alternative `vol.delay(time)` and `vol.delayMicroseconds(time)` functions with the math fixed for you. This new divisor is necessary to drive PWM on pins 5 & 6 at 62.5KHz, faster than you can hear.

**Volume does not yet offer fixed millis() or micros() functions:**
I haven't gotten around to toying with these yet. If you need to use `millis()` or `micros()` BETWEEN playing sounds, just use a `vol.end()` to reset Timer0 to it's default function, and `vol.begin()` to use it for Volume.
