
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

     #ifdef ARDUINO_MKRZERO
void TC5_Handler (void) {
  
      #endif 
    
     #ifdef CHIPKIT_MAX32
  //  void __ISR(_TIMER_2_VECTOR, i p l 3 ) myISR function ( void ){
void JustAFunction(){
    //  ISR(TIMER1_OVF_vect){
    #endif  
    #if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE)//  | defined (ARDUINO_MKRZERO)
          digitalWrite(DEBUG_OUT, digitalRead(DEBUG_OUT) ^ 1);   
    #endif
    #if defined (ARDUINO_MKRZERO) | defined (CHIPKIT_MAX32)
          digitalWrite(DEBUG_OUT, digitalRead(DEBUG_OUT) ^ 1);   
    #endif     
    Loop.IntTimer_250++;
    Loop.IntTimer_500 ++;
    Loop.IntTimer_1 ++;
    Loop.IntTimer_2 ++;
    Loop.IntTimer_5 ++;
    Loop.IntTimer_10 ++;
    Loop.IntTimer_20 ++;   
    Loop.IntTimer_60 ++;   

    if(Loop.IntTimer_250 >= 13){
      Loop.IntTimer_250 = 0;
      Loop.Task_250msec = ON;
    }
    if(Loop.IntTimer_500 >= 25){ // 500 msec
      Loop.IntTimer_500 = 0;
      Loop.Task_500msec = ON;  
    }
    if(Loop.IntTimer_1 >= 50){  // 1 sec
      Loop.IntTimer_1 = 0;
      Loop.Task_1Sec = ON;
      digitalWrite(LED_GREEN, digitalRead(LED_GREEN) ^ 1);  
 
      if(Display.SleepEnable == ON){
        if(Display.OLED_Timer) Display.OLED_Timer--;   // sleep active
      }
      else Display.OLED_Timer = 32768; // no sleep    
      if(Display.InitDelay == OFF)Display.InitDelay = ON;           
    }
    if(Loop.IntTimer_2 >= 100){ // 2 sec
      Loop.IntTimer_2 = 0;
      Loop.Task_2Sec = ON;
      //PrintDisplayBuffer();
    }
    if(Loop.IntTimer_5 >= 250){  // 5 sec
      Loop.IntTimer_5 = 0;
      Loop.Task_5Sec = ON;
    }
    if(Loop.IntTimer_10 >= 500){  // 10 sec
      Loop.IntTimer_10 = 0;
      Loop.Task_10Sec = ON;
    }
    if(Loop.IntTimer_20 >= 1000){  // 20 sec
      Loop.IntTimer_20 = 0;
      Loop.Task_20Sec = ON;
    }
    if(Loop.IntTimer_60 >= 3000){  // 60 sec
      Loop.IntTimer_60 = 0;
      Loop.Task_60Sec = ON;
    }        
    Key_Functions();
       //  digitalWrite(LED_RED, digitalRead(LED_RED) ^ 1);
     //   if(!digitalRead(KEY_LEFT) || !digitalRead(KEY_MID) || !digitalRead(KEY_RIGHT))
     #ifdef ARDUINO_MKRZERO
         TC5->COUNT16.INTFLAG.bit.MC0 = 1; //Writing a 1 to INTFLAG.bit.MC0 clears the interrupt so that it will run again
     #endif

}
// https://www.onlinegdb.com/edit/Hkmlxi_08

void Log_Data_Write_SD(){
    #ifdef SDCARD_EXISTS
      SD_CardLogTask(); // Data 2 SD card Write
    #endif 
}

void Common_Loop(){
  if (Loop.Task_250msec) {
    Loop.Task_250msec = OFF;
	  #ifdef OLEDDISPLAY_EXISTS
    // One time after wake up form sleep
    if (Display.OLED_Init == ON) {
      Display_ReInit_Start(20);
      Display.OLED_Init = OFF;
    }
    if (Display.OLED_Timer) {
      displayValues();
    }
    else {

      Display_SwitchOff();
    }
    Display_ReInit_End();
    #endif
    #ifdef LEM_CURRENT_EXISTS
        AnalogValRead();
    #endif

  }
  if (Loop.Task_500msec) {
    Loop.Task_500msec = OFF;
    if (SampleTime == TASK_500MSEC) Log_Data_Write_SD();
  }

  if (Loop.Task_1Sec) {
    Loop.Task_1Sec = OFF;
#ifndef DEBUG_SIMULATOR_MODE

    SerialPortRx();
    RTC_TimeClock();

#ifdef WIND_SENSOR_EXISTS
    WindSensorRead();
#endif
#ifdef TEMP_HUM_1_SENSOR_EXISTS
    SensorRead_Si072(SI072_FIRST_SENSOR); // MULTIPLEXER NO  
#endif
#ifdef TEMP_HUM_2_SENSOR_EXISTS  
    SensorRead_Si072(SI072_SECOND_SENSOR); // MULTIPLEXER NO  
#endif
#ifdef TEMP_HUM_3_SENSOR_EXISTS 
    SensorRead_Si072(SI072_THIRD_SENSOR); // MULTIPLEXER NO
#endif

#ifdef ENERGYMETER_EXISTS
     EnergymeterCalbLed( );
#endif

#ifdef  BAR_PRES_SENSOR_EXISTS
    SensorAlt_Read();
#endif
#ifdef LIGHT_SENSOR_EXISTS
    SensorLight_Read();
#endif
#ifdef ACCL_GYRO_SENSOR_EXISTS
    SensorAcccel_GyroRead();
#endif
#endif // end of  #ifndef DEBUG_SIMULATOR_MODE

    KeyTimeOutCheck();
    if (SampleTime == TASK_1SEC) Log_Data_Write_SD();
     if((MainMenu == MENU5_SUB7)||(MainMenu == MENU2_SUB8)||(MainMenu == MENU3_SUB3) ||(MainMenu == MENU3_SUB4)||(MainMenu == MENU1_SUB3)||(MainMenu == MENU1_SUB4)||(MainMenu == MENU6_SUB3) ){
      Display.MenuTimeout++;
      if(Display.MenuTimeout > 4){
        Display.MenuTimeout = 0;
        MainMenu = MENU_NULL;
      }
    }
   
/*
      if( (Values.PM25 > 64) &&  !digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,HIGH);
      if( (Values.PM25 < 16) &&   digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,LOW);

      //    digitalWrite(RELAY_OUT_1, digitalRead(RELAY_OUT_1) ^ 1);  

      if(Values.Current > 4 && !digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,HIGH);
      if(Values.Current < 1 && digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,LOW);     
*/
           //     digitalWrite(RELAY_OUT_2, digitalRead(RELAY_OUT_2) ^ 1);  
  }  
  if (Loop.Task_2Sec) {
    Loop.Task_2Sec = OFF;
    if (SampleTime == TASK_2SEC) Log_Data_Write_SD();
      UpdateDispRoll();
      PrintDisplayBuffer();

      if(SDCard.PauseTimer){
         SDCard.PauseTimer--;    
      }
        #ifdef ENERGYMETER_EXISTS   
             EnergyMeterIC_Operation();
        #endif
  }
  if (Loop.Task_5Sec) {
    Loop.Task_5Sec = OFF;
    if (SampleTime == TASK_5SEC) Log_Data_Write_SD();
      #ifdef PM25_DUST_SENSOR_EXISTS  
          SDS_DustSensor();
      #endif

    Relay_loop();
 
    Display.ValueTimer++;
    if (Display.ValueTimer > 4)Display.ValueTimer = 0;
  }
  if (Loop.Task_10Sec) {
    Loop.Task_10Sec = OFF;
    if (SampleTime == TASK_10SEC) Log_Data_Write_SD();
  }
  if (Loop.Task_20Sec) {
    Loop.Task_20Sec = OFF;
    if (SampleTime == TASK_20SEC) Log_Data_Write_SD();
  }
  if (Loop.Task_60Sec) {
    Loop.Task_60Sec = OFF;
    if (SampleTime == TASK_60SEC) Log_Data_Write_SD();
  }
}

void IO_Settings() {

#ifdef FIRST_PROTOTYPE

#endif

#ifdef ARDUINO_MKRZERO
  digitalWrite(OLED_GND, LOW);
  pinMode(OLED_GND, OUTPUT);  // 


    pinMode(A4, INPUT);  
  digitalWrite(OLED_CS, LOW);
  pinMode(OLED_CS, OUTPUT);  //
   digitalWrite(OLED_RESET, LOW);
  pinMode(OLED_RESET, OUTPUT);  // 



   digitalWrite(OLED_DC, LOW);
  pinMode(OLED_DC, OUTPUT);  // 
  digitalWrite(OLED_CLK, LOW);
  pinMode(OLED_CLK, OUTPUT);  // 
  digitalWrite(OLED_MOSI, LOW);
  pinMode(OLED_MOSI, OUTPUT);  // 

  digitalWrite(OLED_POWER, HIGH);
  pinMode(OLED_POWER, OUTPUT);  // 
  
#endif

#ifdef ENERGYMETER_EXISTS
/*
  pinMode(A5, INPUT);  // 
  pinMode(A4, INPUT);  // 
  pinMode(A3, INPUT);  // 
  pinMode(A2, INPUT);  // 
  pinMode(A1, INPUT);  // 
  digitalWrite(A4, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, HIGH);

  pinMode(A5, INPUT_PULLUP);  //
  pinMode(A4, INPUT_PULLUP);  //
  pinMode(A3, INPUT_PULLUP);  //
  pinMode(A2, INPUT_PULLUP);  //
  pinMode(A1, INPUT_PULLUP);  //  
  */
  
//  pinMode(I2C_TIMEOUT, INPUT);  // 
  pinMode(DEBUG_OUT, OUTPUT);  // 

  pinMode(ADE9153A_RESET_PIN, OUTPUT);  // ADE9153A_RESET_PIN
  digitalWrite(ADE9153A_RESET_PIN, HIGH);
  pinMode(ADE9153A_CS_PIN, OUTPUT);  // ADE9153A_SPI_SS_PIN
  digitalWrite(ADE9153A_CS_PIN, HIGH);
  pinMode(ADE9153A_ZX_DREADY_PIN, INPUT);  // ADE9153A_ZX_DREADY_PIN
  pinMode(ADE9153A_IRQ_PIN, INPUT);  // ADE9153A_IRQ_PIN
  pinMode(ADE9153A_CALB_BUTTON, INPUT);  // ADE9153A_USER_BUTTON 
  
  pinMode(ADE9153A_ZX_DREADY_PIN, INPUT_PULLUP);
  pinMode(ADE9153A_IRQ_PIN, INPUT_PULLUP);
  pinMode(ADE9153A_CALB_BUTTON, INPUT_PULLUP);
   
#endif
  digitalWrite(RELAY_OUT_1, LOW);
  pinMode(RELAY_OUT_1, OUTPUT);  // SS Pin high to avoid miscommunication

  digitalWrite(RELAY_OUT_2, LOW);
  pinMode(RELAY_OUT_2, OUTPUT);  // SS Pin high to avoid miscommunication
  
//  pinMode(SD_CS_PINOUT, OUTPUT);
 // digitalWrite(SD_CS_PINOUT, HIGH);
  
  pinMode(LED_GREEN, OUTPUT);           // set pin to input
  digitalWrite(LED_GREEN, LOW);       // turn on pullup resistors
  pinMode(LED_RED, OUTPUT);           // set pin to input
  digitalWrite(LED_RED, LOW);       // turn on pullup resistors

  pinMode(KEY_LEFT, INPUT);           // set pin to input
  pinMode(KEY_LEFT, INPUT_PULLUP);

  pinMode(KEY_MID, INPUT);           // set pin to input
  pinMode(KEY_MID, INPUT_PULLUP);

  pinMode(KEY_RIGHT, INPUT);           // set pin to input
  pinMode(KEY_RIGHT, INPUT_PULLUP);

}
void MicroInit() {
 // Display_Line4[5] ='\0';
  Serial.begin(115200);
  IO_Settings();
  #ifdef OLEDDISPLAY_EXISTS
  DisplaySetPowerIO();
  #endif
  
  ResetCasePrint();
  //  SDCard.LogStatus = 0;      // default start with log off;

  NVRam_Read_Standbye();
  NVRam_Read_SampleTime();
  
  SDCard.LogBootInit = 0;  // put the header of the csv file

  Serial.print(F("SDCard.LogEnable: "));
  Serial.print(SDCard.LogEnable);
  Serial.print(F("    SampleTime: "));
  Serial.println(SampleTime);
  Serial.print(F("    DisplaySleep: "));
  Serial.println(Display.SleepEnable);

#ifdef ARDUINO_MEGA
  wdt_reset();
  wdt_enable(WDTO_8S);   //wdt_enable(WDT0_1S);
  //https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
#endif

#ifdef ARDUINO_MEGA
  ADCSRA &= ~ (1 << ADEN);            // turn off ADC to save power ,, enable when needed and turn off again
#endif

  Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__);
  //Compiled: Jul 21 2020 15:55:39 7.3.0
  //  ShowSerialCode();

  NVRam_Read_SerNo();
  UpdateLogFileId();

//#ifndef DEBUG_SIMULATOR_MODE
   #ifdef SDCARD_EXISTS
    SD_Card_Info();
   SD_Card_Init();
    GetFileSize();
   ReadConfigFile(); // 2020.12.25
  #endif  
  
  Sensors_PeripInit();
  DateTimeBuf.Init = ON;
  #ifdef OLEDDISPLAY_EXISTS
  DisplayInit();   
 // #endif
#endif


#ifdef ARDUINO_MEGA
  // initialize timer1
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    //    TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz 500mS
    TCNT1 = 64286;            // preload timer 65536-16MHz/256/50Hz 20 ms
    TCCR1B |= (1 << CS12);    // 256 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();
#endif

#ifdef ARDUINO_DUE
    startTimer(TC1, 0, TC3_IRQn, 50); //TC1 channel 0, the IRQ for that channel and the desired frequency 64
        // 20 -> 50ms   1000/20 = 50
        // 32 -> 32ms       
        // 50 -> 20ms
        // 51 -> 19.62 m2
        // 52 -> 19.22 ms      
        // 54 -> 18.63
        // 64 -> 15.68 ms
#endif
 #ifdef ARDUINO_MKRZERO // ARM Cortex M0
  tcConfigure(50000); //configure the timer to run at <sampleRate>Hertz
  tcStartCounter(); //starts the timer
#endif

}

