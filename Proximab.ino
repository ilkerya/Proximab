/*
 Rev   Date         Description
 1.0   12.02.2020   Initial release
*/
 //for setting date&time open arduino serial monitor and send the data stream
 //   Year,Month,Date,Hour,Minute;Second
 //   2020,05,27,21,14,23
 //  2020,12,24,22,48,55
 // EEEEf567 
 // 115200 baud Both NL & CR
 // put leading zero for numbers less than 10

 //*********************************************************************
 
// C:\Users\ilker\Documents\Atmel Studio\7.0\trial\trial
// C:\Users\ilker\OneDrive\Belgeler\Arduino\Proximab
  #define ARDUINO_MEGA // 8 bit AVR
 //#define ARDUINO_DUE // ARM Cortex M3

//#pragma 
//#pragma

  #ifdef ARDUINO_MEGA // 8 bit AVR
    #include <EEPROM.h>
    #include <avr/wdt.h>
  #endif
  #ifdef ARDUINO_DUE // ARM 32 bit
    #include <malloc.h>
    #include <DueFlashStorage.h>
    DueFlashStorage dueFlashStorage;
  #endif
  
#include <RTClib.h>
#include <Adafruit_Si7021.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <ADE9153AAPI.h>
#include <ADE9153A.h>


#define ARM_MATH_CM0PLUS


  #ifdef ARDUINO_DUE // ARM 32 bit

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
//  tc->TC_CHANNEL[channel].TC_IER=  TC_IER_CPCS | TC_IER_CPAS;
 // tc->TC_CHANNEL[channel].TC_IDR=~(TC_IER_CPCS | TC_IER_CPAS);
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

  #ifdef ARDUINO_DUE // 
    Due_Memory();
    #endif

}
// the loop function runs over and over again forever
void loop() {
    Common_Loop(); 
       #ifdef ARDUINO_MEGA // 8 bit AVR 
        wdt_reset();
         wdt_enable(WDTO_8S);
     #endif
}  
#ifdef ARDUINO_DUE // 

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;
char Txt[256];
  
void Due_Memory() {
  char *heapend=sbrk(0);
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

/*
  char *p1;
  p1 = (char*)malloc( 8);  // allocate 50 bytes of memeory
  if(p1 == NULL) Serial.print("memory cannot be allocated\n");
  
  p1= (char*)realloc(p1,16);
  if(p1 == NULL) Serial.print("memory cannot be allocated\n");
  
  char *p2;
  p2 = (char*)calloc( 4,sizeof(int));  // allocate 50 bytes of memry with siz of each element all mem st 0 
  if(p2 == NULL) Serial.print("memory cannot be allocated\n");  
   
  free(p2);
*/
/*
        byte b1 = 3;
    byte b2 = 4;
    dueFlashStorage.write(ADR1,b1);  
    dueFlashStorage.write(ADR2,b2);  

   // dueFlashStorage.write(*p1,b1);  
   // dueFlashStorage.write(*(p1+1),b2);
     
    byte i = dueFlashStorage.read(ADR1); 
    byte j = dueFlashStorage.read(ADR2);

  Serial.print("i:");
  Serial.println(i);

   Serial.print("j:");
  Serial.println(j); 
     byte Mode_Read = dueFlashStorage.read(ADDRES_LOG);
     Serial.print("Mode_Read:");
    Serial.println(Mode_Read); 
    */
     

}
#endif
