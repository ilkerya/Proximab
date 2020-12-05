

#define TCAADDR 0x70  //  
//#define CHANNEL_VOC 2 //  
#define CHANNEL_TEMPHUM 7  // 0x40 default address 
#define CHANNEL_LIGHT 6  // 0x29 default address sio71 
#define CHANNEL_BAROMETRIC 5  // 0x77 default address bmp388 
#define CHANNEL_ACCEL_GYRO 1  // 0x77 default address LSM9DS1 accoloremoter/Gyroscope/MAgnetometer 
//                            0x68 RTC sdefault address
// https://cdn-learn.adafruit.com/downloads/pdf/adafruit-tsl2591.pdf
// https://cdn-shop.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf
//https://cdn-learn.adafruit.com/downloads/pdf/adafruit-bmp388.pdf?timestamp=1556108471
// https://cdn-learn.adafruit.com/downloads/pdf/adafruit-si7021-temperature-plus-humidity-sensor.pdf
// https://learn.adafruit.com/adafruit-lsm9ds1-accelerometer-plus-gyro-plus-magnetometer-9-dof-breakout  
// https://moderndevice.com/product/wind-sensor/
// https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/SDS021_laser_PM2.5_sensor_specification-V1.0.pdf    // PM2.5 dust particle sensor

// https://www.lem.com/en/5-b10   current sensor LEM AT5B10
// https://www.microchip.com/wwwproducts/en/MCP6002 for Buffering Cuurent Sensor AT5B10
   
// Differenatial I2C communication
// https://www.sparkfun.com/products/14685      //I2c multiplexer
// https://www.sparkfun.com/products/14589      // I2c sigle ended 2 diffrential 
// https://cdn.sparkfun.com/assets/c/4/7/0/9/SparkFun_Differential_I2C_Breakout_PCA9615_Qwiic.pdf

// https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf
// https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/SDS021_laser_PM2.5_sensor_specification-V1.0.pdf
/*
int rxPin = 19;
int txPin = 18;
SdsDustSensor sds(rxPin, txPin);
*/
//SdsDustSensor sds(Serial1); // passing HardwareSerial& as parameter
//SdsDustSensor sds(HardwareSerial); // passing HardwareSerial& as parameter

#ifdef TEMP_HUM_1_SENSOR_EXISTS  //TEMP_HUM_2_SENSOR_EXISTS TEMP_HUM_3_SENSOR_EXISTS
  #include "Adafruit_Si7021.h"
  Adafruit_Si7021 THsensor = Adafruit_Si7021();
#endif

#ifdef LIGHT_SENSOR_EXISTS  
  #include "Adafruit_TSL2591.h"
  Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
#endif

#ifdef BAR_PRES_SENSOR_EXISTS 
  #include "Adafruit_BMP3XX.h"
  #define SEALEVELPRESSURE_HPA (1013.25)
  Adafruit_BMP3XX bmp; // I2C  //BAROMETRIC PRESSURE
#endif

#ifdef ACCL_GYRO_SENSOR_EXISTS 
  #include "Adafruit_LSM9DS1.h"
  Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(); 
#endif

// Arduino BH1750FVI Light sensor
//https://github.com/mysensors/MySensorsArduinoExamples/blob/master/examples/LightLuxSensor/LightLuxSensor.ino

void Sensors_PeripInit(void){
  #ifdef SDCARD_EXISTS
    SD_Card_Info();
    SD_Card_Init();
    GetFileSize();
    ReadConfigFile();
  #endif  
  RTC_Init();



  #ifdef TEMP_HUM_1_SENSOR_EXISTS
    SensorInit_Si072(SI072_FIRST_SENSOR); // TEMP HUM
  #else
    Serial.print("No Sensor On Channel!!");Serial.print("SI072_FIRST_SENSOR");
  #endif 

  #ifdef TEMP_HUM_2_SENSOR_EXISTS
    SensorInit_Si072(SI072_SECOND_SENSOR); // TEMP HUM
  #else
    Serial.print("No Sensor On Channel!!");Serial.print("SI072_SECOND_SENSOR");
  #endif 

  #ifdef TEMP_HUM_3_SENSOR_EXISTS
    SensorInit_Si072(SI072_THIRD_SENSOR); // TEMP HUM
  #else
    Serial.print("No Sensor On Channel!!");Serial.print("SI072_THIRD_SENSOR");
  #endif 
      
  #ifdef BAR_PRES_SENSOR_EXISTS 
     SensorAlt_Init();     //BAROMETRIC PRESSURE
  #else
    Serial.println("No Bar.Pressure_Sensor!!");
  #endif 
      
  #ifdef LIGHT_SENSOR_EXISTS  
    SensorLight_Init();  // LIGHT 
  #else
    Serial.println("No Light_Sensor!!");
  #endif
      
  #ifdef ACCL_GYRO_SENSOR_EXISTS 
    SensorACccel_GyroInit(); // ACCEL GYRO  
   #else
    Serial.println("No Accel Gyro_Sensor!!");
  #endif  
  #ifdef PM25_DUST_SENSOR_EXISTS 
    Serial1.begin(9600);
    Serial.print("PMSensor Serial Port Init ");
   #endif 

}
void tcaselect(uint8_t i) {
 // if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
    //dafruit Industries
    //  https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(2);
}

