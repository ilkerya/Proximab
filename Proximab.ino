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
 


#define ARM_MATH_CM0PLUS

 #define ARDUINO_MEGA // 8 bit AVR
 //#define ARDUINO_DUE // ARM Cortex M3

//#include <Wire.h>
//#include <SPI.h>
#include <SD.h>
  #ifdef ARDUINO_MEGA // 8 bit AVR
  #include <EEPROM.h>
  #include <avr/wdt.h>
  #endif

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

// C:\Projects\Pangolin\..   Atmel Studio Project Path
// C:\Projects\Pangolin\Pangolin\ArduinoCore\include\libraries\...  Atmel Studio Toolchain Compiler Lib Paths   



void setup() {
  MicroInit();
  // 
//  if(DisplayInitDelay == OFF)DisplayInitDelay = ON;  
}

// interrupt vector
    #ifdef ARDUINO_MEGA
ISR(TIMER1_OVF_vect){        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
 //   TCNT1 = 34286;            // preload timer for 500mSec
       TCNT1 = 64286;            // preload timer for 20mSec
    #endif
    
    #ifdef ARDUINO_DUE
void TC3_Handler(){
        TC_GetStatus(TC1, 0);
    #endif
       
    IntTimer250++;
    IntTimer500 ++;
    IntTimer1 ++;
    IntTimer2 ++;
    IntTimer5 ++;
    IntTimer10 ++;
    IntTimer20 ++;   
    IntTimer60 ++;   

    if(IntTimer250 >= 13){
      IntTimer250 = 0;
      LoopTask_250msec = ON;
    }
    if(IntTimer500 >= 25){ // 500 msec
      IntTimer500 = 0;
      LoopTask_500msec = ON;
    }
    if(IntTimer1 >= 50){  // 1 sec
      IntTimer1 = 0;
      LoopTask_1Sec = ON;
      digitalWrite(LED_GREEN, digitalRead(LED_GREEN) ^ 1);  
   
      if(DisplaySleepEnable == ON){
        if(OLED_Timer) OLED_Timer--;   // sleep active
      }
      else OLED_Timer = 32768; // no sleep    
      if(DisplayInitDelay == OFF)DisplayInitDelay = ON;           
    }
    if(IntTimer2 >= 100){ // 2 sec
      IntTimer2 = 0;
      LoopTask_2Sec = ON;
      //PrintDisplayBuffer();
    }
    if(IntTimer5 >= 250){  // 5 sec
      IntTimer5 = 0;
      LoopTask_5Sec = ON;
    }
    if(IntTimer10 >= 500){  // 10 sec
      IntTimer10 = 0;
      LoopTask_10Sec = ON;
    }
    if(IntTimer20 >= 1000){  // 20 sec
      IntTimer20 = 0;
      LoopTask_20Sec = ON;
    }
    if(IntTimer60 >= 3000){  // 60 sec
      IntTimer60 = 0;
      LoopTask_60Sec = ON;
    }        
    Key_Functions();
       //  digitalWrite(LED_RED, digitalRead(LED_RED) ^ 1);
     //   if(!digitalRead(KEY_LEFT) || !digitalRead(KEY_MID) || !digitalRead(KEY_RIGHT))
}
// the loop function runs over and over again forever
void loop() {
    Common_Loop(); 
       #ifdef ARDUINO_MEGA // 8 bit AVR 
 
        wdt_reset();
         wdt_enable(WDTO_8S);
     #endif
}       
