#ifdef AD9153_PROTOTYPE

#define ARM_MATH_CM0PLUS
#include  <ADE9153A.h>
#include <ADE9153AAPI.h>


int ledState = LOW;
int inputState = LOW;
unsigned long lastReport = 0;
const long reportInterval = 2000;//2000;
const long blinkInterval = 500;
//bool Calib_Done = 0;

////* Basic initializations */
#define SPI_SPEED 1000000     //SPI Speed
#define CS_PIN 8              //8-->Arduino Zero. 15-->ESP8266 
#define ADE9153A_RESET_PIN 4  //On-board Reset Pin
#define USER_INPUT 5          //On-board User Input Button Pin
#define LED 6                 //On-board LED pin
ADE9153AClass ade9153A;
unsigned long currentReport;
      long Igain;
      long Vgain;
      bool commscheck;
//ADE9153AClass cc;
//SetupADE9153A

//ADE9153AClass ade9153A = ADE9153AClass();
//ADE9153AClass ade9153A = SetupADE9153A();

struct EnergyRegs energyVals;  //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  
struct PQRegs pqVals;
struct AcalRegs acalVals;
struct Temperature tempVal;

String SETTINGUP   = "SettingUp  IC"; //21-3
String CALIBRATING = "Calibrting IC";
String ICERROR     = "  IC Error ";


//String SETTINGUP   = "Se"; //21-2
//String CALIBRATING = "Ca";
//String ICERROR    = "Po";

void readandwrite(void);
void resetADE9153A(void);
void Operating_Modes(void);

struct
{
 // byte Modes;
  byte Timer = 0;
  bool Error = 0;
}PowerIC;

unsigned char PowerIC_Mode;

#endif  


// so variables

String Display_Line1 ="Display.........Line1"; 
String Display_Line2 ="Display........Line2."; 
String Display_Line3 ="Display.......Line3.."; 
String Display_Line4 ="Display......Line4..."; 
String Display_Line5 ="Display.....Line5....";
String Display_Line6 ="Display....Line6.....";
String Display_Line7 ="Display...Line7......";
String Display_Line8 ="Display..Line8.......";
String FW_Version ="" ;
byte Menu =0;
unsigned int SerialCode =0;
byte DispRollIndex[4] = {1,0,0,0};
//2 1 0 0
//3 2 1 0
//4 3 2 1
//5 4 3 2
//6 5 4 3
//7 6 5 4
//8 7 6 5
//1 8 7 6
//2 1 8 7
//3 2 1 8 

float RL1Min, RL1Max, RL2Min,RL2Max,CompValue;
String RLlVal, RL2Val;
String Relay1str, RlStr2, RlStr4, Relay2str, RlStr6,  RlStr8;
#define ELEMENTS 12
String KeyWords[12] = {"Tmp1","Tmp2","Tmp3","Hum1","Hum2","Hum3","PM25","PM10","Cur","Volt","Powr","PF",};

unsigned int KeyLogger=0;

bool KeyLeft_Rel = 0;
bool KeyMid_Rel = 0;
bool KeyRight_Rel = 0;
bool KeyReleased = 0;

unsigned int OLED_Timer = 0;
bool OLED_Init = 0 ;

//unsigned int Timer =0;

unsigned int IntTimer250 = 0;
unsigned int  IntTimer500 = 0;
unsigned int  IntTimer1 = 0;
unsigned int  IntTimer2 = 0;
unsigned int  IntTimer5 = 0;
unsigned int  IntTimer10 = 0;
unsigned int  IntTimer20 = 0;
unsigned int  IntTimer60 = 0;

bool LoopTask_250msec =0;
bool LoopTask_500msec =0;
bool LoopTask_1Sec =0;
bool LoopTask_2Sec =0;
bool LoopTask_5Sec =0;
bool LoopTask_10Sec =0;
bool LoopTask_20Sec =0;
bool LoopTask_60Sec =0;

#define TASK_500MSEC  1
#define TASK_1SEC 2
#define TASK_2SEC 4
#define TASK_5SEC 8
#define TASK_10SEC 16
#define TASK_20SEC 32
#define TASK_60SEC 64  

byte SampleTime = TASK_2SEC; // 250msec 1 // 500 2 // 1Sec 4 // 2sec 8 // 5sec 16 // 10sec 32 // 20sec 64  

//byte Sample_LogTime = 4;// default 2 sec



struct
{
  bool LogEnable;  // log on or off  eski LogStatus
  bool LogBootInit; // log on  init case  eski LogStatusInit
  bool Pause;  // At SD Problem Try Again
  unsigned int PauseTimer;
  unsigned int PauseCount;  
  byte Status; 
}SDCard;



//bool LogPause = 0;
//bool SD_KartStop = 0;
//int SDCardStatus = 0;


bool DispExpSens1 =0;
bool DispExpSens2 =0;
bool DispExpSens3 =0;

bool SensorRollKey = 0;
byte SensorRollTimer = 0;

byte KeyBoardTimeOut = 0;
bool KeyBoardTimeOutEnb = 0;

