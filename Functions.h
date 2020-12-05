
// https://www.onlinegdb.com/edit/Hkmlxi_08



void Log_Data_Write_SD(){
    #ifdef SDCARD_EXISTS
      SD_CardLogTask(); // Data 2 SD card Write
    #endif 
}

void Common_Loop(){
  
  if (LoopTask_250msec) {
    LoopTask_250msec = OFF;

    // One time after wake up form sleep
    if (OLED_Init == ON) {
#ifndef DEBUG_SIMULATOR_MODE
      Display_ReInit(20);
#endif
      OLED_Init = OFF;
    }
    if (OLED_Timer) {
#ifndef DEBUG_SIMULATOR_MODE
      displayValues();
#endif
    }
    else {
#ifndef DEBUG_SIMULATOR_MODE
      Display_SwitchOff();
#endif
    }
#ifdef LEM_CURRENT_EXISTS
    AnalogValRead();
#endif

  }
  if (LoopTask_500msec) {
    LoopTask_500msec = OFF;
    if (SampleTime == TASK_500MSEC) Log_Data_Write_SD();
  }

  if (LoopTask_1Sec) {
    LoopTask_1Sec = OFF;
#ifndef DEBUG_SIMULATOR_MODE

    SerialPortRx();
    RTC_TimeClock();

#ifdef WIND_SENSOR_EXISTS
    WindSensorRead();
#endif

#ifdef TEMP_HUM_1_SENSOR_EXISTS
    deBugString = "TmpHmSn1_1";
    SensorRead_Si072(SI072_FIRST_SENSOR); // MULTIPLEXER NO
    deBugString = "TmpHmSn1_2";
#endif
#ifdef TEMP_HUM_2_SENSOR_EXISTS
    deBugString = "TmpHmSn2_1";
    SensorRead_Si072(SI072_SECOND_SENSOR); // MULTIPLEXER NO
    deBugString = "TmpHmSn2_2";
#endif
#ifdef TEMP_HUM_3_SENSOR_EXISTS
    deBugString = "TmpHmSn3_1";
    SensorRead_Si072(SI072_THIRD_SENSOR); // MULTIPLEXER NO
    deBugString = "TmpHmSn3_2";
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

     if((Menu == MENU5_SUB7)||(Menu == MENU2_SUB8)||(Menu == MENU3_SUB3) ||(Menu == MENU3_SUB4)||(Menu == MENU1_SUB3)||(Menu == MENU1_SUB4)||(Menu == MENU6_SUB3) ){
      Display.MenuTimeout++;
      if(Display.MenuTimeout > 4){
        Display.MenuTimeout = 0;
        Menu = MENU_NULL;
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
  
  if (LoopTask_2Sec) {
    LoopTask_2Sec = OFF;
    if (SampleTime == TASK_2SEC) Log_Data_Write_SD();

      UpdateDispRoll();
      PrintDisplayBuffer();

      if(SDCard.PauseTimer){
         SDCard.PauseTimer--;    
      }

        #ifdef AD9153_PROTOTYPE   
             PowerIC_Operation();
        #endif

  }
  if (LoopTask_5Sec) {
    LoopTask_5Sec = OFF;
    if (SampleTime == TASK_5SEC) Log_Data_Write_SD();

      #ifdef PM25_DUST_SENSOR_EXISTS  
          SDS_DustSensor();
      #endif




    Relay_loop();
 

    DisplayValueTimer++;
    if (DisplayValueTimer > 4)DisplayValueTimer = 0;

  }
  if (LoopTask_10Sec) {
    LoopTask_10Sec = OFF;
    if (SampleTime == TASK_10SEC) Log_Data_Write_SD();
 
  }
  if (LoopTask_20Sec) {
    LoopTask_20Sec = OFF;
    if (SampleTime == TASK_20SEC) Log_Data_Write_SD();

  }
  if (LoopTask_60Sec) {
    LoopTask_60Sec = OFF;
    if (SampleTime == TASK_60SEC) Log_Data_Write_SD();


  }
  
}

void AnalogValRead() {
  // https://www.onlinegdb.com/edit/Hkmlxi_08

  deBugString = "Cur_tRd_1";
#ifdef ARDUINO_MEGA
  ADCSRA |= (1 << ADEN); // enable adc
#endif
  delay(1);
  Current_Mains_Raw = analogRead(4);
  //  Current_Mains_Raw_Trim = Current_Mains_Raw;
  //  delay(1);
  //  239vac -> 4.94vdc
  // 242  vac  5vdc  242/1024 = 0.2363281

  //  238vac -> 3.10vdc  // 33K/8K2
  // 383  vac  5vdc  383/1024 = 0.374874

  CurrentArray[CurrentIndexer] = Current_Mains_Raw;
  CurrentIndexer++;
  if (CurrentIndexer >= 10) {
    CurrentIndexer = 0;
    unsigned int Cumulative = 0;
    for (unsigned int i = 0; i < 10; i++) {
      Cumulative += CurrentArray[i];
    }
#define Volt_Coeff 4.8828125 // rms 5v
#define PeakToRms  1.4142135
    // #define Rms_Coeff  6.90533966  // Volt_Coeff *  PeakToRms
#define Rms_Coeff  4.8828125  // Volt_Coeff *  PeakToRms       
    /*
        Current_MainsAverage = ((Cumulative%10) * Volt_Coeff )/10000;
       // then the decimal part
       Current_MainsAverage += ((float)(Cumulative/10) * Volt_Coeff )/1000;
    */
    Current_Mains_Rms = ((Cumulative % 10) * Rms_Coeff ) / 10000;
    // then the decimal part
    Current_Mains_Rms += ((float)(Cumulative / 10) * Rms_Coeff ) / 1000;

    //   Current_Mains_Rms = Current_MainsAverage * 1.414213;
    //   Current_Mains_Av = Current_Mains_Rms / 1,11;
    //  Current_MainsAverage = ((float)Cumulative * 5.044)/1000; // 2K2 // 4.7K // 2K2
    //   Current_MainsAverage = ((float)Cumulative * 9.766)/1000;  //direct
  }

  //  Current_Mains = ((float)Current_Mains_Raw * Rms_Coeff)/1000;
  // Current_Mains *= PeakToRms;

  //   Current_Mains = ((float)Current_Mains_Raw * 5.044)/1000;
  //   Current_Mains = ((float)Current_Mains_Raw * 9.766)/1000; // direct no voltage divider
  Mains_Volt_Raw =  analogRead(1);
  Mains_Volt =   (unsigned int)((float)Mains_Volt_Raw * 0.374874);

#ifdef ARDUINO_MEGA
  ADCSRA &= ~ (1 << ADEN);            // turn off ADC
#endif
  deBugString = "Cur_tRd_2";
}

void SDS_DustSensor(void) {
    #ifdef PM25_DUST_SENSOR_EXISTS
      SerialPortPMSensor();
/*
 // PmResult pm = sds.queryPm();// queay mode
  
     PmResult pm = sds.readPm();// active mode
  if (pm.isOk()) {
    Values.PM25 = pm.pm25;
    Values.PM10 = pm.pm10;
    if (Values.PM25 >= 250.0)Values.PM25 = 250.0;
    if (Values.PM10 >= 250.0)Values.PM25 = 250.0;

    // if you want to just print the measured values, you can use toString() method as well
      Serial.println(pm.toString());
    
  } else {
    Serial.print("Pm2.5 Problem: ");
 //   Serial.println(pm.statusToString());
  }
   */ 
#endif

}

void WindSensorRead() {
  /*
     #ifdef ARDUINO_MEGA
    ADCSRA |= (1 << ADEN); // enable adc
         #endif
    delay(1);
    //  Values.WindRaw = analogRead(2);delay(1);
    Values.WindMPH = analogRead(OUT_PINOUT);delay(1);
    Values.WindTemp = analogRead(TMP_PINOUT);
    WindSpeed_Calculation();
    #ifdef ARDUINO_MEGA
    ADCSRA &= ~ (1 << ADEN);            // turn off ADC
    #endif
  */
}

void IO_Settings() {

#ifdef FIRST_PROTOTYPE

#endif
#ifdef AD9153_PROTOTYPE
  pinMode(4, OUTPUT);  // ADE9153A_RESET_PIN
  digitalWrite(4, HIGH);
  pinMode(8, OUTPUT);  // ADE9153A_SPI_SS_PIN
  digitalWrite(8, HIGH);
  pinMode(2, INPUT);  // ADE9153A_ZX_DREADY_PIN
  pinMode(3, INPUT);  // ADE9153A_IRQ_PIN
  pinMode(5, INPUT);  // ADE9153A_USER_BUTTON 
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
   
#endif
  digitalWrite(RELAY_OUT_1, LOW);
  pinMode(RELAY_OUT_1, OUTPUT);  // SS Pin high to avoid miscommunication

  digitalWrite(RELAY_OUT_2, LOW);
  pinMode(RELAY_OUT_2, OUTPUT);  // SS Pin high to avoid miscommunication

 
  //const int chipSelect = 10; // mega SS for SD Card
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
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
  
  Serial.begin(115200);

  IO_Settings();
  DisplaySetPowerIO();

 




 #ifdef ARDUINO_MEGA // 8 bit AVR

#endif

  ResetCasePrint();
  //  SDCard.LogStatus = 0;      // default start with log off;
  EEDisplaySleepRead();
  EEReadLog();
  
  SDCard.LogBootInit = 0;  // put the header of the csv file

  Serial.print("SDCard.LogEnable: ");
  Serial.print(SDCard.LogEnable);
  Serial.print("    SampleTime: ");
  Serial.println(SampleTime);
  Serial.print("    DisplaySleep: ");
  Serial.println(DisplaySleepEnable);


  //wdt_enable(WDT0_1S);

#ifdef ARDUINO_MEGA

  wdt_reset();
  wdt_enable(WDTO_8S);
  //https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html

#endif

#ifdef ARDUINO_MEGA
  ADCSRA &= ~ (1 << ADEN);            // turn off ADC to save power ,, enable when needed and turn off again
#endif
      // Aug.03.2020 19:03:33
  FW_Version = String (__DATE__)  +  String ( __TIME__ ), // 11 1 8 // 20
  //  FW_Version.remove(17,3); // Remove from from index=7 through the end of the string
  FW_Version.remove(7, 4); // Remove 2 characters starting at index=7
  FW_Version.setCharAt(3 , '.');
 // FW_Version.setCharAt(6 , '.');

  //Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__);
  //Compiled: Jul 21 2020 15:55:39 7.3.0
  //  ShowSerialCode();

  UpdateDeviceEE();
  Serial.print("EE_Id_EString: ") ;
  Serial.print(EE_Id_EString.charAt(0));
  Serial.print(EE_Id_EString.charAt(1));
  Serial.print(EE_Id_EString.charAt(2));
  Serial.print(EE_Id_EString.charAt(3));
  Serial.println() ;

  // String LOG_FILE =  "LOG_xxxx.csv";
  // Rename Log File with the device Id
  LOG_FILE.setCharAt(4, EE_Id_EString.charAt(0));
  LOG_FILE.setCharAt(5, EE_Id_EString.charAt(1));
  LOG_FILE.setCharAt(6, EE_Id_EString.charAt(2));
  LOG_FILE.setCharAt(7, EE_Id_EString.charAt(3));

  Serial.print("LOG_FILE: ") ;
  Serial.print(LOG_FILE);
  Serial.println() ;

#ifndef DEBUG_SIMULATOR_MODE
  Sensors_PeripInit();
  DateTimeBuf.Init = ON;
  DisplayInit();
    
#endif

    #ifdef AD9153_PROTOTYPE
      //ADsetup();
      #endif  

#ifdef ARDUINO_MEGA
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
  startTimer(TC1, 0, TC3_IRQn, 64); //TC1 channel 0, the IRQ for that channel and the desired frequency
#endif



  
}

void Check_CompValue(void){
  
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

float GetValue(byte Relay){
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
  Serial.print("RL1Min: ");Serial.println(RL1Min);
  Serial.print(RLlVal+":  ");Serial.print(CompValue); 
  Serial.print("     RELAY1: ");Serial.println(RELAY_OUT_1); 
  Serial.print("RL1Max: ");Serial.println(RL1Max); 
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
  Serial.print("RL2Min: ");Serial.println(RL2Min);
  Serial.print(RL2Val+":  ");Serial.print(CompValue);
  Serial.print("      RELAY2: ");Serial.println(RELAY_OUT_2);
  Serial.print("RL2Max: ");Serial.println(RL2Max); 

 //   CurrentAD += 0.2;
  //  Values.TemperatureSi072_Ch1 -= 0.35;

 //   if( CurrentAD > 6.00) CurrentAD = 0.1;
  //   if( Values.TemperatureSi072_Ch1 < 13.00) Values.TemperatureSi072_Ch1 = 27.3;  

 
}
