//https://www.arduino.cc/en/reference/SPI
//https://learn.adafruit.com/adafruit-shield-compatibility/data-logging-shield-assembled
  
// https://learn.adafruit.com/adafruit-data-logger-shield/overview

//https://cdn-learn.adafruit.com/assets/assets/000/066/497/original/adafruit_products_schem.png?1543358466'
/*
  SD card test
   
  This example shows how use the utility libraries on which the'
  SD library is based in order to get info about your SD card.
  Very useful for testing a card when you're not sure whether its working or not.

  The circuit:
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
     Pin 4 used here for consistency with other Arduino examples

  created  28 Mar 2011
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe
*/
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN

//const int chipSelect = 53; // mega old
// const int chipSelect = 10; // mega
//const int chipSelect = 10; // mega CAN BE ANY PIN SINCE ITS SLAVE
// set up variables using the SD utility library functions:

#ifdef SDCARD_EXISTS

Sd2Card card;
SdVolume volume;
SdFile root;



    
void GetFileSize(){
    File dataFile = SD.open(LOG_FILE, FILE_WRITE);
    if (dataFile) {
      FileSize.Total = dataFile.size();  
      dataFile.close(); 
    }      
    else  FileSize.Total = 0;
}
void ReadConfigFile(){
  // File Config =  SD.open(ConfigFile);
   File Config =  SD.open(CopyFlashToRam(ConfigFile));
   
   Config_Str = "";
   if (Config) {
      Serial.print(F("Config File "));
      Serial.println(ConfigFile);  
      while (Config.available()) {
        Config_Str += (char)(Config.read());
      }
      Config.close();
      Serial.print(F("File Content: "));Serial.println(Config_Str);  
      Parse_FileString();
   }
   else{
      Serial.print(F("error opening")); Serial.println(ConfigFile);    
   }
}
void UpdateFileQue(){
    uint16_t Que= (LOG_FILE[6]-48) *10;
    Que += LOG_FILE[7]-48;
  //  Serial.print(F("Que ")); Serial.println(Que); 
    #define MAXLOGFILESIZE 1048576 //2048
    if(FileSize.Total > MAXLOGFILESIZE){ // 1Mbyte 
       if(Que<100){
          Que++;
          LOG_FILE[6] =  (Que/10)+48;   
          LOG_FILE[7] =  (Que%10)+48;
          File_Que[0] =  LOG_FILE[6];
          File_Que[1] =  LOG_FILE[7];
          NVRam_Write_QueNo(&File_Que[0]);
       //   Serial.print(F("NVRAM_QUE1 "));Serial.println( EEPROM.read(NVRAM_QUE1)); 
      //    Serial.print(F("NVRAM_QUE2 "));Serial.println( EEPROM.read(NVRAM_QUE2)); 
       }       
    } 
  //  Serial.print(F("LOG_FILE6.7 ")); Serial.print(LOG_FILE[6]);Serial.print(LOG_FILE[7]);
}
void SD_CardLogTask(){ 
 //   Serial.print(F("SDCard.Status ")); Serial.println(SDCard.Status); 
//    Serial.print(F("SDCard.FatError ")); Serial.println(SDCard.FatError);  
 //   Serial.print(F("Display.SleepEnable ")); Serial.println(Display.SleepEnable); 
 //   Serial.print(F("Display.OLED_Timer ")); Serial.println(Display.OLED_Timer); 
  if(!SDCard.LogEnable){ // log on of  from menu
      SDCard.LogBootInit = 0; // In order to start with the header next time
      return;
  }
   if(SDCard.PauseTimer){  
      return;   // sd card reread trial timer 
   }   
    if(SDCard.LogBootInit == OFF){
      SDCard.LogBootInit = ON;
      // put header + data           
         SD_Card_Info(); // card type and fat info
         SD_Card_Init();            
         dataString ="";
         SD_Card_Header_Preparation();dataString += "\n";         
          SD_Card_Data_Preparation();               
    }
    else{ // put  only data          
        SD_Card_Init();                  
        dataString ="";  
        SD_Card_Data_Preparation(); 
        SDCard.PauseCount = OFF;                                 
    } 
    File dataFile = SD.open(LOG_FILE, FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      FileSize.Total = dataFile.size();  
          //  Serial.println(dataFile.position()); // 2021.03.09 
          //  Serial.println(FileSize.Total); // 2021.03.09 
      dataFile.close();
      // print to the serial port too:
      Serial.print(F("Data2 csv File:    "));
      Serial.println(dataString);  
      UpdateFileQue(); 
    }      
  else {  
      Serial.print(F("error opening : ")); 
      Serial.println(LOG_FILE);    
      FileSize.Total = 0;
    }   
}
void SD_FAT_Check(void){
  Serial.println(F("FatCheck Start")); 
  #ifdef ARDUINO_DUE 
  Print_ARM_SPI_Regs();
  #endif
    if (!volume.init(card)) {
         Serial.println(F("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card"));      
         SDCard.Status = SD_NOT_Present; 
         SDCard.FatError = ON;       
    }
    else {        
      Serial.print(F("Clusters:          "));
      Serial.println(volume.clusterCount());
      Serial.print(F("Blocks x Cluster:  "));
      Serial.println(volume.blocksPerCluster());
      Serial.print(F("Total Blocks:      "));
      Serial.println(volume.blocksPerCluster() * volume.clusterCount());
      Serial.println();
      // print the type and size of the first FAT-type volume
      uint32_t volumesize;
      Serial.print(F("Volume type is:    FAT"));
      Serial.println(volume.fatType(), DEC);
      volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
      volumesize *= volume.clusterCount();       // we'll have a lot of clusters
      volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print(F("Volume size (Kb):  "));
    Serial.println(volumesize);
    Serial.print(F("Volume size (Mb):  "));
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print(F("Volume size (Gb):  "));
    SDCard.Volume = (float)volumesize / 1024.0;
    Serial.println(SDCard.Volume);    
    //Serial.println((float)volumesize / 1024.0);
    Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
    root.openRoot(volume);
    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
    } 
  Serial.println(F("FatCheck End"));
  #ifdef ARDUINO_DUE 
  Print_ARM_SPI_Regs(); 
  #endif
}

