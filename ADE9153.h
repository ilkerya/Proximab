
#ifdef AD9153_PROTOTYPE

void resetADE9153A(void)
{
    digitalWrite(ADE9153A_RESET_PIN, LOW);
    delay(100);
    digitalWrite(ADE9153A_RESET_PIN, HIGH);
    delay(1000);
    Serial.println("Reset Done");
}

void runLength( long seconds)
{
  /*
  unsigned long startTime = millis();
  unsigned int COEF = 1000;// original 1000
      #ifdef ARDUINO_MEGA // 8 bit AVR  
     // wdt_reset();
     // wdt_enable(WDTO_8S);
      #endif
  while (millis() - startTime < (seconds*COEF)){
    digitalWrite(LED, HIGH);
    delay(blinkInterval);
    digitalWrite(LED, LOW);
    delay(blinkInterval);
  }
  */  
}

void ADsetup() {
  /* Pin and serial monitor setup */
    pinMode(LED, OUTPUT);
    pinMode(USER_INPUT, INPUT);
    pinMode(ADE9153A_RESET_PIN, OUTPUT);
    digitalWrite(ADE9153A_RESET_PIN, HIGH);  
  resetADE9153A();            //Reset ADE9153A for clean startup
  delay(1000);
  /*SPI initialization and test*/
  bool commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
  if (!commscheck) {
    Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");  
    while (!commscheck) {     //Hold until arduino is reset
      break;
    
     // delay(1000);
    }
  }
  
  ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
  /* Read and Print Specific Register using ADE9153A SPI Library */
  Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
  ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
  //delay(500); 
}
void readandwrite()
{ 
 /* Read and Print WATT Register using ADE9153A Read Library */
  ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(&rmsVals);
  ade9153A.ReadPQRegs(&pqVals);
  ade9153A.ReadTemperature(&tempVal);

  Values.Current = rmsVals.CurrentRMSValue/1000;
 // if(Values.Current < 0.00) Values.Current = 0.00;
  
  Serial.print("RMS Current:\t");Serial.print(Values.Current); Serial.println(" A         ");
  //Serial.print(rmsVals.CurrentRMSValue/1000); Serial.println(" A");

  Values.Voltage = rmsVals.VoltageRMSValue/1000;
  
  Serial.print("RMS Voltage:\t");       Serial.print(Values.Voltage);Serial.println(" V      ");
  //Serial.print(rmsVals.VoltageRMSValue/1000); Serial.println(" V");
  

  Values.ActivePower = powerVals.ActivePowerValue/1000;
//  if(Values.ActivePower < 0.00) Values.ActivePower = 0.00;
  
  Serial.print("Active Power:\t");        
  Serial.print(Values.ActivePower);
  Serial.println(" W");
  
  Serial.print("Reactive Power:\t");        
  Serial.print(powerVals.FundReactivePowerValue/1000);
  Serial.println(" VAR");
  
  Serial.print("Apparent Power:\t");        
  Serial.print(powerVals.ApparentPowerValue/1000);
  Serial.println(" VA");
  
  Values.PowerFactor = pqVals.PowerFactorValue;
  
  Serial.print("Power Factor:\t");        
  Serial.println(Values.PowerFactor);

   Values.Frequency =  pqVals.FrequencyValue;
  
  Serial.print("Frequency:\t");        
  Serial.print(pqVals.FrequencyValue);
  Serial.println(" Hz");
  
  Serial.print("Temperature:\t");        
  Serial.print(tempVal.TemperatureVal);
  Serial.println(" degC");

}

