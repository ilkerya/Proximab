static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
  
//char Disp_MENU1[] =    {'L','O','G',' ','S','T','A','R','T',' ','&',' ','S','T','O','P',' ','M','E','N','U','\0'};
  
//static const char Disp_MENU_NULL_ENT[]  PROGMEM  = "ENTER     "; // 10
//static const char Disp_MENU_NULL_ESC[] PROGMEM = "     ESC";
static const char Disp_MENU_NULL[]    PROGMEM = "ENTER             ESC";
static const char Disp_MENU1[] PROGMEM =     "LOG START & STOP MENU";
static const char Disp_MENU2[] PROGMEM =     "LOG SAMPLE TIME MENU ";
static const char Disp_MENU3[] PROGMEM =     "DISPLAY STANDBYE MENU";
static const char Disp_MENU4[] PROGMEM =     "INFORMATION MENU";
static const char Disp_MENU5[] PROGMEM =     "DATE & TIME ADJ. MENU";
static const char Disp_MENU6[] PROGMEM =     "ENEGY METER CALB MENU";
static const char Disp_MENU5_SUB7[] PROGMEM = "Date & Time Updated !"; 
static const char Disp_MENU6_SUB1[] PROGMEM = "Start Calib? 60 Secs."; 
static const char Disp_MENU6_SUB2[] PROGMEM = "Sure To Start Calib.?"; 
static const char Disp_MENU6_SUB3[] PROGMEM = "Calibration Started !"; 

static const char Disp_MENU1_SUB1[] PROGMEM = "LOG START            "; 
static const char Disp_MENU1_SUB2[] PROGMEM = "LOG STOP             "; 
static const char Disp_MENU1_SUB3[] PROGMEM = "LOG Started !        ";
static const char Disp_MENU1_SUB4[] PROGMEM = "LOG Stopped !        "; 
//static const char Disp_MENU1_SUB4[] PROGMEM = "Stopped !";
static const char Disp_MENU3_SUBMAIN[] PROGMEM = "STANDBYE ";  
static const char Disp_MENU3_SUB1[] PROGMEM = "Enable "; 
static const char Disp_MENU3_SUB2[] PROGMEM = "Disable";  
static const char Disp_MENU3_SUB3[] PROGMEM = "Updated! On";
static const char Disp_MENU3_SUB4[] PROGMEM = "Updated! Off";
  
static const char Disp_MENU2_SUB[] PROGMEM = "Enter -> ";  //9
static const char Disp_MENU2_SUB1[] PROGMEM = " 0.5 Sec    "; //12
static const char Disp_MENU2_SUB2[] PROGMEM = " 1 Sec      "; //12
static const char Disp_MENU2_SUB3[] PROGMEM = " 2 Sec      "; //12
static const char Disp_MENU2_SUB4[] PROGMEM = " 5 Sec      "; //12
static const char Disp_MENU2_SUB5[] PROGMEM = " 10 Sec     "; //12
static const char Disp_MENU2_SUB6[] PROGMEM = " 20 Sec     "; //12
static const char Disp_MENU2_SUB7[] PROGMEM = " 60 Sec     "; //12
static const char Disp_MENU2_SUB8[] PROGMEM = "Sample Time Updated !";

static const char SD1_CARD[] PROGMEM = "SD1 Card "; 
static const char SD2_CARD[] PROGMEM = "SD2 Card ";
static const char SDHC_CARD[] PROGMEM ="SDHC Card "; 
static const char NO_FAT[]  PROGMEM   ="Fat Problem    !";
static const char SD_CARD_ERR[] PROGMEM ="Card Problem    !";

static const char SETTINGUP[] PROGMEM     = "SettingUp"; //21-3
static const char  CALIBRATING[] PROGMEM  = "Calibrating";
static const char  ICERROR[]   PROGMEM    = " IC Error";

#ifdef ENERGYMETER_EXISTS

struct EnergyRegs energyVals;  //Energy register values are read and stored in EnergyRegs structure
struct PowerRegs powerVals;    //Metrology data can be accessed from these structures
struct RMSRegs rmsVals;  
struct PQRegs pqVals;
struct AcalRegs acalVals;
struct Temperature tempVal;

long Igain;
long Vgain;
      