#ifdef PM25_DUST_SENSOR_EXISTS  
void PrintPMValues(byte PMError, byte PMCount){
  /*
      Serial.println();
      for(int i=0; i<BUF_LENGTH;i++){  
          Serial.print(i); Serial.print('.'); Serial.print(PMBuffer[i]);Serial.print("    ");
          if(i==9)Serial.println();  
          if(i==19)Serial.println();  
          if(i==29)Serial.println();  
          if(i==39)Serial.println(); 
          if(i==49)Serial.println();       
      }
      */
     Serial.println();  
      Serial.print("PMCount ");Serial.println(PMCount);
      Serial.print("PM25 ");Serial.println(Values.PM25);
      Serial.print("PM10 ");Serial.println(Values.PM10);
      Serial.print("PMError ");Serial.println(PMError);         
}
 void SerialPortPMSensor() {
    byte Rx;
    byte RxCount=0;
    unsigned int PM25Val=0;
    unsigned int PM10Val=0;
     byte PMError = 0; 
     byte PMCount = 0; 
     unsigned char checksum = 32; 
    if (Serial1.available()) {
        for(int i=0; i<BUF_LENGTH;i++){   
          PMBuffer[i] = 0;
        }   
        while (Serial1.available()) {
            Rx = Serial1.read();
            if(RxCount < BUF_LENGTH)PMBuffer[RxCount] =  Rx; 
            RxCount++;      
        }
        Serial1.flush();      
        for(int i=0; i<BUF_LENGTH;i++){
            if(PMBuffer[i] == 0XAA){
              if(PMBuffer[i+1] == 0XC0){
                    checksum=(unsigned char)(PMBuffer[i+2]+PMBuffer[i+3]+PMBuffer[i+4]+PMBuffer[i+5]+ PMBuffer[i+6]+PMBuffer[i+7]);                         
                    if(checksum == PMBuffer[i+8]){ 
                        PMCount++;  
                        PM25Val += (256 * PMBuffer[i+3])+PMBuffer[i+2];
                        PM10Val += (256 * PMBuffer[i+5])+PMBuffer[i+4];                                  
                    }
                    else PMError++;  
              }
          }       
      }
      if(PMCount){
          Values.PM25 =  (float)(PM25Val / PMCount)/10;
          Values.PM10 =  (float)(PM10Val / PMCount)/10;
          if(Values.PM25 >= 250)Values.PM25 = 250;
          if(Values.PM10 >= 250)Values.PM25 = 250;       
      }
      else{
          Values.PM25 =  0;
          Values.PM10 =  0;   
      }
      PrintPMValues(PMError,PMCount);  
    }
}
 #endif 

#ifdef ACCL_GYRO_SENSOR_EXISTS 
void SensorACccel_GyroInit(){
    tcaselect(CHANNEL_ACCEL_GYRO);
    if (!IMU.begin()) {
      Serial.println("Failed to initialize IMU!");
     // while (1);
    }
    else       
      Serial.println("Acceloreometer & Gyro Initialized");
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    Serial.println();
    Serial.println("Acceleration in G's");
    Serial.println("X\tY\tZ");

    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.println();
    Serial.println("Gyroscope in degrees/second");
    Serial.println("X\tY\tZ");
  }