// Call in 2 seconds
void Operating_Modes(void){
         
  switch(PowerIC_Mode){   
    case POWERIC_SETUP1:
      pinMode(LED, OUTPUT);
      pinMode(USER_INPUT, INPUT);
      pinMode(ADE9153A_RESET_PIN, OUTPUT);
      digitalWrite(ADE9153A_RESET_PIN, HIGH);  
     // resetADE9153A();            //Reset ADE9153A for clean startup 
      digitalWrite(ADE9153A_RESET_PIN, LOW);
      delay(10);
      digitalWrite(ADE9153A_RESET_PIN, HIGH);
     // delay(1000);
    Serial.println("Reset Done");       
      PowerIC.Timer = 1; //20sec
      PowerIC_Mode = POWERIC_SETUP2;//
     // PowerIC_Mode++;   
    break;
    case POWERIC_SETUP2:
          if(PowerIC.Timer)PowerIC.Timer--;
          else PowerIC_Mode = POWERIC_SETUP3;//PowerIC_Mode++;    
    break;
    case POWERIC_SETUP3:
         commscheck = ade9153A.SPI_Init(SPI_SPEED,CS_PIN); //Initialize SPI
      //   commscheck = OFF; // TEST ERROR CASE
        if (!commscheck) {
           Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");  
            PowerIC.Error = ON;
            PowerIC_Mode = POWERIC_SETUP1; 
        }
        else {
          ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
          /* Read and Print Specific Register using ADE9153A SPI Library */
          Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
          ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
          PowerIC.Error = OFF;
         // PowerIC_Mode++;   
          PowerIC_Mode = POWERIC_NORMAL ; 
        }
    break;
    case 3://POWERIC_NORMAL:           // Running Normal Mode
    
          //  readandwrite();  
  ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
  ade9153A.ReadRMSRegs(&rmsVals);
  ade9153A.ReadPQRegs(&pqVals);
  ade9153A.ReadTemperature(&tempVal);

  Values.Current = rmsVals.CurrentRMSValue/1000;
 // if(Values.Current < 0.00) Values.Current = 0.00;
  
  Serial.print("RMS Current:\t");Serial.print(Values.Current); Serial.println(" A         ");
  //Serial.print(rmsVals.CurrentRMSValue/1000); Serial.println(" A");

  Values.Voltage = rmsVals.VoltageRMSValue/1000;
  
  Serial.print("RMS Voltage:\t");       Serial.print(Values.Voltage);Serial.println(" V      ");
  //Serial.print(rmsVals.VoltageRMSValue/1000); Serial.println(" V");
  

  Values.ActivePower = powerVals.ActivePowerValue/1000;
//  if(Values.ActivePower < 0.00) Values.ActivePower = 0.00;
  
  Serial.print("Active Power:\t");        
  Serial.print(Values.ActivePower);
  Serial.println(" W");
  
  Serial.print("Reactive Power:\t");        
  Serial.print(powerVals.FundReactivePowerValue/1000);
  Serial.println(" VAR");
  
  Serial.print("Apparent Power:\t");        
  Serial.print(powerVals.ApparentPowerValue/1000);
  Serial.println(" VA");
  
  Values.PowerFactor = pqVals.PowerFactorValue;
  
  Serial.print("Power Factor:\t");        
  Serial.println(Values.PowerFactor);

   Values.Frequency =  pqVals.FrequencyValue;
  
  Serial.print("Frequency:\t");        
  Serial.print(pqVals.FrequencyValue);
  Serial.println(" Hz");
  
  Serial.print("Temperature:\t");        
  Serial.print(tempVal.TemperatureVal);
  Serial.println(" degC");
            
    //    if (digitalRead(USER_INPUT) == LOW)PowerIC_Mode=POWERIC_CALB1;// Set to calibration   
       
    break;
    case POWERIC_CALB1: // Calibrating The Current Channel Started           
        ade9153A.StartAcal_AINormal();
        PowerIC.Timer = 10; //20sec
        Serial.println("Autocalibrating Current Channel Started");
        PowerIC_Mode++;         
    break;
    case POWERIC_CALB2: // Calibrating The Current Channel Continues
          if(PowerIC.Timer)PowerIC.Timer--;
          else PowerIC_Mode++;
          digitalWrite(LED, !digitalRead(LED));
    break;  
    case POWERIC_CALB3: // Calibrating The Current Channel Ends
        ade9153A.StopAcal();      
        Serial.println("Autocalibrating Current Channel Ended"); 
        PowerIC_Mode++;
    break;
    case POWERIC_CALB4: // Calibrating The Voltage Channel Starts
        ade9153A.StartAcal_AV();
        PowerIC.Timer = 20; // 40 sec
        Serial.println("Autocalibrating Voltage Channel Started");   
        PowerIC_Mode++;               
    break;
    case POWERIC_CALB5: // Calibrating The Voltage Channel continues
          if(PowerIC.Timer)PowerIC.Timer--;
          else PowerIC_Mode++; 
          digitalWrite(LED, !digitalRead(LED));  
    break;    
    case POWERIC_CALB6: // Calibrating The Voltage Channel Ends
         ade9153A.StopAcal();
         PowerIC.Timer = 1;
         Serial.println("Autocalibrating Voltage Channel Ended"); 
         PowerIC_Mode++;   
    break;
    case POWERIC_CALB7: // wait to Read the Calibration Registers
          if(PowerIC.Timer)PowerIC.Timer--;
          else PowerIC_Mode++; 
    break; 
    case POWERIC_CALB8: // Final for Calibration Read The registers and go to normal Reading
      ade9153A.ReadAcalRegs(&acalVals);
      Serial.print("AICC: ");
      Serial.println(acalVals.AICC);
      Serial.print("AICERT: ");
      Serial.println(acalVals.AcalAICERTReg);
      Serial.print("AVCC: ");
      Serial.println(acalVals.AVCC);
      Serial.print("AVCERT: ");
      Serial.println(acalVals.AcalAVCERTReg);
      
      Igain = (-(acalVals.AICC / 838.190) - 1) * 134217728;
      Vgain = ((acalVals.AVCC / 13411.05) - 1) * 134217728;
      ade9153A.SPI_Write_32(REG_AIGAIN, Igain);
      ade9153A.SPI_Write_32(REG_AVGAIN, Vgain);
      
      Serial.println("Autocalibration Complete");
      PowerIC.Timer = 1;
      PowerIC_Mode++;
    break; 
    case POWERIC_CALB9:
          if(PowerIC.Timer)PowerIC.Timer--;
          else PowerIC_Mode++; 
    break; 
    case POWERIC_CALB10:
      PowerIC_Mode = POWERIC_NORMAL;
    break;   
    default:
      PowerIC_Mode = POWERIC_SETUP1;    
    break;
  }  
}

