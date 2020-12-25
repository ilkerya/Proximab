
#if defined (ARDUINO_MEGA)  & defined (ARDUINO_DUE) 
    #error Select Only One Platform
#endif
 #if !(!defined (ARDUINO_MEGA) ^ !defined (ARDUINO_DUE)) 
    #error Select At Least One Platform
#endif


/*
typedef  byte               uint8;
typedef  signed char        int8;
typedef  unsigned char      uint8;
//typedef  signed short       int16;
//typedef  unsigned short     uint16;
typedef  signed int         int16;
typedef  unsigned int       uint16;
typedef  signed long        int32;
typedef  unsigned long      uint32;
typedef  signed long long   int64;
typedef  unsigned long long uint64

C:\Program Files (x86)\Arduino\libraries

*/
#define TEMP_HUM_1_SENSOR_EXISTS
#define TEMP_HUM_2_SENSOR_EXISTS
#define TEMP_HUM_3_SENSOR_EXISTS  

#define PM25_DUST_SENSOR_EXISTS
#define SDCARD_EXISTS
#define OLEDDISPLAY_EXISTS
#define ENERGYMETER_EXISTS 
#define PROGRELAY_EXISTS 

//#define LIGHT_SENSOR_EXISTS  
//#define BAR_PRES_SENSOR_EXISTS  
//#define ACCL_GYRO_SENSOR_EXISTS  
//#define WIND_SENSOR_EXISTS  
//#define LEM_CURRENT_EXISTS
//#define VOLTAGE_MEASURE_EXISTS




#define AD9153_PROTOTYPE  // AD9153 Power Monitoring IC Related IOs

 // #define DEBUG_SIMULATOR_MODE // For DEbugging As A Simulator
// Select Hardware Type
//#define FIRST_PROTOTYPE  // with LEM current Transdcucer

#define ARM_MATH_CM0PLUS


#define RELAY_OUT_1 23
#define RELAY_OUT_2 53

//#include "SdsDustSensor.h" // https://github.com/lewapek/sds-dust-sensors-arduino-library

/*
 #define SI072_FIRST_SENSOR 7  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 1 // first prot  0      0
 #define SI072_THIRD_SENSOR 2 // sec1                2 
*/
/*
 // first ADE9153 prototypes
 #define SI072_FIRST_SENSOR 2  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 3 // first prot  0      0
 #define SI072_THIRD_SENSOR 4 // sec1                2 
*/

#define POWERIC_SETUP1 0  // Init
#define POWERIC_SETUP2 1  // Delay
#define POWERIC_SETUP3 2 // End
#define POWERIC_NORMAL 3
#define POWERIC_CALB1  4
#define POWERIC_CALB2  5 
#define POWERIC_CALB3  6
#define POWERIC_CALB4  7
#define POWERIC_CALB5  8
#define POWERIC_CALB6  9
#define POWERIC_CALB7  10
#define POWERIC_CALB8  11
#define POWERIC_CALB9  12
#define POWERIC_CALB10 13



 #define SI072_FIRST_SENSOR 7  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 3 // first prot  0      0
 #define SI072_THIRD_SENSOR 2 // sec1                2 

#define NO_IC2_MULTIPLEXER 16
#define DEBUG_KEY

#define ON 1 //
#define OFF 0 //

#ifdef FIRST_PROTOTYPE
  //  const int chipSelect = 10; // mega SS for SD Card  
  #define KEY_RIGHT 2//12//2 //
  #define LED_GREEN 3// 11//3 // GREEN
  #define LED_RED 4 // 12//4 //RED
  #define KEY_MID 5// 11//5 //
  #define KEY_LEFT 6//13//6 // ok
#endif


  #define TDI         A7
  #define TDO         A6
  #define TMS         A5
  #define TCK         A4
  #define ANALOG3         A3
  #define ANALOG2         A2
  #define ANALOG1         A1
  
  #define DEBUG_OUT     A0  
  
  #define KEY_RIGHT       13
  #define KEY_MID         12
  #define KEY_LEFT        11
  const int SD_CS_PINOUT = 10;
 // #define SD_CS_PINOUT    10  
  #define LED_RED        9  // L1
    #ifdef ENERGYMETER_EXISTS
  #define ADE9153A_CS_PIN  8 
      #endif      
  #define  LED_GREEN         7 //L2
     #ifdef ENERGYMETER_EXISTS 
  #define ADE9153A_RED_LED 6                 //On-board LED pin 
  #define ADE9153A_CALB_BUTTON   5         
  #define ADE9153A_RESET_PIN     4  
  #define ADE9153A_IRQ_PIN       3
  #define ADE9153A_ZX_DREADY_PIN 2
    #endif
  #define TX_OUTPUT_PIN          1 //ON BOARD PROGRAMMING & DEBUG RESERVED
  #define RX_INPUT_PIN           0  //ON BOARD PROGRAMMING & DEBUG RESERVED

  #define OUT_PINOUT 2 // Out pin of the sensor
  #define RV_PINOUT 1 // RV output of the sensor
  #define TMP_PINOUT 0 // analogRead(TMP_PINOUT);



