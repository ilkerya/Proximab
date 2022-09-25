/*
  Rev   Date         Description
  1.0   12.02.2020   Initial release
*/
//for setting date&time open arduino serial monitor and send the data stream
//   Year,Month,Date,Hour,Minute;Second
//   2022,08,25,16,40,15
//  2020,12,24,22,48,55 TIME DATE COMMAND
// DEVIDf567   // DEV ID COMMAND DEVIDXXXX     Lxxxx_77.CSV  xxxx = 0000 - FFFF
// FILEQUE01 // FILE NO COMMAND  FILEQUEXX      LAB75_xx.CSV  xx = 00 - 99
// https://www.browserling.com/tools/random-hex
// 115200 baud Both NL & CR
// put leading zero for numbers less than 10
//*********************************************************************
// C:\Users\ilker\Documents\Atmel Studio\7.0\trial\trial
// C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab

#define ARDUINO_MEGA // 8 bit AVR Compiler -> GNU AVRDude 
//#define ARDUINO_DUE // ARM Cortex M3 -> GNU AVRDude
// #define CHIPKIT_MAX32 // PIC32MX795F512L
// #define ARDUINO_MKRZERO // ARM Cortex M0

#ifdef ARDUINO_MEGA // 8 bit AVR
#include <EEPROM.h>
#include <avr/wdt.h>
#include <ArduinoUniqueID.h>
#endif
#ifdef ARDUINO_DUE // ARM CortexM3 32 bit
#include <malloc.h>
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
#include <ArduinoUniqueID.h>
#endif

//#define OLED_128X128
#define OLED_128X64

#include <RTClib.h>
#include <Adafruit_Si7021.h>
#ifdef OLED_128X64
#include <Adafruit_SSD1306.h>
#endif
#ifdef OLED_128X128
#include <Adafruit_SSD1327.h>
#endif
#include <SD.h>
#include <Wire.h>
//#include <Wire1.h>
#include <SPI.h>




#define ARM_MATH_CM0PLUS


#ifdef ARDUINO_DUE // ARM 32 bit
void wdt_enable(void)
{
  // Enable watchdog.
  WDT->WDT_MR = WDT_MR_WDD(0xFFF)
                | WDT_MR_WDRPROC
                | WDT_MR_WDRSTEN
                | WDT_MR_WDV(256 * 2); // Watchdog triggers a reset after 2 seconds if underflow
  // 2 seconds equal 84000000 * 2 = 168000000 clock cycles
  /* Slow clock is running at 32.768 kHz
    watchdog frequency is therefore 32768 / 128 = 256 Hz
    WDV holds the periode in 256 th of seconds  */

//  Serial.begin(250000);
//  uint32_t status = (RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> RSTC_SR_RSTTYP_Pos; // Get status from the last Reset
 // Serial.print("RSTTYP = 0b"); Serial.println(status, BIN);  // Should be 0b010 after first watchdog reset
}
void wdt_reset(void){
  #define WDT_KEY (0xA5)
  WDT->WDT_CR = WDT_CR_KEY(WDT_KEY) | WDT_CR_WDRSTT;
  GPBR->SYS_GPBR[0] += 1;    
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  //    uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
  uint32_t rc = VARIANT_MCK / 128 / frequency; //128 because we selected TIMER_CLOCK4 above
  TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
  //  tc->TC_CHANNEL[channel].TC_IER=  TC_IER_CPCS | TC_IER_CPAS;
  // tc->TC_CHANNEL[channel].TC_IDR=~(TC_IER_CPCS | TC_IER_CPAS);
  NVIC_EnableIRQ(irq);
}
#endif
#ifdef ARDUINO_MKRZERO // ARM Cortex M0
//Configures the TC to generate output events at the sample frequency.
//Configures the TC in Frequency Generation mode, with an event output once
//each time the audio sample frequency period expires.
//https://gist.github.com/nonsintetic/ad13e70f164801325f5f552f84306d6f
void tcConfigure(int sampleRate)
{
  //1000 -> 1000 ms
  //10000 -> 100 ms
  //50000 -> 20ms
  // select the generic clock generator used as source to the generic clock multiplexer
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
  while (GCLK->STATUS.bit.SYNCBUSY);

  tcReset(); //reset TC5

  // Set Timer counter 5 Mode to 16 bits, it will become a 16bit counter ('mode1' in the datasheet)
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  // Set TC5 waveform generation mode to 'match frequency'
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  //set prescaler
  //the clock normally counts at the GCLK_TC frequency, but we can set it to divide that frequency to slow it down
  //you can use different prescaler divisons here like TC_CTRLA_PRESCALER_DIV1 to get a different range
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_ENABLE; //it will divide GCLK_TC frequency by 1024
  //set the compare-capture register.
  //The counter will count up to this value (it's a 16bit counter so we use uint16_t)
  //this is how we fine-tune the frequency, make it count to a lower or higher value
  //system clock should be 1MHz (8MHz/8) at Reset by default
  TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate);
  while (tcIsSyncing());

  // Configure interrupt request
  NVIC_DisableIRQ(TC5_IRQn);
  NVIC_ClearPendingIRQ(TC5_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);
  NVIC_EnableIRQ(TC5_IRQn);

  // Enable the TC5 interrupt request
  TC5->COUNT16.INTENSET.bit.MC0 = 1;
  while (tcIsSyncing()); //wait until TC5 is done syncing
}