void ADE9153_Calibration() {
   // Calib_Done = ON;return;

    Serial.println("Autocalibrating Current Channel");        
    ade9153A.StartAcal_AINormal();
    runLength(20);     
    ade9153A.StopAcal();          
    Serial.println("Autocalibrating Voltage Channel");
    ade9153A.StartAcal_AV();
    runLength(40);
    ade9153A.StopAcal();  
    delay(100);  
    ade9153A.ReadAcalRegs(&acalVals);
    Serial.print("AICC: ");
    Serial.println(acalVals.AICC);
    Serial.print("AICERT: ");
    Serial.println(acalVals.AcalAICERTReg);
    Serial.print("AVCC: ");
    Serial.println(acalVals.AVCC);
    Serial.print("AVCERT: ");
    Serial.println(acalVals.AcalAVCERTReg);
    long Igain = (-(acalVals.AICC / 838.190) - 1) * 134217728;
    long Vgain = ((acalVals.AVCC / 13411.05) - 1) * 134217728;
    ade9153A.SPI_Write_32(REG_AIGAIN, Igain);
    ade9153A.SPI_Write_32(REG_AVGAIN, Vgain);
    Serial.println("Autocalibration Complete");
    delay(2000);
}

void AD_Loop() {
 /*
  currentReport = millis(); 
  if ((currentReport - lastReport) >= reportInterval){
    Serial.print("currentReport : ");Serial.println(currentReport);
    Serial.print("lastReport : ");Serial.println(lastReport);
    lastReport = currentReport;
    readandwrite();
  }
  inputState = digitalRead(USER_INPUT);
  if (inputState == LOW) ADE9153_Calibration();
*/
}
#endif  