unsigned int Current_Mains_Raw;
unsigned int Current_Mains_Raw_Trim;
float Current_Mains;
float Current_MainsAverage;
float    Current_Mains_Rms ;
float    Current_Mains_Av ;
unsigned int CurrentArray[10];
byte CurrentIndexer = 0;
unsigned int TrimpotAdc ;
unsigned int Mains_Volt_Raw ;
unsigned int Mains_Volt ;

bool DisplayInitDelay = 0;
bool DisplaySleepEnable = 0;

byte DisplayValueTimer = 0;

String Sensor1_Id = "";
String Sensor2_Id = "";
String Sensor3_Id = "";
String Sensor_Info_SDS= "";

// the logging file
File logfile;

String LOG_FILE =  "LOG_xxxx.csv";
//#define LOG_FILE "AD_Log.csv"

struct
{
  unsigned long Total;  // log on or off
  unsigned char Byte; // log on  init case
  unsigned char Kbyte;
  unsigned char Mbyte;
}FileSize;






String EE_Id_EString =""; 

String deBugString = "Start.....";  // Max 10 caharacter

    String dataString = "";
  //   String DataStr = "";   
   // String Date_Time = "";
    String Str_Time="";
    String Str_Date="";
    String Str_DispTime="";
    float SD_Volume;
    String SD_TypeString = "";
 //   bool SD_Card_Reset = 1;   
    byte RTC_Test = 2;
    String incomingStr ="";
    int incomingByte = 0;
  //  int Character_Length = 0;

/*
char DisplayLine[40] = {'1','2','3','4','5','6','7','8','9','0',
                        'a','b','c','d','e','f','g','h','i','j',
                        'k','l','m','n','o','p','q','r','s','t',
                        'u','v','y','z','-','?','&','%','*','#'};
*/                        

#define DEF_START_NULL_LINES 5
#define DEF_END_LINE_INDEX 21
                        
byte IndxStr = DEF_START_NULL_LINES;
byte IndxEnd = DEF_END_LINE_INDEX;

byte IndxCount = 0;
byte DisplayCounter = 0;

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;


unsigned int AD_Value1;
unsigned int AD_Value2;
unsigned int AD_Value3;

#define BUF_LENGTH 60
byte PMBuffer[BUF_LENGTH];

struct
{
  float Humidity_Ch1;
  float TemperatureSi072_Ch1; // 27
  float Humidity_Ch2;
  float TemperatureSi072_Ch2; // 27
  float Humidity_Ch3;
  float TemperatureSi072_Ch3; // 27  
  float Current;
  float Voltage;
  float PowerFactor; 
  float ActivePower;
  float Frequency;
  float Pressure; //  
  float TemperatureBMP; //  
  float Altitude; //
  float PM25=0; //
  float PM10=0; //   
  float WindRaw;   // 35        
  unsigned int WindMPH;    // 31
  unsigned int WindTemp;   // 35
  unsigned int Luminosity;  
}Values;

struct
{
  float x;
  float y; // 27
  float z; //    
}Accelometer;
/*
struct
{
  float x;
  float y; // 27
  float z; //    
}Gyro;
*/

#define ADDRES_LOG 8
#define SLEEP_LOG 16
void EESetResetLog(bool Mode);
void EEReadLog(void);
void EESetSampleTimeLog(byte Sample);
void EEDisplaySleep(bool Mode);

struct
{
  unsigned int Year=0; 
  unsigned int Month=0; 
  unsigned int Day=0;                       
  unsigned int Hour=0; 
  unsigned int Minute=0; 
  unsigned int Second=0;
  bool Init=0; 
  bool RTC_Update=0;  
 // bool Disp_Update=0;  
  unsigned int Disp_UpdateTimer=0; 
  unsigned int Disp_Flash=0;     
}DateTimeBuf;


struct
{

  bool RTC_Update=0;  
 // bool Disp_Update=0;  
  unsigned char MenuTimeout=0; 
  unsigned char Flash=0;     
}Display;
/*

// for wind sensor
float zeroWindAdjustment = 0.1;             // individual adjustment parameter for wind sensor (standard = 0.1)
const double F_A_temp = 1.00857557;       // constants for calibration factors of Y=A*X+B equation for air temperature, relative humidity, and globe temperature
const double F_B_temp = -0.872682478;      // based on own calibration data - see reference on authors' website
const double F_A_hum = 1.087101959;
const double F_B_hum = -7.173821971;
const double F_A_globe = 0.946524358;
const double F_B_globe = 0.698608493;

float RV_ADunits;                       // variable for analog signal readings of wind sensor
float RV_ADunits_dev1000;               // variable for adjusted analog signal readings of wind sensor
const float T_rev_wind = 21.9;          // constants for wind speed calculation - based on own calibration data - see reference on authors' website
const float v_calc_F1 = 15.36921;
const float v_calc_F2 = 0.01788;
const float v_calc_F3 = -29.04560;
const float v_calc_F4 = 1.01536;
double velReading = 0;
double tempReading = 25;   //default     // variables for sensor readings' temporary storage


*/


  
