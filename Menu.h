
void KeyTimeOutCheck(void) {
  if (KeyBoardTimeOutEnb == ON) {
    if (KeyBoardTimeOut)KeyBoardTimeOut--;
    else {
      KeyBoardTimeOutEnb = OFF;
      Menu =  MENU_NULL;
    }
  }
}

void KeyTimeOutStart(void) {
  KeyBoardTimeOutEnb = ON;
  KeyBoardTimeOut  = 24;// 24 sec
}

void DisplayWakeUp(void) {
  if ((OLED_Timer == 0) && (OLED_Init == OFF)) {
    OLED_Init = ON;
  }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}






void  DispExtTimeout(void) {
  if (OLED_Timer <= KEYDISP_TIMER) OLED_Timer = KEYDISP_TIMER;
}
void UpdateInfoQue(void){
     DispRollIndex[3] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[0];
     DispRollIndex[0]++;
     if (DispRollIndex[0]  > 8) DispRollIndex[0] = 1;
}
void UpdateDispRoll(void){
    if(SensorRollTimer){
      SensorRollTimer--;
      return;
    }
    UpdateInfoQue();

}
void KeySensonsorRoll(){
      SensorRollTimer = 30; // 2sec x 30 = 60 sec
      UpdateInfoQue();      
}
void DispEnable(bool Enable, byte Timer) {
  if (Enable == ON) {
    DisplaySleepEnable = ON; //go sleep
    //OLED_Timer = 20;
    OLED_Timer = Timer;
  }
  else   DisplaySleepEnable = OFF;    // no sleep
}

void LogEnable(bool Enable) {
  
    if(Enable == ON){
      SDCard.LogEnable = ON;
      EESetResetLog(ON);
    }
    if(Enable == OFF){
      SDCard.LogEnable = OFF;
      EESetResetLog(OFF);
    } 
}


void EE_SerNoWrite2_EE(unsigned int SerialNo) {
   #ifdef ARDUINO_MEGA // 8 bit AVR
  //  EEPROM.write(Adr, byte);
  // byte t = 0XFFFF & (SerialNo>>8);
    byte t;
    t = (byte)SerialNo;
    EEPROM.write(4, t);// low byte
    t = (byte)(SerialNo >> 8);
    EEPROM.write(5, t);// high byte
   #endif

}
void EEDisplaySleepRead(void) {
     #ifdef ARDUINO_MEGA // 8 bit AVR
      byte Mode = EEPROM.read(SLEEP_LOG);// OFF
      if(Mode == OFF)DispEnable(OFF,0);
      if(Mode == ON)DispEnable(ON,100);  
    #endif  
}

void EEDisplaySleep(bool Mode) {
    #ifdef ARDUINO_MEGA // 8 bit AVR
      if (Mode == OFF)EEPROM.write(SLEEP_LOG, OFF); // OFF
      else EEPROM.write(SLEEP_LOG, ON);// ON
    #endif
}


void EESetResetLog(bool Mode) {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
      if (Mode == OFF)EEPROM.write(ADDRES_LOG, OFF); // OFF
      else EEPROM.write(ADDRES_LOG, SampleTime);// ON
   #endif
}
void EESetSampleTimeLog(byte Sample) {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
      EEPROM.write(ADDRES_LOG, Sample);// ON
   #endif
}




void EEReadLog(void) {
   #ifdef ARDUINO_MEGA // 8 bit AVR
    byte Mode = EEPROM.read(ADDRES_LOG);// OFF
    switch (Mode) {
      case TASK_500MSEC: SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_1SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_2SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_5SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_10SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_20SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      case TASK_60SEC : SDCard.LogEnable = ON; SampleTime =  Mode;
      break;
      default: SDCard.LogEnable = OFF; SampleTime =  TASK_2SEC;
      break;
    }
   #endif
}
void UpdateDeviceEE() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    char c;
    EE_Id_EString = "";
    c = (char)EEPROM.read(4);
    EE_Id_EString += String(c);
    c = (char)EEPROM.read(5);
    EE_Id_EString += String(c);
    c = (char)EEPROM.read(6);
    EE_Id_EString += String(c);
    c = (char)EEPROM.read(7);
    EE_Id_EString += String(c);
    Serial.print(" Device Id: ");
    Serial.println(EE_Id_EString);
  #endif
}
void ResetCasePrint() {
#ifdef ARDUINO_MEGA // 8 bit AVR 
    
  byte RESET_CASE = MCUSR;
  Serial.print("MCUSR: ");
  Serial.println(RESET_CASE);
  if (MCUSR & WDRF) {
    Serial.print("Rebooting from a Watchdog Reset");
  }
  else if (MCUSR & BORF) {
    Serial.print("Rebooting from a Brown-out Reset");
  }
  else if (MCUSR & EXTRF) {
    Serial.print("Rebooting from an External Reset");
  }
  else if (MCUSR & PORF) {
    Serial.print("Rebooting from a Power On Reset");
  }
  if (MCUSR & (1 << WDRF)) {
    // a watchdog reset occurred
    Serial.print("Rebooting from a Watchdog Reset");
  }
  if (MCUSR & (1 << BORF)) {
    Serial.print("Rebooting from a Brown-out Reset");
  }
  //  a brownout reset occurred
  if (MCUSR & (1 << EXTRF)) {
    //  an external reset occurred
    Serial.print("Rebooting from an External Reset");
  }
  if (MCUSR & (1 << PORF)) {
    //  a power on reset occurred
    Serial.print("Rebooting from a Power On Reset");
  }
  //Clear register
  MCUSR = 0x00;

    #endif
}