void Parse_FileString(){
  int DelimCount=0;
  int j = 0;
  int index;
  for (unsigned int i = 0; i < Config_Str.length(); i++) {
    if (Config_Str.substring(i, i+1) == ",") {
      switch(DelimCount){
        case 0:Relay1str = Config_Str.substring(j, i);
        break;
        case 1:RlStr2 = Config_Str.substring(j,i);
        break;
        case 2:RLlVal = Config_Str.substring(j,i);
        break;
        case 3:RlStr4 = Config_Str.substring(j,i);
        break;
        case 4:Relay2str = Config_Str.substring(j,i);
        break;
        case 5:RlStr6 = Config_Str.substring(j,i);
        break; 
        case 6:RL2Val = Config_Str.substring(j,i);
             j = i+1;
             RlStr8 = Config_Str.substring(j);
        break; 
        default:
        break;   
      }
      j = i+1;
      DelimCount++;
    }
  }
 
    index = ELEMENTS;
    Relay1str.trim();//remove leadig & last space characters
    if(Relay1str == "Relay1"){
     // index == 0; ?????????????????
      RLlVal.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RLlVal == KeyWords[index]){      
          RlStr2.trim();
          RlStr4.trim();       
          RL1Min =RlStr2.toFloat();
          RL1Max= RlStr4.toFloat();
          
          break;  
        }  
      } 
   }
   if(index == ELEMENTS) {
        Relay1str = "---";     
        RLlVal = "Nan";   
        RlStr2 = "----";
        RlStr4 = "----";
    }
    index = ELEMENTS;
    Relay2str.trim();//remove leadig & last space characters
    if(Relay2str == "Relay2"){
      index = 0;
      RL2Val.trim();  // Temperature // Current // PM25
      for( index = 0; index < ELEMENTS; index++){
        if(RL2Val == KeyWords[index]){      
          RlStr6.trim();
          RlStr8.trim();       
          RL2Min= RlStr6.toFloat();
          RL2Max= RlStr8.toFloat();
          break;  
        }  
      }     
   }
    if(index == ELEMENTS) {
        Relay2str = "---";     
        RL2Val = "Nan";   
        RlStr6 = "----";
        RlStr8 = "----";
    }
   // Serial.println("'''''''''''''''''");
  Serial.println(Relay1str);
  Serial.println(RlStr2);
  Serial.println(RLlVal);
  Serial.println(RlStr4);
  Serial.println(Relay2str);
  Serial.println(RlStr6);
  Serial.println(RL2Val);
  Serial.println(RlStr8);
}
#define Relay1_Val 8
#define Relay2_Val 4