void SensorAcccel_GyroRead(){
   tcaselect(CHANNEL_ACCEL_GYRO);
 //  float x, y, z;

  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(Accelometer.x, Accelometer.y, Accelometer.z);

      Serial.print(Accelometer.x);
      Serial.print('\t');
      Serial.print(Accelometer.y);
      Serial.print('\t');
      Serial.println(Accelometer.z);
  }
    else Serial.println("Accelometer Reading Problem");
  
  if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(Gyro.x, Gyro.y, Gyro.z);

      Serial.print(Gyro.x);
      Serial.print('\t');
      Serial.print(Gyro.y);
      Serial.print('\t');
      Serial.println(Gyro.z);
  }
  else Serial.println("Gyroscope Reading Problem");
}
#endif  
#ifdef TEMP_HUM_1_SENSOR_EXISTS  
void SensorInit_Si072(byte Channel){
  // Temperature & Humidity Sensor
  if(Channel != NO_IC2_MULTIPLEXER)tcaselect(Channel);
  Serial.print("Sensor_Channel:");Serial.print(Channel);
  Serial.println("  Si7021 test!");
  
  if (!THsensor.begin()) {
    Serial.print("No Si7021 sensor On Channel: ");Serial.println(Channel);
  //  delay(250);
  //  while (true)      
  }else{
    Serial.print(" Si7021 sensor found!");
    Serial.print(" Rev(");
    Serial.print(THsensor.getRevision());
    Serial.print(")");
     Serial.print(" Serial "); Serial.print(THsensor.sernum_a, HEX); Serial.println(THsensor.sernum_b, HEX);
     Serial.print("Sensor_Channel:");Serial.println(Channel);
     Serial.print(" String : ");
    switch(Channel){
      case NO_IC2_MULTIPLEXER:
      case SI072_FIRST_SENSOR:
            Sensor1_Id = String(THsensor.sernum_a, HEX) ;//+ String(THsensor.sernum_b,HEX); 
            Sensor1_Id.toUpperCase();     
            Serial.println(Sensor1_Id);
       break;
      case SI072_SECOND_SENSOR:
            Sensor2_Id = String(THsensor.sernum_a, HEX);// + String(THsensor.sernum_b,HEX);
            Sensor2_Id.toUpperCase();        
            Serial.println(Sensor2_Id);
       break;
      case SI072_THIRD_SENSOR: 
            Sensor3_Id = String(THsensor.sernum_a, HEX);// + String(THsensor.sernum_b,HEX);
            Sensor3_Id.toUpperCase();        
             Serial.println(Sensor3_Id);
       break; 
       default:
              Serial.print("error");
       break;
    }  
  }
}
void GerSerialNo(void){
      Serial.print(" Rev(");
    Serial.print(THsensor.getRevision());
    Serial.print(")");
    Serial.print(" Serial #"); Serial.print(THsensor.sernum_a, HEX); Serial.println(THsensor.sernum_b, HEX);
}


void SensorRead_Si072(byte Channel){
    if(Channel != NO_IC2_MULTIPLEXER)tcaselect(Channel);

   // Serial.print("Humidity_");Serial.print(Channel);Serial.print(" %");

    switch(Channel){
      case NO_IC2_MULTIPLEXER:
      case SI072_FIRST_SENSOR: 
        Values.Humidity_Ch1 = THsensor.readHumidity();       
      //  Serial.print(Values.Humidity_Ch1, 2);
   
       break;
      case SI072_SECOND_SENSOR: 
        Values.Humidity_Ch2 = THsensor.readHumidity();
   //     Serial.print(Values.Humidity_Ch2, 2);

       break;
      case SI072_THIRD_SENSOR: 
        Values.Humidity_Ch3 = THsensor.readHumidity();
     //   Serial.print(Values.Humidity_Ch3, 2);
 
       break; 
       default:
        Serial.print("error");
       break;
    }
 
  //  Serial.print(" Temperature");Serial.print(Channel);Serial.print(" :");

    switch(Channel){
      case NO_IC2_MULTIPLEXER:
      case SI072_FIRST_SENSOR: 
        Values.TemperatureSi072_Ch1 = THsensor.readTemperature();
   //     Serial.println(Values.TemperatureSi072_Ch1, 2);
       break;
      case SI072_SECOND_SENSOR: 
        Values.TemperatureSi072_Ch2 = THsensor.readTemperature();
    //    Serial.println(Values.TemperatureSi072_Ch2, 2);
       break;
      case SI072_THIRD_SENSOR: 
        Values.TemperatureSi072_Ch3 = THsensor.readTemperature();
   //     Serial.println(Values.TemperatureSi072_Ch3, 2);
       break; 
       default:
        Serial.print("error");
       break;
    } 
}
#endif
#ifdef BAR_PRES_SENSOR_EXISTS 
void SensorAlt_Init() {
  tcaselect(CHANNEL_BAROMETRIC);
  Serial.println("BMP388 test");
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    //while (1);
  }
  else {
      Serial.println(" BMP388 sensor Valid!");
      // Set up oversampling and filter initialization
      bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
      bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
      bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
      //bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  }
}