void Key_Functions(void) {
  if (!KeyLeft_Rel && !KeyMid_Rel && !KeyRight_Rel) {
    KeyReleased = ON;
  }
  if (KeyReleased && !digitalRead(KEY_LEFT)) {
    KeyLeft_Rel = ON;
#ifdef DEBUG_KEY
    KeyLogger = 1;
#endif
    return;
  }
  if (KeyLeft_Rel  &&  digitalRead(KEY_LEFT)) {
    KeyLeft_Rel = OFF;
#ifdef DEBUG_KEY
    KeyLogger = 2;
#endif
    DisplayWakeUp();
    EnterMenuKey();
    KeyTimeOutStart();
    return;
  }
  if (KeyReleased && !digitalRead(KEY_MID)) {
    KeyMid_Rel = ON;
#ifdef DEBUG_KEY
    KeyLogger = 3;
#endif
    return;
  }
  if (KeyMid_Rel  &&  digitalRead(KEY_MID)) {
    KeyMid_Rel = OFF;
#ifdef DEBUG_KEY
    KeyLogger = 4;
#endif
    DisplayWakeUp();
    //DownMenuKey();
    UpMenuKey();   
    KeyTimeOutStart();
    return;
  }
  if (KeyReleased && !digitalRead(KEY_RIGHT)) {
    KeyRight_Rel = ON;
#ifdef DEBUG_KEY
    KeyLogger = 5;
#endif
    return;
  }
  if (KeyRight_Rel  &&  digitalRead(KEY_RIGHT)) {
    KeyRight_Rel = OFF;
#ifdef DEBUG_KEY
    KeyLogger = 6;
#endif
    DisplayWakeUp();
    EscMenuKey();
    KeyTimeOutStart();
    return;

  }
}
void DownMenuKey(void) {
  if (OLED_Timer == 0) return;
  DispExtTimeout();
  switch (Menu) {
    case MENU_NULL :// Menu = MENU_NULL;
            KeySensonsorRoll();
    
      break;
    case MENU1 : Menu = MENU6; //
      break;
    case MENU2 : Menu = MENU1;
      break;
    case MENU3 : Menu = MENU2;
      break;
    case MENU4 : Menu = MENU3;
      break;
     case MENU5 : Menu = MENU4;
      break;  
     case MENU6 : Menu = MENU5;
      break;  
      

    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB1 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB1;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB6;//
      break;
   case MENU2_SUB8 :  
      break;


    case MENU5_SUB1 :  
      break;
    case MENU5_SUB2 :  
      break;
    case MENU5_SUB3 :  
      break;
    case MENU5_SUB4 :  
      break;
    case MENU5_SUB5 :  
      break;
    case MENU5_SUB6 :  
      break;
     case MENU5_SUB7 :      
      break;

       case MENU6_SUB1:  //     
      break;
      case MENU6_SUB2:  //       
      break;        
       case MENU6_SUB3:      
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;


    default: Menu = MENU_NULL;

  }
}

