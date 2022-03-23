
void KeyTimeOutCheck(void) {
  if (Key.BoardTimeOutEnb == ON) {
    if (Key.BoardTimeOut)Key.BoardTimeOut--;
    else {
      Key.BoardTimeOutEnb = OFF;
      MainMenu =  MENU_NULL;
    }
  }
}
  
void KeyTimeOutStart(void) {
  Key.BoardTimeOutEnb = ON;
  Key.BoardTimeOut  = 24;// 24 sec
}

void DisplayWakeUp(void) {
  if ((Display.OLED_Timer == 0) && (Display.OLED_Init == OFF)) {
    Display.OLED_Init = ON;
  }//else if(OLED_Timer)OLED_Timer += 10; // add additional time every time any key released
}

void  DispExtTimeout(void) {
  if (Display.OLED_Timer <= KEYDISP_TIMER) Display.OLED_Timer = KEYDISP_TIMER;
}
void UpdateInfoQue(uint8_t UpDown){
    #define MAXLINE 9
    #define MINLINE 1
   if (UpDown == DOWNROLL){    // down menu
     DispRollIndex[3] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[0];
     DispRollIndex[0]++;
     if (DispRollIndex[0]  > MAXLINE) DispRollIndex[0] = MINLINE;
  } 
  if (UpDown == UPROLL){   
     DispRollIndex[0] = DispRollIndex[1];
     DispRollIndex[1] = DispRollIndex[2];
     DispRollIndex[2] = DispRollIndex[3];
     DispRollIndex[3]--;  
     if (DispRollIndex[3]  < MINLINE) DispRollIndex[3] = MAXLINE; 
   } 
}
void UpdateDispRoll(uint8_t UpDown){
 
    if(Display.SensorRollTimer){
      Display.SensorRollTimer--;
      return;
    }

    UpdateInfoQue(UpDown);

}
void KeySensonsorRoll(uint8_t UpDown){
      Display.SensorRollTimer = 30; // 2sec x 30 = 60 sec
      UpdateInfoQue(UpDown);      
}
void DispEnable(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  else   Display.SleepEnable = OFF;    // no sleep
}
void DispEnable_4SD_Prblm(bool Enable, uint8_t Timer) {
  if (Enable == ON) {
    //Display.SleepEnable = ON; //go sleep
    Display.OLED_Timer = Timer;
  }
  //else   Display.SleepEnable = OFF;    // no sleep
}

void SetResetLog(bool Enable) {
    if(Enable == ON){
      SDCard.LogEnable = ON;
      NVRam_Write_LogStatus(ON); 
    }
    if(Enable == OFF){
      SDCard.LogEnable = OFF;
      NVRam_Write_LogStatus(OFF);
    } 
}
void NVRam_Read_QueNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    File_Que[0] = (char)EEPROM.read(NVRAM_QUE1);
    File_Que[1] = (char)EEPROM.read(NVRAM_QUE2);

  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    File_Que[0] = (char)dueFlashStorage.read(NVRAM_QUE1);
    File_Que[1] = (char)dueFlashStorage.read(NVRAM_QUE2);
   #endif
   Serial.print(F("File_Que: ")) ;
   Serial.println(File_Que);  
}