void SensorAlt_Read(){
      tcaselect(CHANNEL_BAROMETRIC);
      
    if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
   // return;
  }
  else{
  
    Serial.print("Temperature = ");
    Values.TemperatureBMP = bmp.temperature;
    Serial.print(Values.TemperatureBMP);
    Serial.print(" *C");

    Serial.print("  Pressure = ");
    Values.Pressure = bmp.pressure / 100.0;
    Serial.print(Values.Pressure);
    Serial.println(" hPa");

    Serial.print("Altitude = ");
    Values.Altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.print(Values.Altitude);
    Serial.println(" m");
  }
}
#endif
#ifdef LIGHT_SENSOR_EXISTS
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
 // delay(500);
}
void configureSensor(void)
{
 
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}
void SensorLight_Init(void) {
  tcaselect(CHANNEL_LIGHT);
  Serial.println(F("Starting Adafruit TSL2591 Test!"));
  if (tsl.begin()) {
    Serial.println(F("Found a TSL2591 sensor"));
  } 
  else {
    Serial.println(F("No sensor found ... check your wiring?"));
   // while (1);
  }    
  /* Display some basic information on this sensor */
  displaySensorDetails();  
  /* Configure the sensor */
  configureSensor();
  // Now we're ready to get readings ... move on to loop()!
}
void SensorLight_Read(void) {
    tcaselect(CHANNEL_LIGHT);
    // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
//  uint16_t x = tsl.getLuminosity(TSL2591_VISIBLE);
  Values.Luminosity = tsl.getLuminosity(TSL2591_VISIBLE);
  
  //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

//  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Luminosity: "));
  Serial.println(Values.Luminosity, DEC);
}
#endif


#ifdef WIND_SENSOR_EXISTS

void WindSpeed_Calculation(){
    tempReading = (double)Values.TemperatureSi072;
    // calculate wind speed 
 //   #define analogPinForRV 1 // Analog pin0 for mega 
    RV_ADunits = analogRead(RV_PINOUT);      // read analog input of wind sensor RV output
    for (int i = 0; i < 7; i++) {                 // read another 7 times to improve reading quality and reduce noise
      RV_ADunits += analogRead(RV_PINOUT);
    }
    RV_ADunits /= 8;                              // devide by 8, to get average
    RV_ADunits_dev1000 = RV_ADunits / 1000;       // devide by 1000, to match calibration factors and calculation
    Values.WindRaw = RV_ADunits_dev1000;
    velReading = (2.285 * pow(RV_ADunits_dev1000, 3) - 12.417 * pow(RV_ADunits_dev1000, 2) 
      + 22.831 * RV_ADunits_dev1000 - 14.19) / (T_rev_wind / tempReading) * v_calc_F4 
      - ((T_rev_wind - tempReading) * v_calc_F1 * pow(RV_ADunits_dev1000, v_calc_F3)) 
      - (T_rev_wind - tempReading) * v_calc_F2 ; // wind speed calculation based on own calibration - see reference on authors' website
    velReading = velReading - zeroWindAdjustment; // adjust zero windspeed
    if (velReading < 0) {
      velReading = 0; // error handling in case of false zeroWindAdjustment setting
    }
}
 
 //  This Part is based on the Sensors Manufacturer Data sheet
  //  Values.WindMPH
  
  // #define WND_OUT_Pin  0;// A0;   // wind sensor analog pin  hooked up to Wind P sensor "OUT" pin
  //  Values.WindTemp  
  // #define WND_TEMP_Pin 1;// A1;   // temp sesnsor analog pin hooked up to Wind P sensor "TMP" pin

 
void WinSensor(){
        int windADunits = analogRead(WND_OUT_Pin);
    // Serial.print("RW ");   // print raw A/D for debug
    // Serial.print(windADunits);
    // Serial.print("\t");
    if (windADunits < 350 ) windADunits = 350; // min 350 for 
  
    Serial.print("windADunits");Serial.println(windADunits);
    Values.WindMPH =  pow((((float)windADunits - 264.0) / 85.6814), 3.36814);
  //  float windMPH =  pow((((float)windADunits - 264.0) / 85.6814), 3.36814);
   // Serial.print(windMPH);
  //  Serial.print(" MPH\t");    

    // temp routine and print raw and temp C
    int tempRawAD = analogRead(WND_TEMP_Pin);  
 //   Serial.print("tempRawAD");Serial.println(tempRawAD); 
    // Serial.print("RT ");    // print raw A/D for debug
    // Serial.print(tempRawAD);
    // Serial.print("\t");
    
    // convert to volts then use formula from datatsheet 
    // Vout = ( TempC * .0195 ) + .400
    // tempC = (Vout - V0c) / TC   see the MCP9701 datasheet for V0c and TC
    
   // float tempC = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195; 
    Values.WindTemp = ((((float)tempRawAD * 5.0) / 1024.0) - 0.400) / .0195; 
 //   Serial.print(tempC);
 //   Serial.println(" C");
    //delay(750);
}
#endif

    