void UpMenuKey(void) {
  if (OLED_Timer == 0) return;
  DispExtTimeout();
  switch (Menu) {
    case MENU_NULL : Menu = MENU_NULL;
          KeySensonsorRoll();
      break;
    case MENU1 : Menu = MENU2; //
      break;
    case MENU2 : Menu = MENU3;
      break;
    case MENU3 : Menu = MENU4;
      break;
    case MENU4 : Menu = MENU5;
      break;
    case MENU4_SUB1 :   Menu = MENU4_SUB2; 

      break;  
    case MENU4_SUB2 :   Menu = MENU4_SUB1; 

      break;

      
     case MENU5 : Menu = MENU6;
      break;     

     case MENU6 : Menu = MENU1;
      break; 
      
    case MENU1_SUB1 :  Menu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : Menu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB1 :  Menu =  MENU2_SUB2;//
      break;
    case MENU2_SUB2 :  Menu =  MENU2_SUB3;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2_SUB4;//
      break;
    case MENU2_SUB4 :  Menu =  MENU2_SUB5;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2_SUB6;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2_SUB7;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2_SUB1;//
      break;
       case MENU2_SUB8 :  
      break;

    case MENU3_SUB1 :  Menu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;


    case MENU5_SUB1 :  
        DateTimeBuf.Year++;
        if (DateTimeBuf.Year > 2030)DateTimeBuf.Year = 2020;
        if (DateTimeBuf.Year < 2020)DateTimeBuf.Year = 2020;
      break;
    case MENU5_SUB2 :
         DateTimeBuf.Month++;
        if (DateTimeBuf.Month > 12)DateTimeBuf.Month = 1;
        //if (DateTimeBuf.Month < 1)DateTimeBuf.Month = 1;
    
      break;
    case MENU5_SUB3 :  
          DateTimeBuf.Day ++;
        if (DateTimeBuf.Day > 31)DateTimeBuf.Day = 1;
        //if (DateTimeBuf.Day < 1)DateTimeBuf.Day = 1;
      break;
    case MENU5_SUB4 :
        DateTimeBuf.Hour ++;
        if (DateTimeBuf.Hour >= 24)DateTimeBuf.Hour = 0;
       // if (DateTimeBuf.Hour < 1)DateTimeBuf.Hour = 1;  
      break;
    case MENU5_SUB5 :  
              DateTimeBuf.Minute ++;
        if (DateTimeBuf.Minute >= 60)DateTimeBuf.Minute = 0;
      //  if (DateTimeBuf.Minute < 1)DateTimeBuf.Minute = 1;  
      break;
    case MENU5_SUB6 :  
         DateTimeBuf.Second ++;
        if (DateTimeBuf.Second >= 60)DateTimeBuf.Second = 0;    
      break;

     case MENU5_SUB7 :      
      break;


       case MENU6_SUB1:  //     
      break;
      case MENU6_SUB2:  //       
      break;        
       case MENU6_SUB3:      
      break;


    default: Menu = MENU_NULL;

  }
}


