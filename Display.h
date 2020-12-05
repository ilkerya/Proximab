
//https://github.com/adafruit/Adafruit_SSD1306

// https://upload.wikimedia.org/wikipedia/commons/f/f8/Codepage-437.png

#ifdef FIRST_PROTOTYPE
  #define OLED_GND 46// 13
  #define OLED_POWER 44// 13
  #define OLED_CS    40// 13
  #define OLED_RESET 38
  #define OLED_DC    36// 12  // common 50
  #define OLED_CLK   34 //13 // common  52
  #define OLED_MOSI  32// 11 //common 51
#endif
#ifdef AD9153_PROTOTYPE

  #define OLED_GND 42// 13
  #define OLED_POWER 40// 13
  #define OLED_CS    36// 13
  #define OLED_RESET 34
  #define OLED_DC    32// 12  // common 50
  #define OLED_CLK   30 //13 // common  52
  #define OLED_MOSI  28// 11 //common 51
 
#endif

/*
#define OLED_CS    A0
#define OLED_RESET A1
#define OLED_DC    A2
#define OLED_CLK   A3
#define OLED_MOSI  A4
*/

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
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
 
  /*
 * In Adafruit_SSD1306.h
 * 1. uncomment #define SSD1306_128_64
2. comment #define SSD1306_128_32
3. comment #define SSD1306_96_16
In the example ssd1306_128x64_i2c
4. add #define SSD1306_LCDHEIGHT 64
 *
 */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

  /*
#ifdef SSD1306_LCDHEIGHT
  #define SSD1306_LCDHEIGHT 64
#endif  
#ifndef SSD1306_LCDHEIGHT
  #define SSD1306_LCDHEIGHT 64
#endif  

//#define SSD1306_LCDHEIGHT 64


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
*/
//Adafruit_SSD1306 display(OLED_RESET);
//DISPLAY INITIALIZER
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
void Display_SwitchOff(){
     // digitalWrite(OLED_POWER, LOW);       // turn on pullup resistors
     // digitalWrite(OLED_GND, LOW);       // keep GND Level   
      Menu = MENU_NULL;    
      display.clearDisplay(); 
      display.display();  
}
void DisplaySetPowerIO(){
     //-- DISPLAY INIT --//
    pinMode(OLED_GND, OUTPUT);
    digitalWrite(OLED_GND, LOW);       // keep GND Level           
    pinMode(OLED_POWER, OUTPUT);
    digitalWrite(OLED_POWER, HIGH);       // turn on pullup resistors    
}
void Display_ReInit(byte Timer){
    DisplaySetPowerIO();
    OLED_Timer = Timer; // 10 sec
  //  delay(300); // Pause for 2 seconds 
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {  //    SSD1306_EXTERNALVCC
        Serial.println("SSD1306 allocation failed");
            //for(;;); // Don’t proceed, loop forever
    }
     Serial.println("Dsiplay ReInitilized");  
 //   display.clearDisplay();
 //   display.setTextSize(3);
 //   display.setTextColor(WHITE);  //0  white on black
 //    display.setTextColor(0);  //1     Black on white
    display.dim(1); // lower brightness 
 //   display.setCursor(0, 0);
 //   display.println();
//    display.println("DATALOG"); 
 //   display.display();
}

 

void DisplayInit(void){
    DisplaySetPowerIO();
    //-- DISPLAY INIT --//    
    OLED_Timer = 40; // 20-> 10 sec
    delay(300); // Pause for 2 seconds
    
    Serial.println("Display Initing");
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {  //  SSD1306_EXTERNALVCC
        Serial.println("SSD1306 allocation failed");
  
     // for(;;); // Don’t proceed, loop forever
      }
       display.dim(1); // lower brightness 
   // display.begin(SSD1306_SWITCHCAPVCC);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);  //0  white on black
 //    display.setTextColor(0);  //1     Black on white
    display.setCursor(0, 0);
    display.println();
    display.println("DATALOG"); 
      display.display();
  //    delay(250);
     // Adafruit_SSD1306::dim  ( 1 ) //1 == lower brightness // 0 == full brightness
      //display.dim       
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
 // delay(2000);
}