void NVRam_Write_QueNo(char* p) {
  Serial.print(F("File_Que:"));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_QUE1, p[0]);// high byte
          EEPROM.write(NVRAM_QUE2, p[1]);// high byte                         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_QUE1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_QUE2, p[1]);// high byte                
      
    #endif
         Serial.println(p[0]);Serial.println(p[1]);
}
void NVRam_Read_SerNo() {
   #ifdef ARDUINO_MEGA // 8 bit AVR 
    Device_Id[0] = (char)EEPROM.read(NVRAM_ID1);
    Device_Id[1] = (char)EEPROM.read(NVRAM_ID2);
    Device_Id[2] = (char)EEPROM.read(NVRAM_ID3);
    Device_Id[3] = (char)EEPROM.read(NVRAM_ID4);
  #endif
   #ifdef ARDUINO_DUE // 8 bit AVR
    Device_Id[0] = (char)dueFlashStorage.read(NVRAM_ID1);
    Device_Id[1] = (char)dueFlashStorage.read(NVRAM_ID2);
    Device_Id[2] = (char)dueFlashStorage.read(NVRAM_ID3);
    Device_Id[3] = (char)dueFlashStorage.read(NVRAM_ID4);
   #endif
   Serial.print(F("Device_Id: ")) ;
   Serial.println(Device_Id);  
}
void NVRam_Write_SerNo(char* p) {
  Serial.print(F("EECode:"));
  #ifdef ARDUINO_MEGA // 8 bit AVR
          EEPROM.write(NVRAM_ID1, p[0]);// high byte
          EEPROM.write(NVRAM_ID2, p[1]);// high byte                 
          EEPROM.write(NVRAM_ID3, p[2]);// high byte         
          EEPROM.write(NVRAM_ID4, p[3]);// high byte         
    #endif
    #ifdef ARDUINO_DUE
          dueFlashStorage.write(NVRAM_ID1, p[0]);// high byte
          dueFlashStorage.write(NVRAM_ID2, p[1]);// high byte                
          dueFlashStorage.write(NVRAM_ID3, p[2]);// high byte        
          dueFlashStorage.write(NVRAM_ID4, p[3]);// high byte         
    #endif
         Serial.println(p[0]);Serial.println(p[1]);Serial.println(p[2]);Serial.println(p[3]);
}
void NVRam_Write_LogStatus(bool Mode) { 
    #ifdef ARDUINO_MEGA
      if (Mode == OFF)EEPROM.write(ADDRES_LOG, OFF); // OFF
      else EEPROM.write(ADDRES_LOG, SampleTime);// ON
    #endif
    #ifdef ARDUINO_DUE
      if (Mode == OFF)dueFlashStorage.write(ADDRES_LOG, OFF); // OFF
      else dueFlashStorage.write(ADDRES_LOG, SampleTime);// ON
    #endif    
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif 
      
}
void NVRam_Write_Standbye(bool Mode) {
    #ifdef ARDUINO_MEGA
      if (Mode == OFF)EEPROM.write(SLEEP_LOG, OFF); // OFF
      else EEPROM.write(SLEEP_LOG, ON);// ON
    #endif
    #ifdef ARDUINO_DUE
      if (Mode == OFF)dueFlashStorage.write(SLEEP_LOG, OFF); // OFF
      else dueFlashStorage.write(SLEEP_LOG, ON);// ON
    #endif 
    #ifdef CHIPKIT_MAX32 // 32 bit ARM
      Mode = 0;
    #endif       
}

void NVRam_Read_Standbye(void) {
    #ifdef ARDUINO_MEGA
        uint8_t Read = EEPROM.read(SLEEP_LOG);// OFF
    #endif
     #ifdef ARDUINO_DUE
      uint8_t Read =dueFlashStorage.read(SLEEP_LOG);
    #endif
     #ifdef ARDUINO_MKRZERO
        uint8_t Read = 0;  
      #endif
       #ifdef CHIPKIT_MAX32 // 32 bit ARM
       uint8_t Read = 0;
    #endif 
      if(Read == OFF)DispEnable(OFF,0);
      if(Read == ON)DispEnable(ON,100); // default 
}
void NVRam_Write_SampleTime(byte Sample) {
  #ifdef ARDUINO_MEGA
      EEPROM.write(ADDRES_LOG, Sample);// ON
  #endif
     #ifdef ARDUINO_DUE 
    dueFlashStorage.write(ADDRES_LOG,Sample); 
  #endif
     #ifdef CHIPKIT_MAX32 // 32 bit ARM
       
    #endif  
}
void NVRam_Read_SampleTime(void) {
   #ifdef ARDUINO_MEGA // 8 bit AVR
    uint8_t Mode = EEPROM.read(ADDRES_LOG);// OFF
    #endif
   #ifdef ARDUINO_DUE // 32 bit ARM
       uint8_t Mode = dueFlashStorage.read(ADDRES_LOG);
    #endif
     #ifdef ARDUINO_MKRZERO
        uint8_t Mode = 0;  
      #endif    
   #ifdef CHIPKIT_MAX32 // 32 bit ARM
       uint8_t Mode = 0;
    #endif    
    if((Mode== TASK_500MSEC)||(Mode== TASK_1SEC)||(Mode== TASK_2SEC)||(Mode== TASK_5SEC)||(Mode== TASK_10SEC)||(Mode== TASK_20SEC)||(Mode== TASK_60SEC)){
      SDCard.LogEnable = ON; 
      SampleTime =  Mode;
    }
    else{
      //SDCard.LogEnable = OFF; 
      SampleTime =  TASK_2SEC; // default
    }       
    /*
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
  */
}

