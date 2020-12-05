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
*/
 
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

 // #define DEBUG_SIMULATOR_MODE // For DEbugging As A Simulator
// Select Hardware Type
//#define FIRST_PROTOTYPE  // with LEM current Transdcucer
#define AD9153_PROTOTYPE  // AD9153 Power Monitoring IC

#define TEMP_HUM_1_SENSOR_EXISTS
#define TEMP_HUM_2_SENSOR_EXISTS
#define TEMP_HUM_3_SENSOR_EXISTS  
//#define LIGHT_SENSOR_EXISTS  
//#define BAR_PRES_SENSOR_EXISTS  
//#define ACCL_GYRO_SENSOR_EXISTS  
//#define WIND_SENSOR_EXISTS  
//#define LEM_CURRENT_EXISTS
//#define VOLTAGE_MEASURE_EXISTS
#define PM25_DUST_SENSOR_EXISTS
#define SDCARD_EXISTS

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


 #define SI072_FIRST_SENSOR 2  // multiplexer Channel 7 first blu box prot
 #define SI072_SECOND_SENSOR 3 // first prot  0      0
 #define SI072_THIRD_SENSOR 7 // sec1                2 

#define NO_IC2_MULTIPLEXER 16

#define DEBUG_KEY

#define ON 1 //
#define OFF 0 //

#ifdef FIRST_PROTOTYPE
  #define KEY_RIGHT 2//12//2 //
  #define LED_GREEN 3// 11//3 // GREEN
  #define LED_RED 4 // 12//4 //RED
  #define KEY_MID 5// 11//5 //
  #define KEY_LEFT 6//13//6 // ok
#endif
#ifdef AD9153_PROTOTYPE
// occupied pins 2,3,4,5,8
  #define KEY_RIGHT 13
  #define LED_GREEN 9
  #define LED_RED  7
  #define KEY_MID  12
  #define KEY_LEFT 11
#endif
  const int chipSelect = 10; // mega SS for SD Card

#define  MAXSHOWLINE 6  // define how many lines for sensorts to show including fw info line 

/*
#define MENU_NULL 0
#define MENU1   32
#define MENU2   64
#define MENU3   96
#define MENU4   128
#define MENU5   160

//#define MENU_MAINMIN  32
//#define MENU_MAINMAX  MENU5

//#define MENU1MIN  36
//#define MENU1MAX  40

#define MENU1_SUB1 36 // +=4
#define MENU1_SUB2 40

//#define MENU2MIN  68
//#define MENU2MAX  92

#define MENU2_SUB1  68  // +=4
#define MENU2_SUB2  72
#define MENU2_SUB3  76
#define MENU2_SUB4  80
#define MENU2_SUB5  84
#define MENU2_SUB6  88
#define MENU2_SUB7  92
//#define MENU2_SUB8  96

//#define MENU3MIN  100
//#define MENU3MAX  104

#define MENU3_SUB1  100 // +=4
#define MENU3_SUB2  104

#define MENU4_SUB1 132
#define MENU4_SUB2 136

#define MENU5_SUB1 164
#define MENU5_SUB2 168
#define MENU5_SUB3 172
#define MENU5_SUB4 176
#define MENU5_SUB5 180
#define MENU5_SUB6 184
#define MENU5_SUB7 188

*/
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

#define OUT_PINOUT 2 // Out pin of the sensor
#define RV_PINOUT 1 // RV output of the sensor
#define TMP_PINOUT 0 // TMP pin of sensor this is temperature output

// function prototypes
void Common_Loop(); 
void ResetCasePrint();
void IO_Settings();
void MicroInit(void);
void Display_ReInit(byte Timer);
void  RTC_Init();
void  SensorInit_Si072(byte);
void  SensorAlt_Init();
void  SensorLight_Init();
void  SensorACccel_GyroInit();
void  Sensors_PeripInit();

void AnalogValRead();
void  AdcRead();
void WindSensorRead();
void  SensorRead_Si072();
void  SensorAlt_Read();
void  SensorLight_Read();
void  SensorAcccel_GyroRead();
void SDS_DustSensor(void);
void UpdateSensorInfo(void);
void EE_SerNoWrite2_EE(unsigned int SerialNo);
void UpdateDeviceEE();
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
void LogEnable(bool Enable);
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
