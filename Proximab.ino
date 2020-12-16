/*
 Rev   Date         Description
 1.0   12.02.2020   Initial release
*/
 //for setting date&time open arduino serial monitor and send the data stream
 //   Year,Month,Date,Hour,Minute;Second
 //   2020,05,27,21,14,23
 //  2020,11,09,21,02,10
 // EEEEf567 
 // 115200 baud Both NL & CR
 // put leading zero for numbers less than 10

 //*********************************************************************
 
// C:\Users\ilker\Documents\Atmel Studio\7.0\trial\trial
// C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab
 //#define ARDUINO_MEGA // 8 bit AVR
 #define ARDUINO_DUE // ARM Cortex M3
 /*
//#include <Wire.h>
//#include <SPI.h>
 #include "RTClib.h"
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #include  <ADE9153A.h>
 #include <ADE9153AAPI.h>
 #include "Adafruit_Si7021.h"
  #include <SD.h>
  */
  #ifdef ARDUINO_MEGA // 8 bit AVR
    #include <EEPROM.h>
    #include <avr/wdt.h>
  #endif

#include <RTClib.h>
#include <Adafruit_Si7021.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>
#include <gfxfont.h>
/*
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
*/
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <ADE9153AAPI.h>
#include <ADE9153A.h>


#define ARM_MATH_CM0PLUS


#ifdef ARDUINO_DUE
void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  //    uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
  uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
  TC_SetRA(tc, channel, rc/2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}
#endif

// C:\Projects\Pangolin\..   Atmel Studio Project Path
// C:\Projects\Pangolin\Pangolin\ArduinoCore\include\libraries\...  Atmel Studio Toolchain Compiler Lib Paths   

// C:\Users\ilker\Documents\Atmel Studio\7.0\trial\trial
// C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab

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


void setup() {
  MicroInit();
}
// the loop function runs over and over again forever
void loop() {
    Common_Loop(); 
       #ifdef ARDUINO_MEGA // 8 bit AVR 
        wdt_reset();
         wdt_enable(WDTO_8S);
     #endif
}  
  