void SD_Card_Info(void){
  SDCard.Status = SD_NOT_Present;
  SDCard.FatError = OFF;
  #ifdef ARDUINO_DUE 
  Print_ARM_SPI_Regs();
  #endif
  
  Serial.print(F("\nInitializing SD card..."));
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
#ifdef ARDUINO_MKRZERO 
  if (!card.init(SPI_HALF_SPEED, SDCARD_SS_PIN)) {  // SPI_QUARTER_SPEED  // Sd2Card.h
#endif
#if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 
  if (!card.init(SPI_HALF_SPEED, SD_CS_PINOUT)) {  // SPI_QUARTER_SPEED  // Sd2Card.h
#endif      
 // if (!card.init(SPI_QUARTER_SPEED, SD_CS_PINOUT)) {  //   // Sd2Card.h
    Serial.println(F("initialization failed:"));
   // Serial.println(F("* is a card inserted?"));
   // Serial.println(F("* is your wiring correct?"));
   // Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
  } 
  else {
    //Serial.println("Wiring is correct and a card is present."); 
    // print the type of card
    Serial.println();
    Serial.print(F("Card type:         "));
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println(F("SD1"));
        SDCard.Status = SD1_TYPE;
      break;
      case SD_CARD_TYPE_SD2:
        Serial.println(F("SD2"));
        SDCard.Status = SD2_TYPE;
      break;
      case SD_CARD_TYPE_SDHC:
        Serial.println(F("SDHC"));
        SDCard.Status = SDHC_TYPE;
      break;
      default:
        SDCard.Status = UNKNOWN_TYPE;
        Serial.println(F("Unknown"));
    }
    SD_FAT_Check();
  }
    Serial.println(F("Card Info End"));
    
    #ifdef ARDUINO_DUE 
    Print_ARM_SPI_Regs();
    #endif
}