void UpdateLogFileNo(void){
  uint16_t No = (File_Que[0]-48)*10;
  No += (File_Que[1]-48);

  Serial.print(F("    No: "));
  Serial.println(No);
  if(No >= 100){
      File_Que[0] =  '0';
      File_Que[1] =  '1';  
      NVRam_Write_QueNo(&File_Que[0]);
  }
}

void UpdateLogFileId(void){
    LOG_FILE[1] = Device_Id[0];
    LOG_FILE[2] = Device_Id[1];
    LOG_FILE[3] = Device_Id[2];
    LOG_FILE[4] = Device_Id[3];
    
    LOG_FILE[6] =  File_Que[0];   
    LOG_FILE[7] =  File_Que[1];  
    
    Serial.print(F("LOG_FILE: ")) ;
    Serial.println(LOG_FILE);
}
void ResetCasePrint() {
#ifdef ARDUINO_MEGA // 8 bit AVR 
    
  uint8_t RESET_CASE = MCUSR;
  Serial.print(F("MCUSR: "));
  Serial.println(RESET_CASE);
  if (MCUSR & WDRF) {
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  else if (MCUSR & BORF) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  else if (MCUSR & EXTRF) {
    Serial.print(F("Rebooting from an External Reset"));
  }
  else if (MCUSR & PORF) {
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  if (MCUSR & (1 << WDRF)) {
    // a watchdog reset occurred
    Serial.print(F("Rebooting from a Watchdog Reset"));
  }
  if (MCUSR & (1 << BORF)) {
    Serial.print(F("Rebooting from a Brown-out Reset"));
  }
  //  a brownout reset occurred
  if (MCUSR & (1 << EXTRF)) {
    //  an external reset occurred
    Serial.print(F("Rebooting from an External Reset"));
  }
  if (MCUSR & (1 << PORF)) {
    //  a power on reset occurred
    Serial.print(F("Rebooting from a Power On Reset"));
  }
  //Clear register
  MCUSR = 0x00;

    #endif
}
#ifdef KEY_ANALOG
void Key_Functions_Analog(uint32_t Adc) {
    Key.Adc = Adc;
    #ifdef ARDUINO_DUE // 
      Key.Adc >>= 2; 
      if(Key.Adc < 990) {
        Key.Adc *= 66;
        Key.Adc /= 100;        
      }
    #endif
  

    Key.Down_Press = OFF;
    Key.Right_Press = OFF;
    Key.Up_Press = OFF;
    Key.Left_Press = OFF;
    if((Key.Adc < 404) &&(Key.Adc > 384)){
      Key.Released = ON;
      Key.Error = OFF;
      return;  
    }
    else if((Key.Adc < 110) &&(Key.Adc > 84))Key.Right_Press = ON;
    else if((Key.Adc < 358) &&(Key.Adc > 334))Key.Up_Press = ON;
    else if((Key.Adc < 588) &&(Key.Adc > 564))Key.Down_Press = ON;
    else if((Key.Adc < 478) &&(Key.Adc > 440))Key.Left_Press = ON;   
    else  {
      Key.Error = ON;
      return;
    }
    Key.Error = OFF;
  if (!Key.Released)return;
  Key.Released = OFF;
  DisplayWakeUp();

  if (Key.Left_Press) EnterMenuKey();
  if (Key.Down_Press) DownMenuKey();
  if (Key.Right_Press) EscMenuKey();
  if (Key.Up_Press)   UpMenuKey();
    KeyTimeOutStart();
}
#endif
#ifdef KEY_DIGITAL
void Key_Functions_Digital(void) {

  if (!Key.Left_Rel && !Key.Down_Rel && !Key.Right_Rel&& !Key.Up_Rel) {
    Key.Released = ON;
  }
  if (Key.Released && !digitalRead(KEY_LEFT)) {
    Key.Left_Rel = ON;
    return;
  }
  if (Key.Left_Rel  &&  digitalRead(KEY_LEFT)) {
    Key.Left_Rel = OFF;
    DisplayWakeUp();
    EnterMenuKey();
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_DOWN)) {
    Key.Down_Rel = ON;
    return;
  }
  if (Key.Down_Rel  &&  digitalRead(KEY_DOWN)) {
    Key.Down_Rel = OFF;
    DisplayWakeUp();
    DownMenuKey(); 
    KeyTimeOutStart();
    return;
  }
  if (Key.Released && !digitalRead(KEY_UP)) {
    Key.Up_Rel = ON;
    return;
  }
  if (Key.Up_Rel  &&  digitalRead(KEY_UP)) {
    Key.Up_Rel = OFF;
    DisplayWakeUp();
    UpMenuKey();   
    KeyTimeOutStart();
    return;
  }
  
  if (Key.Released && !digitalRead(KEY_RIGHT)) {
    Key.Right_Rel = ON;
    return;
  }
  if (Key.Right_Rel  &&  digitalRead(KEY_RIGHT)) {
    DisplayWakeUp();
    EscMenuKey();
    KeyTimeOutStart();
    return;
  }
}
#endif
void DownMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL :// Menu = MENU_NULL;
            KeySensonsorRoll(DOWNROLL);
      break;
    case MENU1 : MainMenu = MENU6; //
      break;
    case MENU2 : MainMenu = MENU1;
      break;
    case MENU3 : MainMenu = MENU2;
      break;
    case MENU4 : MainMenu = MENU3;
      break;
     case MENU5 : MainMenu = MENU4;
      break;  
     case MENU6 : MainMenu = MENU5;
      break;     
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB1 :  MainMenu =  MENU2_SUB7;//
      break;
    case MENU2_SUB2 :  MainMenu =  MENU2_SUB1;//
      break;
    case MENU2_SUB3 :  MainMenu =  MENU2_SUB2;//
      break;
    case MENU2_SUB4 :  MainMenu =  MENU2_SUB3;//
      break;
    case MENU2_SUB5 :  MainMenu =  MENU2_SUB4;//
      break;
    case MENU2_SUB6 :  MainMenu =  MENU2_SUB5;//
      break;
    case MENU2_SUB7 :  MainMenu =  MENU2_SUB6;//
      break;
   case MENU2_SUB8 :  
      break;
      
    case MENU5_SUB1 :  
        DateTimeBuf.Year--;
        if (DateTimeBuf.Year < 2021)DateTimeBuf.Year = 2040;
      break;
    case MENU5_SUB2 :
         DateTimeBuf.Month--;
        if (DateTimeBuf.Month < 1)DateTimeBuf.Month = 12;
      break;
    case MENU5_SUB3 :  
        DateTimeBuf.Day --; 
        if (DateTimeBuf.Day < 1)DateTimeBuf.Day = 31;
      break;
    case MENU5_SUB4 :
        DateTimeBuf.Hour --;
        if (DateTimeBuf.Hour < 1)DateTimeBuf.Hour = 23;  
      break;
    case MENU5_SUB5 :  
        DateTimeBuf.Minute --;
        if (DateTimeBuf.Minute < 1)DateTimeBuf.Minute = 59;  
      break;
    case MENU5_SUB6 :  
         DateTimeBuf.Second --;
        if (DateTimeBuf.Second < 1)DateTimeBuf.Second = 59;    
      break;

      
     case MENU5_SUB7 :      
      break;
    case MENU6_SUB1:  //     
      break;
    case MENU6_SUB2:  //       
      break;        
    case MENU6_SUB3:      
      break;
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;
    default: MainMenu = MENU_NULL;
     break;

  }
}

void UpMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
          KeySensonsorRoll(UPROLL);
      break;
    case MENU1 : MainMenu = MENU2; //
      break;
    case MENU2 : MainMenu = MENU3;
      break;
    case MENU3 : MainMenu = MENU4;
      break;
    case MENU4 : MainMenu = MENU5;
      break;
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4_SUB1; 
      break;
       
     case MENU5 : MainMenu = MENU6;
      break;     
     case MENU6 : MainMenu = MENU1;
      break; 
    case MENU1_SUB1 :  MainMenu =  MENU1_SUB2;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1_SUB1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB1 :  MainMenu =  MENU2_SUB2;//
      break;
    case MENU2_SUB2 :  MainMenu =  MENU2_SUB3;//
      break;
    case MENU2_SUB3 :  MainMenu =  MENU2_SUB4;//
      break;
    case MENU2_SUB4 :  MainMenu =  MENU2_SUB5;//
      break;
    case MENU2_SUB5 :  MainMenu =  MENU2_SUB6;//
      break;
    case MENU2_SUB6 :  MainMenu =  MENU2_SUB7;//
      break;
    case MENU2_SUB7 :  MainMenu =  MENU2_SUB1;//
      break;
    case MENU2_SUB8 :  
      break;
    case MENU3_SUB1 :  MainMenu =  MENU3_SUB2;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3_SUB1;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;
    case MENU5_SUB1 :  
        DateTimeBuf.Year++;
        if (DateTimeBuf.Year > 2040)DateTimeBuf.Year = 2021;
      //  if (DateTimeBuf.Year < 2020)DateTimeBuf.Year = 2020;
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
    default: MainMenu = MENU_NULL;
      break;
  }
}