float GetValue(uint8_t Relay){
  String Val = "";
  if(Relay == Relay1_Val) Val =  RLlVal;
  else if(Relay == Relay2_Val) Val =  RL2Val;
  if(Val == KeyWords[0])return Values.TemperatureSi072_Ch1;
  if(Val == KeyWords[1])return Values.TemperatureSi072_Ch2;
  if(Val == KeyWords[2])return  Values.TemperatureSi072_Ch3;
  if(Val == KeyWords[3])return Values.Humidity_Ch1;
  if(Val == KeyWords[4])return  Values.Humidity_Ch2;
  if(Val == KeyWords[5])return  Values.Humidity_Ch3;
  if(Val == KeyWords[6])return  Values.PM25;
  if(Val == KeyWords[7])return  Values.PM10;
  if(Val == KeyWords[8]) return  Values.Current;
  if(Val == KeyWords[9]) return  Values.Voltage;
  if(Val == KeyWords[10]) return  Values.ActivePower;  
  if(Val == KeyWords[11]) return  Values.PowerFactor;     
  return 0;
}
void Relay_loop() {
 // Parse_FileString();
  CompValue = 0;
  if( RLlVal != "Nan"){
    CompValue = GetValue(Relay1_Val);
    if(CompValue > RL1Max && !digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,HIGH);
    if(CompValue < RL1Min &&  digitalRead(RELAY_OUT_1) ) digitalWrite(RELAY_OUT_1,LOW); 
  }
  else{
      digitalWrite(RELAY_OUT_1,LOW);
  }
   // put your main code here, to run repeatedly:
  Serial.print(F("RL1Min: "));Serial.println(RL1Min);
  Serial.print(RLlVal+":  ");Serial.print(CompValue); 
  Serial.print(F("     RELAY1: "));Serial.println(RELAY_OUT_1); 
  Serial.print(F("RL1Max: "));Serial.println(RL1Max); 
  CompValue = 0;
  if( RL2Val != "Nan"){
    CompValue = GetValue(Relay2_Val);
    //if((CompValue < RL2Min)&& RELAY2) RELAY2 = 0;
    //if((CompValue > RL2Max)&& !RELAY2) RELAY2 = 1;  
    if(CompValue > RL2Max && !digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,HIGH);
    if(CompValue < RL2Min &&  digitalRead(RELAY_OUT_2) ) digitalWrite(RELAY_OUT_2,LOW);  
  }
  else{
       digitalWrite(RELAY_OUT_2,LOW);  
  }
  Serial.print(F("RL2Min: "));Serial.println(RL2Min);
  Serial.print(RL2Val+":  ");Serial.print(CompValue);
  Serial.print(F("      RELAY2: "));Serial.println(RELAY_OUT_2);
  Serial.print(F("RL2Max: "));Serial.println(RL2Max);  
}