void SD_Card_Init(){
      Serial.println(F("SD_Card_Init Start"));

#ifdef ARDUINO_DUE 
   Print_ARM_SPI_Regs();
#endif
      
#ifdef ARDUINO_MKRZERO 
      if (!SD.begin(SDCARD_SS_PIN)) {
#endif
#if defined (ARDUINO_MEGA)  | defined (ARDUINO_DUE) 
  if (!SD.begin(SD_CS_PINOUT)) {
#endif   
  
    Serial.println(F("Card failed, or not present"));
      SDCard.Status = SD_NOT_Present;
      switch(SDCard.PauseCount){
          case 0: SDCard.PauseTimer = 10;SDCard.PauseCount++;
          break;
           case 1: SDCard.PauseTimer = 12;SDCard.PauseCount++;
          break;
           case 2: SDCard.PauseTimer = 14;SDCard.PauseCount++;
          break;
           case 3: SDCard.PauseTimer = 16;SDCard.PauseCount++;
          break;
          case 4: SDCard.PauseTimer = 18;SDCard.PauseCount++;
          break;
           case 5: SDCard.PauseTimer = 20;SDCard.PauseCount++;
          break;
          case 6: SDCard.PauseTimer = 22;SDCard.PauseCount++;
          break;
           case 7: SDCard.PauseTimer = 24;SDCard.PauseCount++;
          break;
          case 8: SDCard.PauseTimer = 26;SDCard.PauseCount++;
          break;
           case 9: SDCard.PauseTimer = 28;SDCard.PauseCount++;
          break;   
           case 10: SDCard.PauseTimer = 30;SDCard.PauseCount++;
          break; 
           case 11: SDCard.PauseTimer = 32;SDCard.PauseCount++;
          break;
          case 12: SDCard.PauseTimer = 34;SDCard.PauseCount++;
          break;
           case 13: SDCard.PauseTimer = 36;SDCard.PauseCount++;
          break;
          case 14: SDCard.PauseTimer = 38;SDCard.PauseCount++;
          break;
           case 15: SDCard.PauseTimer = 40;SDCard.PauseCount++;
          break;   
           case 16: SDCard.PauseTimer = 42;SDCard.PauseCount++;
          break;      
          default: SDCard.PauseTimer = 50;
          break;                                           
      }
      DispEnable_4SD_Prblm(ON,(60-SDCard.PauseTimer));
      //SDCard.PauseTimer = 15;
      SDCard.LogBootInit = OFF;     
  }
  //SDCard.Status = SD_NOT_Present;
        Serial.println(F("SD_Card_Init Start"));
#ifdef   ARDUINO_DUE     
      Print_ARM_SPI_Regs();
#endif
}
void SD_Card_Header_Preparation(){
      //char* p =(char*)&SD_CARD_ERR[0]; 
      char* p =NULL;   
      switch(SDCard.Status){
        case SD1_TYPE :  p= (char*)SD1_CARD; 
        break;
        case SD2_TYPE : p=(char*) SD2_CARD;
        break;
        case SDHC_TYPE :p=(char*) SDHC_CARD;
        break;
      //  case SD_NO_FAT :p= NO_FAT;
     //   break;
        case SD_NOT_Present :
        default:
              p=(char*) SD_CARD_ERR;
        break;     
    }  
          //    dataString = "Year,Month,Date,Hour,Min,Sec,WindRaw,velReading,WindMPH,WindTemp,TemperatureSi072,Humidity,Pressure(hPa),";
        //    dataString += "TemperatureBMP,Altitude(m),Luminosity,Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z)";  
   
       dataString += F("FirmVers ");   
       dataString += String(__DATE__) + ' ' + String(__TIME__);
       dataString += F(",Dev_Id:") ;
       dataString += String(Device_Id) ; 
       dataString +=  F(",SD Type: ") ; 
       dataString +=  String(p);
       dataString +=  F(",Volume: ");
       dataString +=  String(SDCard.Volume);
       dataString += F(" GB,Found Sensors Id's:,") ;        
       dataString += String(SensorId.No1, HEX) +  ',' +  String(SensorId.No2, HEX) + ','  + String(SensorId.No3, HEX) + ',' ;
       
       #ifdef PM25_DUST_SENSOR_EXISTS 
         // dataString += "PMsensor Rev"  + ',' +  Sensor_Info_SDS  +  ',';
      #endif 
        dataString += "\n" ;
        dataString +=  F("Year,Month,Date,Hour,Min,Sec,");
      #ifdef WIND_SENSOR_EXISTS  
        dataString += F("WindRaw,velReading,WindMPH,WindTemp,");
      #endif  
      #ifdef TEMP_HUM_1_SENSOR_EXISTS 
       // dataString += "TemperatureSi072_1,Humidity_1,";
        dataString += F("Temp1,Hum1(");
        dataString += String(SensorId.No1, HEX);
        dataString += F("),");
      #endif
      #ifdef TEMP_HUM_2_SENSOR_EXISTS 
       // dataString += "TemperatureSi072_2,Humidity_2,";
        dataString += F("Temp2,Hum2(");
        dataString += String(SensorId.No2, HEX);
        dataString += F("),");
        
      #endif
       #ifdef TEMP_HUM_3_SENSOR_EXISTS 
      //  dataString += "TemperatureSi072_3,Humidity_3,";
        dataString += F("Temp3,Hum3(");
        dataString += String(SensorId.No3, HEX);
        dataString += F("),");     
      #endif     
      #ifdef LEM_CURRENT_EXISTS 
        dataString += F("Current(A)rms,");
      #endif   
      #ifdef VOLTAGE_MEASURE_EXISTS 
        dataString += F("Voltage(V),");
      #endif          
      #ifdef BAR_PRES_SENSOR_EXISTS
        dataString += F("Pressure(hPa),TemperatureBMP,Altitude(m),");
      #endif
      #ifdef LIGHT_SENSOR_EXISTS
         dataString += F("Luminosity,");
      #endif
      #ifdef   ACCL_GYRO_SENSOR_EXISTS       
          dataString += F("Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z),"); 
      #endif 
       #ifdef PM25_DUST_SENSOR_EXISTS 
          dataString += F("PM2.5,PM10,"); 
      #endif  
       #ifdef ENERGYMETER_EXISTS 
          dataString += F("(A)rms,(V)rms,Power(W),PF,Freq.,"); 
      #endif  
        #ifdef PROGRELAY_EXISTS 
      dataString += F("RL1,RL2");
       #endif             
}
void SD_Card_Data_Preparation(){
      dataString += Str_DispTime;     
          /*        
          dataString += String(Values.WindRaw) + ',' + String(velReading)+ ',' + String(Values.WindTemp) + ',' +String(Values.WindMPH)+ ','       
          + String(Values.TemperatureSi072)+ ',' + String(Values.Humidity)+ ','
          + String(Values.Pressure)+ ',' + String(Values.TemperatureBMP) + ',' + String(Values.Altitude)+ ','
          + String(Values.Luminosity) +','
          + String(Accelometer.x) + ',' + String(Accelometer.y)+ ','+ String(Accelometer.z) + ',' + String(Gyro.x) + ',' + String(Gyro.y)+ ','+ String(Gyro.z);
          */
      #ifdef WIND_SENSOR_EXISTS  
        dataString += String(Values.WindRaw) + ',' + String(velReading)+ ',' + String(Values.WindTemp) + ',' +String(Values.WindMPH)+ ',';
      #endif
      #ifdef TEMP_HUM_1_SENSOR_EXISTS 
        dataString += String(Values.TemperatureSi072_Ch1)+ ',' + String(Values.Humidity_Ch1)+ ',';
      #endif
      #ifdef TEMP_HUM_2_SENSOR_EXISTS 
        dataString += String(Values.TemperatureSi072_Ch2)+ ',' + String(Values.Humidity_Ch2)+ ',';
      #endif
      #ifdef TEMP_HUM_3_SENSOR_EXISTS 
        dataString += String(Values.TemperatureSi072_Ch3)+ ',' + String(Values.Humidity_Ch3)+ ',';
      #endif   
      #ifdef LEM_CURRENT_EXISTS 
        dataString += String(Current_Mains_Rms) + ',';
      #endif  
      #ifdef VOLTAGE_MEASURE_EXISTS 
        dataString += String(Mains_Volt) + ',';
      #endif    
      #ifdef BAR_PRES_SENSOR_EXISTS
        dataString +=  String(Values.Pressure)+ ',' + String(Values.TemperatureBMP) + ',' + String(Values.Altitude)+ ',';
      #endif
      #ifdef LIGHT_SENSOR_EXISTS
         dataString +=  String(Values.Luminosity) +',';
      #endif
      #ifdef   ACCL_GYRO_SENSOR_EXISTS       
         dataString += String(Accelometer.x) + ',' + String(Accelometer.y)+ ','+ String(Accelometer.z) + ',' + String(Gyro.x) + ',' + String(Gyro.y)+ ','+ String(Gyro.z)+',';     
      #endif

         #ifdef PM25_DUST_SENSOR_EXISTS 
          dataString += String(Values.PM25)+ ',' + String(Values.PM10)+ ','  ; 
      #endif  

        #ifdef ENERGYMETER_EXISTS 
        /*
          if(EnergyMeterIC.Mode == POWERIC_NORMAL){ 
            dataString += String(Values.Current)+ ',' + String(Values.Voltage)+ ',';
            dataString += String(Values.ActivePower)+ ',' + String(Values.PowerFactor)+ ',';
           dataString += String(Values.Frequency)+ ',';
          }
          */
  switch(EnergyMeterIC.Mode){
    case POWERIC_SETUP1:
    case POWERIC_SETUP2:
    case POWERIC_SETUP3:dataString += F("Setup,Setup,Setup,Setup,Setup,");  
    
    break;
    case POWERIC_NORMAL:         
                dataString += String(Values.Current)+ ',' + String(Values.Voltage)+ ',';dataString += String(Values.ActivePower)+ ',' + String(Values.PowerFactor)+ ',';
                dataString += String(Values.Frequency)+ ',';
    break;
    case POWERIC_CALB1: // Calibrating The Current Channel Started            
    case POWERIC_CALB2: // Calibrating The Current Channel Continues        
    case POWERIC_CALB3: // Calibrating The Current Channel Ends
    case POWERIC_CALB4: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB5:    
    case POWERIC_CALB6: // Calibrating The Current Channel Ends
    case POWERIC_CALB7: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB8:  
    case POWERIC_CALB9: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB10: 
                dataString += F("Calib.,Calib.,Calib.,Calib.,Calib.,");     
    break;
     default:   
    break;
  }   
      #endif // endof  #ifdef ENERGYMETER_EXISTS 
      #ifdef PROGRELAY_EXISTS    
      dataString +=  String(digitalRead(RELAY_OUT_1))+ ','+ String(digitalRead(RELAY_OUT_2));  
      #endif 
}
#ifdef ARDUINO_DUE 
void Print_ARM_RegsFormat(uint32_t Reg){
      uint32_t  temp = Reg & 0xFF000000;
      Serial.print(temp >> 24,HEX);Serial.print('.'); 
      temp = Reg & 0x00FF0000;
      Serial.print(temp >> 16,HEX);Serial.print('.');       
      temp = Reg & 0x0000FF00;
      Serial.print(temp>> 8,HEX);Serial.print('.');
       temp = Reg & 0x000000FF;
      Serial.print(temp,HEX);Serial.print(':');
      Serial.println(Reg,BIN);  
}
void Print_ARM_SPI_Regs(void){
      //Serial.print(F("Total Blocks:      "));
     // uint32_t temp; uint16_t i;uint32_t Reg;
      //Serial.println(SPI0->SPI_CSR[0],BIN);      
      Serial.print(F("SPI_CSR0  "));     
      Serial.println(SPI0->SPI_CSR[0] ,HEX);
      Serial.print(F("SPI_CSR1  "));
      Serial.println(SPI0->SPI_CSR[1],HEX);
      Serial.print(F("SPI_CSR2  "));
      Serial.println(SPI0->SPI_CSR[2],HEX);     
      Serial.print(F("SPI_CSR3  "));
      Serial.println(SPI0->SPI_CSR[3],HEX);    
      //Print_ARM_RegsFormat(SPI0->SPI_CSR[3]); 
    // https://developer.arm.com/documentation/dui0417/d/programmer-s-reference/status-and-system-control-registers/pll-initialization-register--sys-pll-init
/*
      Serial.print(F("PLLADIV[24]  "));
      Serial.println(SYS_PLL_INIT->PLLADIV[24],HEX);  
       Serial.print(F("PLLADIV[24]  "));
      Serial.println(SYS_PLL_INIT->PLLADIV[25],HEX);  
      Serial.print(F("PLLADIV[24]  "));
      Serial.println(SYS_PLL_INIT->PLLADIV[26],HEX);     
*/
      Serial.println(PMC->PMC_SCER ,HEX);   
       Serial.println (REG_PMC_SCER, BIN);   

      Serial.println(PMC->PMC_MCKR ,HEX);   
       Serial.println (REG_PMC_MCKR, BIN);
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf

      Serial.println(PMC->CKGR_PLLAR ,HEX);   
       Serial.println (REG_CKGR_PLLAR, BIN);
            
      Serial.println(PMC->PMC_PCK[0] ,HEX);

  //Code Example to select divider 4 for peripheral index 43 (0x2B) and enable its clock:
    //write_register(PMC_PCR,0x1002102B)
    //REG_PMC_PCR = 0x0000002B;
       
    //Code Example to read the divider of the same peripheral:
    // write_register(PMC_PCR,0x0000002B)
   // read_register(PMC_PCR)
    //REG_PMC_PCR = 0x0000002B;
        Serial.println (PMC->PMC_PCR, BIN);   
    
       Serial.println (REG_PMC_WPMR, BIN);
       Serial.println (REG_PMC_SCSR, BIN);

      Serial.println(REG_PMC_WPMR ,HEX);        
       
//https://forum.arduino.cc/index.php?topic=218033.0             
}
#endif // endoff #ifdef ARDUINO_DUE 
#endif  //  endoff #ifdef SDCARD_EXISTS