void EscMenuKey(void) {   
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU_NULL;
      break;
    case MENU1 : MainMenu = MENU_NULL; //
      break;
    case MENU2 : MainMenu = MENU_NULL;
      break;
    case MENU3 : MainMenu = MENU_NULL;
      break;
    case MENU4 : MainMenu = MENU_NULL;
      break;
    case MENU4_SUB1 :   MainMenu = MENU4; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4; 
      break;   
    case MENU4_SUB3 :   MainMenu = MENU4; 
      break;           
    case MENU5 : MainMenu = MENU_NULL;
      break;
    case MENU1_SUB1 :  MainMenu =  MENU1;//
      break;
    case MENU1_SUB2 : MainMenu =  MENU1;//
      break;
    case MENU1_SUB3 :       
      break;
    case MENU1_SUB4 :     
      break;
    case MENU2_SUB1 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB2 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB3 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB4 : MainMenu =  MENU2;//
      break;
    case MENU2_SUB5 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB6 :  MainMenu =  MENU2;//
      break;
    case MENU2_SUB7 :  MainMenu =  MENU2;//
      break;
      case MENU2_SUB8 : 
      break;
    case MENU3_SUB1 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB2 :  MainMenu =  MENU3;//
      break;
    case MENU3_SUB3 : 
      break;
    case MENU3_SUB4 : 
      break;
    case MENU5_SUB1 :  MainMenu = MENU5;
      break;
    case MENU5_SUB2 :  MainMenu = MENU5_SUB1;
      break;
    case MENU5_SUB3 :  MainMenu = MENU5_SUB2;
      break;
    case MENU5_SUB4 :  MainMenu = MENU5_SUB3;
      break;
    case MENU5_SUB5 :  MainMenu = MENU5_SUB4;
      break;
    case MENU5_SUB6 :  MainMenu = MENU5_SUB5;
      break;
     case MENU5_SUB7 :      
      break;
       case MENU6:   MainMenu = MENU_NULL; //     
      break;
       case MENU6_SUB1: MainMenu = MENU6; //     
      break;
      case MENU6_SUB2: MainMenu = MENU6; //       
      break;        
       case MENU6_SUB3:      
      break;     
    default: MainMenu = MENU_NULL;
    break;
  }
}
void EnterMenuKey(void) {
  if (Display.OLED_Timer == 0) return;
  DispExtTimeout();
  switch (MainMenu) {
    case MENU_NULL : MainMenu = MENU1;
          break;
    case MENU1 : //Menu = MENU1MIN; // go to sub menu  // sd kart log on
      if (SDCard.LogEnable == ON) MainMenu = MENU1_SUB2; //already logging
      else  MainMenu = MENU1_SUB1;
      break;
    case MENU1_SUB1 :  SetResetLog(ON); //SDCard.LogEnable = ON; EESetResetLog(ON);
      MainMenu =  MENU1_SUB3;//MENU1
      break;
    case MENU1_SUB2 : SetResetLog(OFF);// SDCard.LogEnable = OFF; EESetResetLog(OFF); // default
      MainMenu =  MENU1_SUB4;//MENU1
      break;
    case MENU1_SUB3 :  
      break;
    case MENU1_SUB4 :   
      break;
     case MENU2 : // Menu = MENU2MIN; // call the right menu according to current one
      switch (SampleTime) {
        case TASK_500MSEC: MainMenu = MENU2_SUB1;
          break;
        case TASK_1SEC : MainMenu = MENU2_SUB2;
          break;
        case TASK_2SEC : MainMenu = MENU2_SUB3;
          break;
        case TASK_5SEC : MainMenu = MENU2_SUB4;
          break;
        case TASK_10SEC : MainMenu = MENU2_SUB5;
          break;
        case TASK_20SEC : MainMenu = MENU2_SUB6;
          break;
        case TASK_60SEC : MainMenu = MENU2_SUB7;
          break;
        default:
          break;
      }
      break;   
    case MENU2_SUB1 :  SampleTime = TASK_500MSEC ; NVRam_Write_SampleTime(TASK_500MSEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB2 : SampleTime = TASK_1SEC; NVRam_Write_SampleTime(TASK_1SEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB3 :  SampleTime = TASK_2SEC; NVRam_Write_SampleTime(TASK_2SEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB4 : SampleTime = TASK_5SEC; NVRam_Write_SampleTime(TASK_5SEC); // default
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB5 :  SampleTime = TASK_10SEC; NVRam_Write_SampleTime(TASK_10SEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB6 :  SampleTime = TASK_20SEC; NVRam_Write_SampleTime(TASK_20SEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB7 :  SampleTime = TASK_60SEC; NVRam_Write_SampleTime(TASK_60SEC);
      MainMenu =  MENU2_SUB8;//MENU2;//
      break;
    case MENU2_SUB8 :  
      
      break;
      case MENU3 : // Menu = MENU3MIN;
      if (Display.SleepEnable == ON) MainMenu = MENU3_SUB2; //already logging
      else  MainMenu = MENU3_SUB1;
      break;    
    case MENU3_SUB1 :  DispEnable(ON,40);
      NVRam_Write_Standbye(ON);
      MainMenu =  MENU3_SUB3;//MENU3
      break;
    case MENU3_SUB2 :  DispEnable(OFF,0);
      NVRam_Write_Standbye(OFF); 
      MainMenu =  MENU3_SUB4;//MENU3
      break;
    case MENU3_SUB3 :  
      
      break;
    case MENU3_SUB4 :     
      break;   
    case MENU4 :   MainMenu = MENU4_SUB1;       
      break;
    case MENU4_SUB1 :   MainMenu = MENU4_SUB2; 
      break;  
    case MENU4_SUB2 :   MainMenu = MENU4_SUB3; 
      break;    
    case MENU4_SUB3 :   MainMenu = MENU4_SUB1; 
      break;         
    case MENU5 :        MainMenu = MENU5_SUB1;
        DateTimeBuf.Init = ON;       
      break;
     case MENU5_SUB1 :  MainMenu = MENU5_SUB2;   //Year  
      break;     
     case MENU5_SUB2 :  MainMenu = MENU5_SUB3; // Month
      break;        
     case MENU5_SUB3 :  MainMenu = MENU5_SUB4; // Day
      break;  
     case MENU5_SUB4 :  MainMenu = MENU5_SUB5; // Hour
      break;  
     case MENU5_SUB5 :  MainMenu = MENU5_SUB6; // Minute
      break;
     case MENU5_SUB6 :  MainMenu = MENU5_SUB7; //Second
          DateTimeBuf.RTC_Update = ON;    
      break;
     case MENU5_SUB7 :      
      break;
     case MENU6:   MainMenu = MENU6_SUB1; //     
      break;
       case MENU6_SUB1: MainMenu = MENU6_SUB2; //     
      break;
      case MENU6_SUB2: MainMenu = MENU6_SUB3; //
        #ifdef ENERGYMETER_EXISTS
          EnergyMeterIC.Mode = POWERIC_CALB1; 
        #endif      
      break;        
       case MENU6_SUB3:      
      break;  
    default: MainMenu = MENU_NULL; 
    break;
  }
}
