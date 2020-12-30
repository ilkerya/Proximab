C Standarts
http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf

git add .
git commit -m "Comment"
git push https://github.com/ilkerya/Pangolin.git master

First Time
git init
git add README.md

GitHub Bash Commands
Adding new file
https://help.github.com/articles/adding-a-file-to-a-repository-using-the-command-line/#platform-windows

git add .
git commit -m "Add existing file"
// For Common git
git push https://github.com/ilkerya/Pangolin master
// For Electrolux git
git push  http://git.int.electrolux.com/ilkerya/Phoenix_Pangolin.git master
https://help.github.com/articles/fetching-a-remote/
http://git.int.electrolux.com/ilkerya/Phoenix_Mobile.git

In case in the first push gives error use below command
git remote add origin remote repository URL
git push origin master
git push https://github.com/ilkerya/Phoenix master --force

DataLogger Arduino Atmega/Arduino Due // Arduino IDE / Microchip Studio 7.0
CPU Modules : Both Can Used as Selectable Due Needs to be debugged for I2C issue!!
https://content.arduino.cc/assets/Pinout-Mega2560rev3_latest.pdf https://content.arduino.cc/assets/Pinout-Due_latest.pdf
RTC and SD CArd Module
https://www.adafruit.com/product/1141
Display 128x64 SSD1306 OLED Display
https://esphome.io/components/display/ssd1306.html
Sensors That Has been Used:
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-tsl2591.pdf
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-bmp388.pdf?timestamp=1556108471
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-si7021-temperature-plus-humidity-sensor.pdf
https://learn.adafruit.com/adafruit-lsm9ds1-accelerometer-plus-gyro-plus-magnetometer-9-dof-breakout
https://moderndevice.com/product/wind-sensor/
Energy Metering IC from Analog Devices
https://www.analog.com/media/en/technical-documentation/data-sheets/ade9153a.pdf https://www.analog.com/en/products/ade9153a.html#product-overview
Debugger & Protocol Analyzer
https://reference.digilentinc.com/reference/instrumentation/analog-discovery-2/start

// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries
// Location of Main Librarires

// Set Origin
//   git remote add origin git@github.com:ilkerya/Pangolin.git
//  https://github.com/ilkerya/Pangolin
//  https://blog.plover.com/prog/git-ff-error.html
// C:\Users\Yagciilk\Documents\Arduino\libraries
// C:\Program Files (x86)\Arduino\libraries   
// C:\Projects\Pangolin\Pangolin\ArduinoCore\include  default AtmelStudio Project lib locations

#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Defs.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Variables.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\RTC.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\SDCard.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\UserInt.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Display.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Sensors.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\ADE9153.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Menu.h"
#include  "C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab\Functions.h"
