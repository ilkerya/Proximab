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
Sd2Card card;
SdVolume volume;
SdFile root;

//File logfile;

#define SD_NOT_Present 0
#define SD1_TYPE 1
#define SD2_TYPE 2
#define SDHC_TYPE 3
#define UNKNOWN_TYPE 4

void SD_Card_Info();
void SD_Card_Init();
void SD_Card_Data_Preparation();
void SD_Card_Header_Preparation();

void GetFileSize(){
    File dataFile = SD.open(LOG_FILE, FILE_WRITE);
    if (dataFile) {
      FileSize.Total = dataFile.size();  
      dataFile.close(); 
    }      
    else  FileSize.Total = 0;


}
   String ConfigFile= "ADConfig.txt";
   String Config_Str = "";
   
void ReadConfigFile(){
   File Config =  SD.open(ConfigFile);
   Config_Str = "";
   if (Config) {
      Serial.println(ConfigFile);  
      while (Config.available()) {
        Config_Str += (char)(Config.read());
      }
      Config.close();
      Serial.print("File Content: ");Serial.println(Config_Str);  

       Parse_FileString();
   }
   else{
      Serial.print("error opening"); Serial.println(ConfigFile);    
   }
}


void SD_CardLogTask(){
  
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
         SD_Card_Info();
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
      dataFile.close();
      // print to the serial port too:
      Serial.print("Data2 csv File:    ");
      Serial.println(dataString);  
    }      
  else {  
      Serial.print("error opening : "); 
      Serial.println(LOG_FILE);    
      FileSize.Total = 0;
    }
}


void SD_Card_Info(void){
  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, SD_CS_PINOUT)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    SDCard.Status = SD_NOT_Present;

    //while (1);
  } else {
    //Serial.println("Wiring is correct and a card is present."); 
    // print the type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        SDCard.Status = SD1_TYPE;
      break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        SDCard.Status = SD2_TYPE;
      break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        SDCard.Status = SDHC_TYPE;
      break;
      default:
        SDCard.Status = UNKNOWN_TYPE;
        Serial.println("Unknown");
    }
      // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
      if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      // while (1);
    }
  else {

    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    SD_Volume = (float)volumesize / 1024.0;
    Serial.println(SD_Volume);    
    //Serial.println((float)volumesize / 1024.0);

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);

    // list all files in the card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
    }
  }
 // File_Length();
}