void EscMenuKey(void) {
  if (OLED_Timer == 0) return;
  DispExtTimeout();
  switch (Menu) {
    case MENU_NULL : Menu = MENU_NULL;
      break;
    case MENU1 : Menu = MENU_NULL; //
      break;
    case MENU2 : Menu = MENU_NULL;
      break;
    case MENU3 : Menu = MENU_NULL;
      break;
    case MENU4 : Menu = MENU_NULL;
      break;
    case MENU4_SUB1 :   Menu = MENU4; 

      break;  
    case MENU4_SUB2 :   Menu = MENU4; 

      break;       
    case MENU5 : Menu = MENU_NULL;
      break;

    case MENU1_SUB1 :  Menu =  MENU1;//
      break;
    case MENU1_SUB2 : Menu =  MENU1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;

    case MENU2_SUB1 :  Menu =  MENU2;//
      break;
    case MENU2_SUB2 : Menu =  MENU2;//
      break;
    case MENU2_SUB3 :  Menu =  MENU2;//
      break;
    case MENU2_SUB4 : Menu =  MENU2;//
      break;
    case MENU2_SUB5 :  Menu =  MENU2;//
      break;
    case MENU2_SUB6 :  Menu =  MENU2;//
      break;
    case MENU2_SUB7 :  Menu =  MENU2;//
      break;
      case MENU2_SUB8 : 
      break;

    case MENU3_SUB1 :  Menu =  MENU3;//
      break;
    case MENU3_SUB2 :  Menu =  MENU3;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;



    case MENU5_SUB1 :  Menu = MENU5;
      break;
    case MENU5_SUB2 :  Menu = MENU5_SUB1;
      break;
    case MENU5_SUB3 :  Menu = MENU5_SUB2;
      break;
    case MENU5_SUB4 :  Menu = MENU5_SUB3;
      break;
    case MENU5_SUB5 :  Menu = MENU5_SUB4;
      break;
    case MENU5_SUB6 :  Menu = MENU5_SUB5;
      break;
     case MENU5_SUB7 :      
      break;
       case MENU6:   Menu = MENU_NULL; //     
      break;
       case MENU6_SUB1: Menu = MENU6; //     
      break;
      case MENU6_SUB2: Menu = MENU6; //       
      break;        
       case MENU6_SUB3:      
      break;     
    default: Menu = MENU_NULL;

  }
}
void EnterMenuKey(void) {
  if (OLED_Timer == 0) return;
  DispExtTimeout();
  switch (Menu) {
    case MENU_NULL : Menu = MENU1;
          break;

          
    case MENU1 : //Menu = MENU1MIN; // go to sub menu  // sd kart log on
      if (SDCard.LogEnable == ON) Menu = MENU1_SUB2; //already logging
      else  Menu = MENU1_SUB1;
      break;

      break;

    case MENU1_SUB1 :  LogEnable(ON); //SDCard.LogEnable = ON; EESetResetLog(ON);
      Menu =  MENU1_SUB3;//MENU1
      break;
    case MENU1_SUB2 : LogEnable(OFF);// SDCard.LogEnable = OFF; EESetResetLog(OFF); // default
      Menu =  MENU1_SUB4;//MENU1
      break;
    case MENU1_SUB3 :  
      
      break;
    case MENU1_SUB4 : 
      
      break;


     case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one
      switch (SampleTime) {
        case TASK_500MSEC: Menu = MENU2_SUB1;
          break;
        case TASK_1SEC : Menu = MENU2_SUB2;
          break;
        case TASK_2SEC : Menu = MENU2_SUB3;
          break;
        case TASK_5SEC : Menu = MENU2_SUB4;
          break;
        case TASK_10SEC : Menu = MENU2_SUB5;
          break;
        case TASK_20SEC : Menu = MENU2_SUB6;
          break;
        case TASK_60SEC : Menu = MENU2_SUB7;
          break;
        default:
          break;
      }
      break;   
    case MENU2_SUB1 :  SampleTime = TASK_500MSEC ; EESetSampleTimeLog(TASK_500MSEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB2 : SampleTime = TASK_1SEC; EESetSampleTimeLog(TASK_1SEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB3 :  SampleTime = TASK_2SEC; EESetSampleTimeLog(TASK_2SEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB4 : SampleTime = TASK_5SEC; EESetSampleTimeLog(TASK_5SEC); // default
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB5 :  SampleTime = TASK_10SEC; EESetSampleTimeLog(TASK_10SEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB6 :  SampleTime = TASK_20SEC; EESetSampleTimeLog(TASK_20SEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB7 :  SampleTime = TASK_60SEC; EESetSampleTimeLog(TASK_60SEC);
      Menu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB8 :  
      
      break;
      
      case MENU3 : // Menu = MENU3MIN;
      if (DisplaySleepEnable == ON) Menu = MENU3_SUB2; //already logging
      else  Menu = MENU3_SUB1;
      break;    
    case MENU3_SUB1 :  DispEnable(ON,40);EEDisplaySleep(ON);
      Menu =  MENU3_SUB3;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF,0);EEDisplaySleep(OFF);
      Menu =  MENU3_SUB4;//MENU3
      break;
    case MENU3_SUB3 :  
      
      break;
    case MENU3_SUB4 :  
      
      break;
      
    case MENU4 :   Menu = MENU4_SUB1;  
       
      break;
    case MENU4_SUB1 :   Menu = MENU4_SUB2; 

      break;  
    case MENU4_SUB2 :   Menu = MENU4_SUB1; 

      break;      
    case MENU5 :        Menu = MENU5_SUB1;
        DateTimeBuf.Init = ON;       
      break;
     case MENU5_SUB1 :  Menu = MENU5_SUB2;   //Year  
      break;     
     case MENU5_SUB2 :  Menu = MENU5_SUB3; // Month
      break;        
     case MENU5_SUB3 :  Menu = MENU5_SUB4; // Day
      break;  
     case MENU5_SUB4 :  Menu = MENU5_SUB5; // Hour
      break;  
     case MENU5_SUB5 :  Menu = MENU5_SUB6; // Minute
      break;
     case MENU5_SUB6 :  Menu = MENU5_SUB7; //Second
          DateTimeBuf.RTC_Update = ON;    
      break;
     case MENU5_SUB7 :      
      break;
     case MENU6:   Menu = MENU6_SUB1; //     
      break;
       case MENU6_SUB1: Menu = MENU6_SUB2; //     
      break;
      case MENU6_SUB2: Menu = MENU6_SUB3; //
          PowerIC_Mode = POWERIC_CALB1;       
      break;        
       case MENU6_SUB3:      
      break;  
    default: Menu = MENU_NULL; 
  }
}