struct
{
  byte Mode = 0;
  byte Timer = 0;
  bool Error = 0;
  bool commscheck;  
}EnergyMeterIC;

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
byte MainMenu =0;
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
 const String KeyWords[12]   = {"Tmp1","Tmp2","Tmp3","Hum1","Hum2","Hum3","PM25","PM10","Cur","Volt","Powr","PF",};
/*
bool I2CSet = 0;
byte I2CTimer = 0;
bool I2Error = 0;
#define ACK_TIMEOUT 12
*/
unsigned int IntTimer250 = 0;
unsigned int  IntTimer500 = 0;
unsigned int  IntTimer1 = 0;
unsigned int  IntTimer2 = 0;
unsigned int  IntTimer5 = 0;
unsigned int  IntTimer10 = 0;
unsigned int  IntTimer20 = 0;
unsigned int  IntTimer60 = 0;


struct
{
  bool Task_250msec =0;
  bool Task_500msec =0;
  bool Task_1Sec =0;
  bool Task_2Sec =0;
  bool Task_5Sec =0;
  bool Task_10Sec =0;
  bool Task_20Sec =0;
  bool Task_60Sec =0;
}Loop;


#define TASK_500MSEC  1
#define TASK_1SEC 2
#define TASK_2SEC 4
#define TASK_5SEC 8
#define TASK_10SEC 16
#define TASK_20SEC 32
#define TASK_60SEC 64  

byte SampleTime = TASK_2SEC; // 250msec 1 // 500 2 // 1Sec 4 // 2sec 8 // 5sec 16 // 10sec 32 // 20sec 64  

// the logging file
File logfile;
String dataString = "";
String LOG_FILE =  "LOG_xxxx.csv";
static const String ConfigFile  = "ADConfig.txt";
String Config_Str = "";

struct
{
  bool LogEnable;  // log on or off  eski LogStatus
  bool LogBootInit; // log on  init case  eski LogStatusInit
  bool Pause;  // At SD Problem Try Again
  unsigned int PauseTimer;
  unsigned int PauseCount;  
  byte Status; 
  bool FatError;
  float Volume;
}SDCard;

struct
{
  unsigned long Total;  // log on or off
  unsigned char Byte; // log on  init case
  unsigned char Kbyte;
  unsigned char Mbyte;
}FileSize;

struct
{
  unsigned int Logger=0;
  byte BoardTimeOut = 0;
  bool BoardTimeOutEnb = 0;  
  bool Left_Rel = 0;
  bool Mid_Rel = 0;
  bool Right_Rel = 0;
  bool Released = 0;
  byte TimeOut = 0;
  bool TimeOutEnb = 0;
}Key;

struct
{
  long No1;
  long No2;
  long No3;    
}SensorId;

//String Sensor_Info_SDS= "";

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesdy", "Wedns.", "Thurs.", "Friday", "Satur."};
//static const char SD_Type[4][14] = {"SD1 Card " , "SD2 Card " , "SDHC Card " , "Card Problem    !"};


const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;

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
#ifdef   ACCL_GYRO_SENSOR_EXISTS 
struct
{
  float x;
  float y; // 27
  float z; //    
}Accelometer;
#endif
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
#define NVRAM_ID1 4
#define NVRAM_ID2 5
#define NVRAM_ID3 6
#define NVRAM_ID4 7

String EE_Id_EString =""; 

String Str_Time="";
String Str_Date="";
String Str_DispTime="";
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
  unsigned int Disp_UpdateTimer=0; 
  unsigned int Disp_Flash=0;     
}DateTimeBuf;


struct
{
  bool RTC_Update=0; 
  bool OLED_Init = 0 ; 
  unsigned char MenuTimeout=0; 
  unsigned char Flash=0; 
  int OLED_Timer = 0; 
  bool InitDelay = 0;
  bool SleepEnable = 0;
  byte ValueTimer = 0; 
  bool ExpSens1 =0;
  bool ExpSens2 =0;
  bool ExpSens3 =0;
  byte SensorRollTimer = 0;
  byte ReInit_Timer = 2;
  bool ReInit_Enable = OFF;
}Display;


/*
unsigned int AD_Value1;
unsigned int AD_Value2;
unsigned int AD_Value3;

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
*/

/*
Display.
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


  