void SD_Card_Init(){
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS_PINOUT)) {
    Serial.println("Card failed, or not present");
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
      DispEnable(ON,(60-SDCard.PauseTimer));
      //SDCard.PauseTimer = 15;
      SDCard.LogBootInit = OFF;
      
  }
  /*
  else
     //Serial.println("card Ready For Logging."); 
      switch(SDCard.Status){
      case 1 : SD_TypeString = SD1_CARD; //SD1
            SD_Type = ; 
      break;
      case 2 :SD_TypeString = SD2_CARD;//SD2
            SD_Type = ; 
      break;
      case 3 :SD_TypeString = SDHC_CARD;//SDHC
            SD_Type = ; 
      break; 
      case 0 :SD_TypeString = SD_CARD_ERR;//no card 
            SD_Type = ;  
      default: //SD_Card_Reset = OFF;//unknown  
      break;     
    }
*/   
}
void SD_Card_Header_Preparation(){
      char* p =&SD_CARD_ERR[0];
      
      switch(SDCard.Status){
      case 1 :  p= SD1_CARD; 
      break;
      case 2 : p= SD2_CARD;
      break;
      case 3 :p= SDHC_CARD;
      break; 
      case 0 :p= SD_CARD_ERR;
      default: //SD_Card_Reset = OFF;//unknown  
      break;     
    }
  
          //    dataString = "Year,Month,Date,Hour,Min,Sec,WindRaw,velReading,WindMPH,WindTemp,TemperatureSi072,Humidity,Pressure(hPa),";
        //    dataString += "TemperatureBMP,Altitude(m),Luminosity,Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z)";  
        
    //    dataString += "FirmVers " + FW_Version +  ",Dev_Id:" + EE_Id_EString + ',' + "SD Type: " + SD_TypeString + ',' + "Volume: " +String(SD_Volume) + " GB" + ',' +
        dataString += "FirmVers " + FW_Version +  ",Dev_Id:" + EE_Id_EString + ',' + "SD Type: " + String(p) + ',' + "Volume: " +String(SD_Volume) + " GB" + ',' +
        
       "Found Sensors Id's:"  +  ','  + Sensor1_Id +  ',' +  Sensor2_Id + ','  + Sensor3_Id + ',' ;
       
       #ifdef PM25_DUST_SENSOR_EXISTS 
          dataString += "PMsensor Rev"  + ',' +  Sensor_Info_SDS  +  ',';
      #endif 
        dataString += "\n" ;
        dataString +=  "Year,Month,Date,Hour,Min,Sec,";
      #ifdef WIND_SENSOR_EXISTS  
        dataString += "WindRaw,velReading,WindMPH,WindTemp,";
      #endif  
      #ifdef TEMP_HUM_1_SENSOR_EXISTS 
       // dataString += "TemperatureSi072_1,Humidity_1,";
        dataString += "Temp1,Hum1(";
        dataString += Sensor1_Id;
        dataString += "),";
      #endif
      #ifdef TEMP_HUM_2_SENSOR_EXISTS 
       // dataString += "TemperatureSi072_2,Humidity_2,";
        dataString += "Temp2,Hum2(";
        dataString += Sensor2_Id;
        dataString += "),";
        
      #endif
       #ifdef TEMP_HUM_3_SENSOR_EXISTS 
      //  dataString += "TemperatureSi072_3,Humidity_3,";
        dataString += "Temp3,Hum3(";
        dataString += Sensor3_Id;
        dataString += "),";     
      #endif     
      #ifdef LEM_CURRENT_EXISTS 
        dataString += "Current(A)rms,";
      #endif   
      #ifdef VOLTAGE_MEASURE_EXISTS 
        dataString += "Voltage(V),";
      #endif          
      #ifdef BAR_PRES_SENSOR_EXISTS
        dataString += "Pressure(hPa),TemperatureBMP,Altitude(m),";
      #endif
      #ifdef LIGHT_SENSOR_EXISTS
         dataString += "Luminosity,";
      #endif
      #ifdef   ACCL_GYRO_SENSOR_EXISTS       
          dataString += "Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z),"; 
      #endif 
       #ifdef PM25_DUST_SENSOR_EXISTS 
          dataString += "PM2.5,PM10,"; 
      #endif  
       #ifdef AD9153_PROTOTYPE 
          dataString += "(A)rms,(V)rms,Power(W),PF,Freq.,"; 
      #endif  

      dataString += "RL1,RL2";
            
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

        #ifdef AD9153_PROTOTYPE 
          dataString += String(Values.Current)+ ',' + String(Values.Voltage)+ ',';
          dataString += String(Values.ActivePower)+ ',' + String(Values.PowerFactor)+ ',';
        //  if((Values.Voltage) > 32) dataString += String(Values.Frequency)+ ',';
       //   else                                    dataString += "--,";
           dataString += String(Values.Frequency)+ ',';
      #endif     

           dataString +=  String(digitalRead(RELAY_OUT_1))+ ','+ String(digitalRead(RELAY_OUT_2));  
  
}




/*
void SD_Log_File(){  
    if(SD_Card_Reset){
      SD_Card_Reset = 0;
      SD_Card_Info();
      SD_Card_Init();     
      dataString = "Year,Month,Date,Hour,Min,Sec,WindRaw,velReading,WindMPH,WindTemp,TemperatureSi072,Humidity,Pressure(hPa),";
      dataString += "TemperatureBMP,Altitude(m),Luminosity,Acc.(x),Acc.(y),Acc.(z),Gyro(x),Gyro(y),Gyro(z)";       
    }
    else{
      SD_Card_Init();     
      dataString = Str_DispTime;          
      dataString += String(RV_ADunits) + ',' + String(velReading)+ ',' + String(Values.WindTemp) + ',' +String(Values.WindMPH)+ ','       
      + String(Values.TemperatureSi072)+ ',' + String(Values.Humidity)+ ','+ String(Values.Pressure)+ ',' 
      + String(Values.TemperatureBMP) + ',' + String(Values.Altitude)+ ','+ String(Values.Luminosity) +','
      + String(Accelometer.x) + ',' + String(Accelometer.y)+ ','+ String(Accelometer.z) + ',
      '      
      + String(Gyro.x) + ',' + String(Gyro.y)+ ','+ String(Gyro.z) ;       
   }      
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(LOG_FILE, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
        Serial.print("dataString:");
      Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
  //  Serial.println("error opening datalog.txt");
    Serial.print("error opening : "); 
    Serial.println(LOG_FILE);    
  }
  
}

*/
/*
void SD_Write_File() {
  // make a string for assembling the data to log:
 // String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
*/

/*
void SD_Create_File(void){
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
    if (! logfile) {
   // error("couldnt create file");
    Serial.print("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);

   logfile.println("millis,stamp,datetime,light,temp,vcc");  
 
}
*/
/*
void SD_Create_File(void){

  if (SD.exists("AD_Logfile.txt")) {
    Serial.println("AD_Logfile.txt exists.");
  } else {
    Serial.println("AD_Logfile.txt doesn't exist. Creating");
    logfile = SD.open("AD_Logfile.txt", FILE_WRITE);
    logfile.close();
     if (SD.exists("AD_Logfile.txt")) 
     Serial.println("AD_Logfile.txt exists.");   
     else
     Serial.println("Cannot Create File!!"); 
  }
}
*/
