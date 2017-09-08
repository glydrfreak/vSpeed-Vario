![alt tag](https://github.com/glydrfreak/vSpeedVario/blob/master/logo.jpg)

# v^SPEED VARIO
Self-Built Paragliding Variometer / Vertical Speed Indicator. <br />
(...it goes beep beep while climbing in a thermal...) <br /> 

## PROTOTYPE3: <br />
![alt tag](https://github.com/glydrfreak/vSpeedVario/blob/master/SolidWorks/P2_BLACK.JPG)

### Features: <br />
-Adafruit Feather Bluefruit M0, <br />
-MS5611 Barometric Pressure/Temperature Sensor (SPI mode), <br />
-Less than several INCHES of altitude noise with a simple averaging filter, <br />
-20 Altitude samples per second with the display screen ON, <br />
-50 Altitude samples per second while connected to your phone with the display screen OFF, <br />
-0.66" OLED display (SPI mode), <br />
-PS1740 Piezo Buzzer, <br />
-3D Printed Case, <br />
-Custom designed PCB, <br />
-Bluetooth compatible with Android & iOS, <br />
-Several widgets and Audio Beeps are also implemented in the app. <br />
Material cost for ten: as low as $45 each, <br />
Material cost for one: as low as $77 each, <br />
Purchase one from me: as low as $70 each. <br />

### Footage: <br />
Introduction: https://www.youtube.com/watch?v=gFNgn0X94sw <br />
In-Flight Test: https://www.youtube.com/watch?v=RXyAx8dpHaY <br />

The only footage I have for the app is during an Imagine Dragons concert in a low averaging setting--  <br />
Android App: https://www.youtube.com/watch?v=2kcBOBjCzPE&t=4s <br />
(At about 1:20 the heavy base ramps up during the song and makes the barometric sensor data go nuts!) <br />

## Buy one for $70 -- OR -- Make your own for $77
If you want a PROTOTYPE3, email me at vSpeedVario@gmail.com ... I can buy enough materials for ten variometers so the material cost for each variometer can be as low as $45, or you can make your own single variometer for as low as $77. I'm offering a very very small price mark up until I sell at least ten variometers. (I'm hoping to never raise the price above $70 anyway.)


#### This is an open-source project 
...found on GitHub ( https://github.com/glydrfreak )
Be careful not to get too involved, because it may just eat away your whole entire Summer, prohibit you from sleeping all night, and even may cause you to actually study everything about electronics. More extreme side effects may include forgetting to eat, resulting in very serious hunger pains. 

##### Android App--
I have modified Adafruit's "Bluefruit LE Connect" for Android only so far, and the source code can be found in the Android folder. Just plop it into Android Studio and upload it to your Android device. I have not published anything to Google Play yet, and I am still learning iOS. Stay tuned for the iOS version of the app. More info about the Android app will also be posted soon.
Adafruit's "Bluefruit LE Connect" (without my modifications) is available to download on your mobile device from Google Play and the App Store, and will work in UART mode, via sending characters between the Feather Bluefruit M0 and your mobile device.

##### 3D Printing--
I'm still trying to nail down the type of 3D printing I should use for production casings. Obviously I cannot afford injection molding at the moment, but a 3D printed case can range from $4 - $15, and it all depends on the desired quality. The cheapest option I found is $4 for a 200micron filament PLA from 3Dhubs.com. I'm still waiting to see how PROTOTYPE3 will look with such large layer thickness. On the other hand, PROTOTYPE2's casing was about $13 from ShapeWays.com. I chose the cheapest option, PA 2200 which is a strong and flexible nylon plastic powder. No print layers are visible on PROTOTYPE2.

##### Arduino IDE--
Arduino IDE is the free software and programming language for this device. Paul McWhorter got me hooked on his YouTube video series of lessons for learning Arduino. Before the summer of 2017, I had no interest in electronics, until I realized you can do almost anything when you combine it with computer programming. I ended up pulling an all nighter as I watched every single one of those videos, and suddenly I knew how electronics worked, and I knew how to implement my own ideas in the Arduino Software. 

##### Custom PCB--
A Custom PCB is designed in the free fritzing software and ordered through SEEED Studios. Its just a simple PCB, containing pinouts for soldering all through-hole components together.

##### Where Did I Learn All This?--
Besides the fact that I've been teaching myself all this stuff,<br />
HUGE bragging rights within this project can be claimed by:<br />
Paul Butler ( my Dad )<br />
Paul McWhorter ( http://toptechboy.com )<br />
Adafruit Industries ( https://www.adafruit.com/ )<br />
Sparkfun Electronics ( https://www.sparkfun.com/ )<br />
BlueFlyVario ( http://blueflyvario.blogspot.com.au/ )<br />

All necessary files can be found in this repository, but feel free to dig through my un-organized version of the entire project at https://drive.google.com/open?id=0B610ABpCama_UF9jZ3pDQzFWTkE

# PREVIOUS VERSIONS --
### PROTOTYPE2: <br />
![alt tag](https://github.com/glydrfreak/vSpeedVario/blob/master/PROTOTYPE2.jpg)

### PROTOTYPE1: (Obviously one of a kind) <br />
![alt tag](https://github.com/glydrfreak/vSpeedVario/blob/master/PROTOTYPE1.jpg)