void UpdateDispSpChar(byte Index,byte Line){
   switch(Line){
      case 4:display.setCursor(36, 24);  //x,y                            
      break; 
      case 5:display.setCursor(36, 32);                         
      break;
      case 6:display.setCursor(36, 40);
      break;
      case 7:display.setCursor(36, 48);           
      break;
      default:              // 
      break;
    } 
    switch(Index){
      case 0: //// show nothing                 
      break;    
      /*
      case 1:    //"FW " + FW_Version;  // fw version compile time             
      break;
      case 2:        // "Dev Id:  " + EE_Id_EString;  // device id           
      break;
      */
      case 1:
           if(DispExpSens1 ){
                DispExpSens1 = OFF;
                display.write(247); // 5th character  '°';   
          }   
      break;    
      case 2:
             if(DispExpSens2 ){
                DispExpSens2 = OFF;
                display.write(247); // 5th character  '°';   
            } 
      break;     
      case 3:      
           if(DispExpSens3 ){
                DispExpSens3 = OFF;
                display.write(247); // 5th character  '°';   
          }  
      break;
      case 6:           
       break;
      case 7:
      break;
      default:              // 
      break;
    }
}



void displayValues(void)
{
  UpdateDisplayBuffer();
  if(DisplayInitDelay == OFF)return;
  //testdrawchar();
  //return;

    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print(Display_Line1);   //10

    display.setCursor(0, 8);
    display.print(Display_Line2);   //10
  
    display.setCursor(0, 16);
    display.print(Display_Line3);   //10 

    display.setCursor(0, 24);
    display.print(Display_Line4);   //10 

    display.setCursor(0, 32);
    display.print(Display_Line5);   //10 

    display.setCursor(0, 40);
    display.print(Display_Line6);   //10 

    display.setCursor(0, 48);
    display.print(Display_Line7);   //10 

      UpdateDispSpChar(DispRollIndex[0], 4); // Line 4
      UpdateDispSpChar(DispRollIndex[1], 5); // Line 5
      UpdateDispSpChar(DispRollIndex[2], 6); // Line 6
      UpdateDispSpChar(DispRollIndex[3], 7); // Line 7
 
    display.setCursor(0, 56); // 8th line
    display.print(Display_Line8);   //10 
/*
    if( DispExpLin8_1){
      DispExpLin8_1 = OFF;
      display.setCursor(60, 56); //x,y  6*10 = 60
      display.write(30); // 5th character  '>';  

      display.setCursor(72, 56); //x,y  6*12 = 72
      display.write(31); // 5th character  '<';        
    }
    */
  switch(Menu){

      case MENU_NULL :
           display.setCursor(60, 56); //x,y  6*10 = 60
           display.write(30); // 5th character  '>';  
           display.setCursor(72, 56); //x,y  6*12 = 72
           display.write(31); // 5th character  '<';  
       break;      
    case MENU5_SUB1 :

        break;       
    default: 
    break;
  }
    

   display.display();
}

//String Disp_MENU_NULL = "ENTER  UP DOWN    ESC";
String Disp_MENU_NULL_ENT = "ENTER     "; // 10
String Disp_MENU_NULL_ESC = "     ESC";

String Disp_MENU1 =     "LOG START & STOP MENU";
String Disp_MENU2 =     "LOG SAMPLE TIME MENU ";
String Disp_MENU3 =     "DISPLAY STANDBYE MENU";
String Disp_MENU4 =     "INFORMATION MENU     ";
String Disp_MENU5 =     "DATE & TIME ADJ. MENU";
String Disp_MENU6 =     "POWER IC CALIBR. MENU";

String Disp_MENU5_SUB7= "Date & Time Updated !"; 

String Disp_MENU6_SUB1= "Start Calib? 60 Secs."; 
String Disp_MENU6_SUB2= "Sure To Start Calib.?"; 
String Disp_MENU6_SUB3= "Calibration Started !"; 

//String Disp_MENU1_SUB1= "START LOG? If Yes ENT"; 
//String Disp_MENU1_SUB2= "STOP LOG? If Yes ENT ";


String Disp_MENU1_SUBMAIN= "LOG "; 
String Disp_MENU1_SUB1= "START"; 
String Disp_MENU1_SUB2= "STOP ";
String Disp_MENU1_SUB3= "Started !"; 
String Disp_MENU1_SUB4= "Stopped !";

String Disp_MENU3_SUBMAIN= "STANDBYE ";  
String Disp_MENU3_SUB1= "Enable "; 
String Disp_MENU3_SUB2= "Disable";  
String Disp_MENU3_SUB3= "Updated! On";
String Disp_MENU3_SUB4= "Updated! Off";
  
String Disp_MENU2_SUB= "Enter -> ";  //9
String Disp_MENU2_SUB1= " 0.5 Sec    "; //12
String Disp_MENU2_SUB2= " 1 Sec      "; //12
String Disp_MENU2_SUB3= " 2 Sec      "; //12
String Disp_MENU2_SUB4= " 5 Sec      "; //12
String Disp_MENU2_SUB5= " 10 Sec     "; //12
String Disp_MENU2_SUB6= " 20 Sec     "; //12
String Disp_MENU2_SUB7= " 60 Sec     "; //12
String Disp_MENU2_SUB8= "Sample Time Updated !";

