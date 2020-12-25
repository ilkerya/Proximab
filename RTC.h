 
//https://github.com/adafruit/Data-Logger-shield
// https://forum.arduino.cc/index.php?topic=523216.0

// DS1307

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
/*    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
*/

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
    

/*
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");

    // calculate a date which is 7 days, 12 hours and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    Serial.print(" now + 7d + 12h + 30m + 6s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    */

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
    static byte ndx = 0;
    char endMarker = '\n';
    byte Timer = 0;
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
           /*       
              Serial.print("Year:");  
              Serial.println(Year);
               Serial.print("Month:");  
              Serial.println(Month);
              Serial.print("Day:");  
              Serial.println(Day);

              Serial.print("Hour:");  
              Serial.println(Hour);
              Serial.print("Minute:");  
              Serial.println(Minute);
              Serial.print("Second:");  
              Serial.println(Second); 
              */          
              rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
        //      Serial.println("Date & Time Adjusted");
               MainMenu = MENU5_SUB7;
               DispExtTimeout();
          }
        if((Timer == 10) && (receivedChars[0] == 'E' )&&(receivedChars[1] == 'E') && (receivedChars[2] == 'E')&&  (receivedChars[3] == 'E' )){
          // EE Serial Code Write
          //receivedChars[4] == 'E' ;
/*
          unsigned int Code=0;
       //   Code <<= 12;
          Code += (receivedChars[4]<<12);          
          Code += (receivedChars[5]<<8);
          Code += (receivedChars[6]<<4);
          Code += receivedChars[7];
    */      
     
/*
   Serial.print("EECode:");

  #ifdef ARDUINO_MEGA // 8 bit AVR 
          

          EEPROM.write(4, receivedChars[4]);// high byte
          Serial.print(receivedChars[4]);

          EEPROM.write(5, receivedChars[5]);// high byte          
          Serial.print(receivedChars[5]);       

           EEPROM.write(6, receivedChars[6]);// high byte         
          Serial.print(receivedChars[6]);

           EEPROM.write(7, receivedChars[7]);// high byte         
          Serial.println(receivedChars[7]);
  #endif

      #ifdef ARDUINO_DUE
          dueFlashStorage.write(4, receivedChars[4]);// high byte
          Serial.print(receivedChars[4]);

          dueFlashStorage.write(5, receivedChars[5]);// high byte          
          Serial.print(receivedChars[5]);       

           dueFlashStorage.write(6, receivedChars[6]);// high byte         
          Serial.print(receivedChars[6]);

           dueFlashStorage.write(7, receivedChars[7]);// high byte         
          Serial.println(receivedChars[7]);
    
    #endif
*/
        NVRam_Write_SerNo(&receivedChars[4]);
    
          
        //  Serial.println(Code);  
/*
          EEPROM.write(4, receivedChars[4]);// high byte
          EEPROM.write(5, receivedChars[5]);// low byte
          EEPROM.write(6, receivedChars[6]);// high byte
          EEPROM.write(7, receivedChars[7]);// low byte
       */   
          delay(10);

          NVRam_Read_SerNo();                        
      }  
    }
 }


/*
 void RTC_SerialAdj() {
  if (Serial.available() > 0) {
    incomingStr = Serial.readString();  
     Serial.print("I received: ");
    Serial.print(incomingStr); 
    Serial.println("");

  // Send ADjTime from serial port to Adjust date & time from computer
    if (incomingStr[0] == 'A')
      if (incomingStr[1] == 'd')
        if (incomingStr[2] == 'j')
          
            if (incomingStr[3] == 'T')
              if (incomingStr[4] == 'i')
                if (incomingStr[5] == 'm')
                    if (incomingStr[6] == 'e')
                    
                    {
                      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
                      Serial.println("AdjTime RTC routine Started");
                      
                    }
    Serial.flush(); 
  }
}

*/
 
