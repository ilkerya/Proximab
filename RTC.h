 
//https://github.com/adafruit/Data-Logger-shield
// https://forum.arduino.cc/index.php?topic=523216.0

RTC_PCF8523 rtc; // I2c Addres 0x68
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//static const char char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesdy", "Wedns.", "Thurs.", "Friday", "Satur."};

void RTC_TimeClock(){

  if(DateTimeBuf.RTC_Update == ON){
    DateTimeBuf.RTC_Update = OFF;
    rtc.adjust(DateTime(DateTimeBuf.Year, DateTimeBuf.Month, DateTimeBuf.Day, DateTimeBuf.Hour, DateTimeBuf.Minute, DateTimeBuf.Second));
    return;
  }
    DateTime now = rtc.now();

    Str_DispTime = "";
    Str_DispTime += String(now.year(),DEC);   
    Str_DispTime += ',';    
    if(now.month()<10)Str_DispTime += '0';  
     Str_DispTime += String(now.month(),DEC);
     Str_DispTime += ',';       
    if(now.day()<10)Str_DispTime += '0';  
     Str_DispTime += String(now.day(),DEC);  
     Str_DispTime += ",";
    if(now.hour()<10)Str_DispTime += ' ';
     Str_DispTime += String(now.hour(),DEC);
      Str_DispTime += ',';    
    if(now.minute()<10)Str_DispTime += '0';   
     Str_DispTime += String(now.minute(),DEC);
    Str_DispTime += ',';   
    if(now.second()<10)Str_DispTime += '0';   
     Str_DispTime += String(now.second(),DEC);
     Str_DispTime +=  ','; 

    Str_Date = "";
    Str_Date += String(now.year(),DEC);   
    Str_Date += '.';    
    if(now.month()<10)Str_Date += '0';  
     Str_Date += String(now.month(),DEC);
     Str_Date += '.';       
    if(now.day()<10)Str_Date += '0';  
     Str_Date += String(now.day(),DEC);
    // Str_Date += ''; 
     
     Str_Time = "";
    if(now.hour()<10)Str_Time += '0';
     Str_Time += String(now.hour(),DEC);
      Str_Time += ':';    
    if(now.minute()<10)Str_Time += '0';   
     Str_Time += String(now.minute(),DEC);
    Str_Time += ':';   
    if(now.second()<10)Str_Time += '0';   
     Str_Time += String(now.second(),DEC);
   //  Str_Time +=  ''; 

    if(DateTimeBuf.Init == ON){
      DateTimeBuf.Init = OFF;
      DateTimeBuf.Year = now.year();
      DateTimeBuf.Month = now.month();
      DateTimeBuf.Day = now.day();
      DateTimeBuf.Hour = now.hour();
      DateTimeBuf.Minute = now.minute();
      DateTimeBuf.Second = now.second();
    }
}

void  RTC_Init(){
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    //while (1);
  }
  if (! rtc.initialized()) {
    Serial.println(F("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //   RTC_Test = 18;
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  else{
    Serial.println(F("RTC is Running!"));
  } 

  if(Serial) { // if serial port connected adjust it from computer
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  //  Serial.println("RTC Time Adjusted!");
  //  RTC_Test = 18;
  }
 }

 void SerialPortRx() {
    static uint8_t ndx = 0;
    char endMarker = '\n';
    uint8_t Timer = 0;
    char rc;
        // if (Serial.available() > 0) {
     while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        Timer++; // 2020,05,27,21,14,23 19 characters + \0' // in total 21

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
              ndx = numChars - 1;
          }
        }
        else {
          receivedChars[ndx] = '\0'; // terminate the string
          ndx = 0;
          newData = true;
      }
   }
  //////////////////// end of while loop 
   if (newData == true) {
        Serial.print(F("This just in .................................. "));
          Serial.println(receivedChars);
          newData = false;            
          Serial.print(F("Timer:"));  
          Serial.println(Timer);
          unsigned int Year=0; 
          unsigned int Month=0; 
          unsigned int Day=0;                       
          unsigned int Hour=0; 
          unsigned int Minute=0; 
          unsigned int Second=0; 
               
        if((Timer == 21) && (receivedChars[4] == ',' )&&(receivedChars[7] == ',') && 
           (receivedChars[10] == ',')&&  (receivedChars[13] == ',' )&& (receivedChars[16] == ',' )){
            for(int i = 0; i<32; i ++ ){ // ascii 2 byte
                receivedChars[i] -= 48;          
            }
            Year  = 1000 * receivedChars[0];
            Year  += 100 * receivedChars[1]; 
            Year  += 10 * receivedChars[2];  
            Year  += receivedChars[3];  
            Month  += 10 * receivedChars[5];      
            Month  += receivedChars[6];  
            Day  += 10 * receivedChars[8];       
            Day  += receivedChars[9];            
            Hour  += 10 * receivedChars[11];  
            Hour  += receivedChars[12]; 
            Minute  += 10 * receivedChars[14];
            Minute  += receivedChars[15]; 
            Second  += 10 * receivedChars[17];    
            Second  += receivedChars[18];       
            rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
            MainMenu = MENU5_SUB7;
            DispExtTimeout();
        }
        if((Timer == 10) && (receivedChars[0] == 'E' )&&(receivedChars[1] == 'E') && (receivedChars[2] == 'E')&&  (receivedChars[3] == 'E' )){       
          NVRam_Write_SerNo(&receivedChars[4]); 
          delay(10);
          NVRam_Read_SerNo();                        
        }  
    }
}