bool DisplayFlash(void){
        Display.Flash++;
        if(Display.Flash > 2) {
          Display.Flash = 0;
          return OFF;
        }
        return ON;
}

void UpddateDateTimeBuffer(void){
        String str =  String(DateTimeBuf.Year) + '.';
        if(DateTimeBuf.Month < 10) str += '0';
        str += String(DateTimeBuf.Month) + '.';
        if(DateTimeBuf.Day < 10) str += '0';
        str += String(DateTimeBuf.Day) + "   ";
        if(DateTimeBuf.Hour < 10) str += '0';
        str += String(DateTimeBuf.Hour) + ':';
        if(DateTimeBuf.Minute < 10) str += '0';
        str += String(DateTimeBuf.Minute) + ':';  
        if(DateTimeBuf.Second < 10) str += '0';
        str += String(DateTimeBuf.Second);  
        Display_Line8 = str;
}


void UpdateDisplayMenu(void){
  
  switch(Menu){
    case MENU_NULL : Display_Line8 = Disp_MENU_NULL_ENT;
                     Display_Line8 += "   ";
                     Display_Line8 += Disp_MENU_NULL_ESC;
                      
      break;
    case MENU1 :  Display_Line8 = Disp_MENU1;
      break;
     case MENU1_SUB1 :    Display_Line8 = Disp_MENU1_SUBMAIN + Disp_MENU1_SUB1;
        if(DisplayFlash() == 0){     
          Display_Line8[4] = ' ';
          Display_Line8[5] = ' ';
          Display_Line8[6] = ' ';
          Display_Line8[7] = ' '; 
          Display_Line8[8] = ' ';        
        } 
      break;
    case MENU1_SUB2 :     Display_Line8 = Disp_MENU1_SUBMAIN + Disp_MENU1_SUB2;
           if(DisplayFlash() == 0){     
          Display_Line8[4] = ' ';
          Display_Line8[5] = ' ';
          Display_Line8[6] = ' ';
          Display_Line8[7] = ' '; 
          Display_Line8[8] = ' '; 
        }  
      break;
    case MENU1_SUB3 : Display_Line8 = Disp_MENU1_SUBMAIN + Disp_MENU1_SUB3;

      break;
    case MENU1_SUB4 : Display_Line8 = Disp_MENU1_SUBMAIN + Disp_MENU1_SUB4;    
      break;

           
    case MENU2 :   Display_Line8 = Disp_MENU2;
      break;
    case MENU2_SUB1 : Display_Line8 = Disp_MENU2_SUB +Disp_MENU2_SUB1;
          if(DisplayFlash() == OFF){     
              Display_Line8[10] = ' ';
              Display_Line8[11] = ' '; 
              Display_Line8[12] = ' '; 
        }      
      break;
    case MENU2_SUB2 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB2; 
              if(DisplayFlash() == OFF)Display_Line8[10] = ' ';        
      break;
    case MENU2_SUB3 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB3;
             if(DisplayFlash() == OFF)Display_Line8[10] = ' ';
      break;
    case MENU2_SUB4 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB4;
             if(DisplayFlash() == OFF)Display_Line8[10] = ' ';
      break;
    case MENU2_SUB5 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB5;
              if(DisplayFlash() == OFF){     
                Display_Line8[10] = ' ';
                Display_Line8[11] = ' ';        
            }    
      break;
    case MENU2_SUB6 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB6;
              if(DisplayFlash() == OFF){     
                Display_Line8[10] = ' ';
                Display_Line8[11] = ' ';        
            }     
      break;
    case MENU2_SUB7 : Display_Line8 = Disp_MENU2_SUB + Disp_MENU2_SUB7; 
              if(DisplayFlash() == OFF){     
                Display_Line8[10] = ' ';
                Display_Line8[11] = ' ';        
            }     
      break;
    case MENU2_SUB8 :Display_Line8 = Disp_MENU2_SUB8;
        break;   
    case MENU3 :   Display_Line8 = Disp_MENU3;
      break;
    case MENU3_SUB1 : Display_Line8 = Disp_MENU3_SUBMAIN + Disp_MENU3_SUB1;
              if(DisplayFlash() == OFF){     
              Display_Line8[9] = ' '; 
              Display_Line8[10] = ' ';
               Display_Line8[11] = ' ';
              Display_Line8[12] = ' '; 
              Display_Line8[13] = ' '; 
              Display_Line8[14] = ' '; 
               Display_Line8[15] = ' ';               
        }  
      break;
    case MENU3_SUB2 :  Display_Line8 = Disp_MENU3_SUBMAIN + Disp_MENU3_SUB2;
         if(DisplayFlash() == OFF){              
              Display_Line8[9] = ' '; 
              Display_Line8[10] = ' ';
               Display_Line8[11] = ' ';
              Display_Line8[12] = ' '; 
              Display_Line8[13] = ' '; 
              Display_Line8[14] = ' '; 
               Display_Line8[15] = ' ';                            
        }
              break;  
       case MENU3_SUB3 :Display_Line8 = Disp_MENU3_SUBMAIN + Disp_MENU3_SUB3;
        break;  
       case MENU3_SUB4 :Display_Line8 = Disp_MENU3_SUBMAIN + Disp_MENU3_SUB4;
        break;
      
    case MENU4 :   Display_Line8 = Disp_MENU4; 
      break;
    case MENU4_SUB1 :  
            Display_Line8 = "Fw:  " + FW_Version;  // fw version compile time 21- 14 = 7 
      break;  
    case MENU4_SUB2 :   
          Display_Line8 = "Dev Id: " + EE_Id_EString;  // fw version compile time
      break;
          
    case MENU5 :   Display_Line8 = Disp_MENU5;
      break;
    case MENU5_SUB1 :UpddateDateTimeBuffer();
        if(DisplayFlash() == 0){     
          Display_Line8[0] = ' ';
          Display_Line8[1] = ' ';
          Display_Line8[2] = ' ';
          Display_Line8[3] = ' ';   
        }                   
      break;
    case MENU5_SUB2 :UpddateDateTimeBuffer();
        if(DisplayFlash() == 0){     
          Display_Line8[5] = ' ';
          Display_Line8[6] = ' '; 
        }  
      break;
    case MENU5_SUB3 :UpddateDateTimeBuffer();
        if(DisplayFlash() == 0){     
          Display_Line8[8] = ' ';
          Display_Line8[9] = ' '; 
        }  
      break;    
     
    case MENU5_SUB4 :UpddateDateTimeBuffer();
          if(DisplayFlash() == 0){     
          Display_Line8[13] = ' ';
          Display_Line8[14] = ' '; 
        }    
      break;
    case MENU5_SUB5 :UpddateDateTimeBuffer();
        if(DisplayFlash() == 0){     
          Display_Line8[16] = ' ';
          Display_Line8[17] = ' '; 
        }    
      break;   
          
    case MENU5_SUB6 :UpddateDateTimeBuffer();
        if(DisplayFlash() == 0){     
          Display_Line8[19] = ' ';
          Display_Line8[20] = ' '; 
        }   
      break;  
    case MENU5_SUB7 :Display_Line8 = Disp_MENU5_SUB7;
        break; 


       case MENU6 :Display_Line8 = Disp_MENU6;
        break;
       case MENU6_SUB1 :Display_Line8 = Disp_MENU6_SUB1;
        break;
       case MENU6_SUB2 :Display_Line8 = Disp_MENU6_SUB2;
        break;
       case MENU6_SUB3 :Display_Line8 = Disp_MENU6_SUB3;
        break; 



      
/*      
    case MENU2_SUB1 : Display_Line8 = Disp_MENU2_SUB;Display_Line8 += Disp_MENU2_SUB1;   
      break;
    case MENU2_SUB2 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB2;  
      break;
    case MENU2_SUB3 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB3;
      break;
    case MENU2_SUB4 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB4;
      break;
    case MENU2_SUB5 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB5;   
      break;
    case MENU2_SUB6 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB6;
      break;
    case MENU2_SUB7 : Display_Line8 = Disp_MENU2_SUB; Display_Line8 += Disp_MENU2_SUB7; 
      break;
  */

      

            

    default: 
    break;
  }
}


void DisplayFullSensors(void){
    display.print("x");
    display.print(Accelometer.x); //6
    
    display.print(" y");
    display.print(Accelometer.y); //7

     display.print(" z");
     display.println(Accelometer.z); //7     
    
    
    display.print(Values.TemperatureSi072_Ch1,1);
   //  display.print("°");                     
    display.print("C %");
    display.print(Values.Humidity_Ch1,1); //
    display.print(" Lm:");
    display.println(Values.Luminosity); //

    display.print(Values.Pressure,0);
    display.print("hPA ");
    display.print(Values.TemperatureBMP,1);
   //  display.print("\0x7F");   
    display.print("C ");  
    display.print(Values.Altitude,1);
    display.println("m"); 
}