#define MAX_DISPLAY_CHAR 21
#define  MAXSHOWLINE 6  // define how many lines for sensorts to show including fw info line 


#define MENU_NULL 0
#define MENU1   16
#define MENU2   32
#define MENU3   48
#define MENU4   64
#define MENU5   80
#define MENU6   96

#define MENU1_SUB1 17 // +=4
#define MENU1_SUB2 18
#define MENU1_SUB3 19 // +=4
#define MENU1_SUB4 20

#define MENU2_SUB1  33  // +=4
#define MENU2_SUB2  34
#define MENU2_SUB3  35
#define MENU2_SUB4  36
#define MENU2_SUB5  37
#define MENU2_SUB6  38
#define MENU2_SUB7  39
#define MENU2_SUB8  40

#define MENU3_SUB1  49 // +=4
#define MENU3_SUB2  50
#define MENU3_SUB3  51 // +=4
#define MENU3_SUB4  52

#define MENU4_SUB1 65
#define MENU4_SUB2 66

#define MENU5_SUB1 81
#define MENU5_SUB2 82
#define MENU5_SUB3 83
#define MENU5_SUB4 84
#define MENU5_SUB5 85
#define MENU5_SUB6 86
#define MENU5_SUB7 87
#define MENU5_SUB8 88

#define MENU6_SUB1 97
#define MENU6_SUB2 98
#define MENU6_SUB3 99

#define KEYDISP_TIMER 40

// function prototypes
void Common_Loop(); 
void ResetCasePrint();
void IO_Settings();
void MicroInit(void);
void Display_ReInit_Start(byte Timer);
void Display_ReInit_End(void);

void  RTC_Init();
void  SensorInit_Si072(byte);
void  SensorAlt_Init();
void  SensorLight_Init();
void  SensorACccel_GyroInit();
void  Sensors_PeripInit();

void CurrentVolt_Read();
void  AdcRead();
void WindSensorRead();
void  SensorRead_Si072(unsigned char);
void  SensorAlt_Read();
void  SensorLight_Read();
void  SensorAcccel_GyroRead();
void SDS_DustSensor(void);
void UpdateSensorInfo(void);


void UpdateInfoLine();
void UpdateDisplayMenu();
void UpdateSD_LogTime();
void UpdateFileSize();
void UpdateProperLine(byte Index, byte Line);

void EscMenuKey(void);
void EnterMenuKey(void);
void DownMenuKey(void);
void UpMenuKey(void);
void SetSampling(unsigned int Time);
void DispEnable(bool Enable, byte Timer);
void DispEnable_4SD_Prblm(bool Enable, byte Timer);

void  DispExtTimeout(void);
void   DisplayMenu(void);
void KeyTimeOutCheck(void);
void SD_CardLogTask(void);
void SD_Log_File(void);
void SD_Info_Only(void);
void DisplayFullSensors(void);
void DisplayTestDevices(void);
void SerialPortRx(void);
void UpdateDispRoll(void);
void Log_Data_Write_SD(void);

void Parse_FileString(void);
void Relay_loop(void) ;
float GetValue(byte Relay);
String LimitCopyDisplayStr(String str, byte MaxNumber);
void EnergyMeterIC_Operation(void);
void I2_ACK_Reset(void);


void SetResetLog(bool Enable);
void NVRam_Write_LogStatus(bool Mode);
void NVRam_Read_SampleTime(void);
void NVRam_Write_SampleTime(byte Sample);
void NVRam_Read_Standbye(void);
void NVRam_Write_Standbye(bool Mode);
void NVRam_Read_SerNo(void);
void NVRam_Write_SerNo(char* p);

void Due_Memory();


// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries
// Location of Main Librarires

// Set Origin
//   git remote add origin git@github.com:ilkerya/Pangolin.git
//  https://github.com/ilkerya/Pangolin
//  https://blog.plover.com/prog/git-ff-error.html
// C:\Users\Yagciilk\Documents\Arduino\libraries
// C:\Program Files (x86)\Arduino\libraries   
// C:\Projects\Pangolin\Pangolin\ArduinoCore\include  default AtmelStudio Project lib locations
/*
git add .
git commit -m "Comment"
git push https://github.com/ilkerya/Pangolin.git master
*/
/*
First Time
git init
git add README.md

GitHub Bash Commands
Adding new file
https://help.github.com/articles/adding-a-file-to-a-repository-using-the-command-line/#platform-windows

git add .
git commit -m "Add existing file"
// For Common git
git push https://github.com/ilkerya/Pangolin master
// For Electrolux git
git push  http://git.int.electrolux.com/ilkerya/Phoenix_Pangolin.git master
https://help.github.com/articles/fetching-a-remote/
http://git.int.electrolux.com/ilkerya/Phoenix_Mobile.git

In case in the first push gives error use below command
git remote add origin remote repository URL
git push origin master

git push https://github.com/ilkerya/Phoenix master --force
*/