//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

//This function enables TC5 and waits for it to be ready
void tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

//Reset TC5
void tcReset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//disable TC5
void tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}
#endif

#include  "Defs.h"
#include  "Variables.h"
#include  "RTC.h"
#include  "SDCard.h"
#include  "UserInt.h"
#include  "Display.h"
#include  "Sensors.h"
#include  "ADE9153.h"
#include  "Menu.h"
#include  "Functions.h"


void setup() {
  MicroInit(); //
#ifdef ARDUINO_DUE // 
  //Due_Memory();
  analogReadResolution(12);
#endif
}
// the loop function runs over and over again forever
void loop() {
    Common_Loop();
    #ifdef ARDUINO_MEGA // 8 bit AVR 
      wdt_reset();
      wdt_enable(WDTO_8S);
    #endif
    #ifdef ARDUINO_DUE // 
      wdt_reset();
      wdt_enable();
    #endif
    #ifdef ARDUINO_MKRZERO

    #endif
}
#ifdef ARDUINO_DUE // 

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart = (char *)0x20070000;
char *ramend = (char *)0x20088000;
char Txt[256];

void Due_Memory() {

  char *heapend = sbrk(0);
  register char * stack_ptr asm("sp");
  struct mallinfo mi = mallinfo();

  sprintf(Txt, "    arena = %d\n", mi.arena);     Serial.print(Txt);
  sprintf(Txt, "  ordblks = %d\n", mi.ordblks);   Serial.print(Txt);
  sprintf(Txt, " uordblks = %d\n", mi.uordblks);  Serial.print(Txt);
  sprintf(Txt, " fordblks = %d\n", mi.fordblks);  Serial.print(Txt);
  sprintf(Txt, " keepcost = %d\n", mi.keepcost);  Serial.println(Txt);

  sprintf(Txt, "RAM Start:    %lx\n", (unsigned long)ramstart);  Serial.print(Txt);
  sprintf(Txt, "Data/Bss end: %lx\n", (unsigned long)&_end);     Serial.print(Txt);
  sprintf(Txt, "Heap End:     %lx\n", (unsigned long)heapend);   Serial.print(Txt);
  sprintf(Txt, "Stack Ptr:    %lx\n", (unsigned long)stack_ptr); Serial.print(Txt);
  sprintf(Txt, "RAM End:      %lx\n", (unsigned long)ramend);    Serial.println(Txt);

  sprintf(Txt, "Heap RAM Used:      %d\n",   mi.uordblks);                       Serial.print(Txt);
  sprintf(Txt, "Program RAM Used:   %d\n",   &_end - ramstart);                  Serial.print(Txt);
  sprintf(Txt, "Stack RAM Used:     %d\n",   ramend - stack_ptr);                Serial.print(Txt);
  sprintf(Txt, "Estimated Free RAM: %d\n\n", stack_ptr - heapend + mi.fordblks); Serial.println(Txt);
  //dueFlashStorage.write(0,b1);

}
#endif
