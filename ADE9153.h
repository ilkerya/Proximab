
#ifdef ENERGYMETER_EXISTS
 //#define ADE9153A_ACCMODE 0x0000      /*Energy accumulation modes, Bit 4, 0 for 50Hz, 1 for 60Hz 0x0010 -> 0x0000 */
  #define SPI_SPEED 1000000     //SPI Speed   ////* Basic initializations for energy meter IC ADE9153 */
  ADE9153AClass ade9153A;
void EnergymeterCalbLed(void){
  switch(EnergyMeterIC.Mode){
    case POWERIC_SETUP1:
    break;
    case POWERIC_SETUP2:
     break;
    case POWERIC_SETUP3:
    break;
    case POWERIC_NORMAL:           // Running Normal Mode  
    break;
    case POWERIC_CALB1: // Calibrating The Current Channel Started            
    case POWERIC_CALB2: // Calibrating The Current Channel Continues        
    case POWERIC_CALB3: // Calibrating The Current Channel Ends
    case POWERIC_CALB4: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB5:    
    case POWERIC_CALB6: // Calibrating The Current Channel Ends
    case POWERIC_CALB7: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB8:  
    case POWERIC_CALB9: // Calibrating The Voltage Channel Starts              
    case POWERIC_CALB10: 
      digitalWrite(ADE9153A_RED_LED, !digitalRead(ADE9153A_RED_LED));
    break;
     default:   
    break;
  }
}
// Call in 2 seconds
void EnergyMeterIC_Operation(void){ 
            /*
                Serial.print("EnergyMeterIC.Mode : "); 
                Serial.println(EnergyMeterIC.Mode); 
                Serial.print("EnergyMeterIC.Timer : "); 
                Serial.println(EnergyMeterIC.Timer);  
                */   
  switch(EnergyMeterIC.Mode){   
    case POWERIC_SETUP1:
      pinMode(ADE9153A_RED_LED, OUTPUT);
      pinMode(ADE9153A_CALB_BUTTON, INPUT);
      pinMode(ADE9153A_RESET_PIN, OUTPUT);
      digitalWrite(ADE9153A_RESET_PIN, HIGH);  
      digitalWrite(ADE9153A_RESET_PIN, LOW);
      delay(10);
      Serial.println("Reset Done");
      digitalWrite(ADE9153A_RESET_PIN, HIGH);;
      EnergyMeterIC.Mode = POWERIC_SETUP2;//
    break;
    case POWERIC_SETUP2:
          if(EnergyMeterIC.Timer)EnergyMeterIC.Timer--;
          else EnergyMeterIC.Mode = POWERIC_SETUP3;//PowerIC_Mode++;    
    break;
    case POWERIC_SETUP3:
         EnergyMeterIC.commscheck = ade9153A.SPI_Init(SPI_SPEED,ADE9153A_CS_PIN); //Initialize SPI
      //   commscheck = OFF; // TEST ERROR CASE
        if (!EnergyMeterIC.commscheck) {
           Serial.println("ADE9153A Shield not detected. Plug in Shield and reset the Arduino");  
            EnergyMeterIC.Error = ON;
            EnergyMeterIC.Mode = POWERIC_SETUP1; 
        }
        else {
          ade9153A.SetupADE9153A(); //Setup ADE9153A according to ADE9153AAPI.h
          /* Read and Print Specific Register using ADE9153A SPI Library */
          Serial.println(String(ade9153A.SPI_Read_32(REG_VERSION_PRODUCT), HEX)); // Version of IC
          ade9153A.SPI_Write_32(REG_AIGAIN, -268435456); //AIGAIN to -1 to account for IAP-IAN swap
          EnergyMeterIC.Error = OFF; 
          EnergyMeterIC.Mode = POWERIC_NORMAL ; 
        }
    break;
    case POWERIC_NORMAL:           // Running Normal Mode
      ade9153A.ReadPowerRegs(&powerVals);    //Template to read Power registers from ADE9000 and store data in Arduino MCU
      ade9153A.ReadRMSRegs(&rmsVals);
      ade9153A.ReadPQRegs(&pqVals);
      ade9153A.ReadTemperature(&tempVal);

      Values.Current = rmsVals.CurrentRMSValue/1000;
      if(Values.Current < 0) Values.Current = 0;

      Values.Voltage = rmsVals.VoltageRMSValue/1000;
      if(Values.Voltage < 0) Values.Voltage = 0 ;

      Values.ActivePower = powerVals.ActivePowerValue/1000;
      if(Values.ActivePower < 0) Values.ActivePower = 0; 

      Values.PowerFactor = pqVals.PowerFactorValue;
      if(Values.PowerFactor < 0) Values.PowerFactor = 0;

      Values.Frequency =  pqVals.FrequencyValue;
      if(Values.Frequency < 0) Values.Frequency = 0;
      if(Values.Voltage < 30) Values.Frequency = 0;

      Serial.print("RMS Current:\t");Serial.print(Values.Current); Serial.println(" A         ");
      //Serial.print(rmsVals.CurrentRMSValue/1000); Serial.println(" A");

      Serial.print("RMS Voltage:\t");       Serial.print(Values.Voltage);Serial.println(" V      ");
      //Serial.print(rmsVals.VoltageRMSValue/1000); Serial.println(" V");
  
      Serial.print("Active Power:\t");        
      Serial.print(Values.ActivePower);
      Serial.println(" W");
  
      Serial.print("Reactive Power:\t");        
      Serial.print(powerVals.FundReactivePowerValue/1000);
      Serial.println(" VAR");
  
      Serial.print("Apparent Power:\t");        
      Serial.print(powerVals.ApparentPowerValue/1000);
      Serial.println(" VA");
   
      Serial.print("Power Factor:\t");        
      Serial.println(Values.PowerFactor);

      Serial.print("Frequency:\t");        
      Serial.print(pqVals.FrequencyValue);
      Serial.println(" Hz");
  
      Serial.print("Temperature:\t");        
      Serial.print(tempVal.TemperatureVal);
      Serial.println(" degC");
            
      if (digitalRead(ADE9153A_CALB_BUTTON) == LOW){
         if(EnergyMeterIC.Mode == POWERIC_NORMAL) 
            EnergyMeterIC.Mode=POWERIC_CALB1;// Set to calibration   
      }
       
    break;
    case POWERIC_CALB1: // Calibrating The Current Channel Started           
        ade9153A.StartAcal_AINormal();
        EnergyMeterIC.Timer = 10; //20sec
        Serial.println("Autocalibrating Current Channel Started");
        EnergyMeterIC.Mode++;         
    break;
    case POWERIC_CALB2: // Calibrating The Current Channel Continues
          if(EnergyMeterIC.Timer)EnergyMeterIC.Timer--;
          else EnergyMeterIC.Mode++;
    break;  
    case POWERIC_CALB3: // Calibrating The Current Channel Ends
        ade9153A.StopAcal();      
        Serial.println("Autocalibrating Current Channel Ended"); 
        EnergyMeterIC.Mode++;
    break;
    case POWERIC_CALB4: // Calibrating The Voltage Channel Starts
        ade9153A.StartAcal_AV();
        EnergyMeterIC.Timer = 20; // 40 sec
        Serial.println("Autocalibrating Voltage Channel Started");   
        EnergyMeterIC.Mode++;               
    break;
    case POWERIC_CALB5: // Calibrating The Voltage Channel continues
          if(EnergyMeterIC.Timer)EnergyMeterIC.Timer--;
          else EnergyMeterIC.Mode++; 
      
    break;    
    case POWERIC_CALB6: // Calibrating The Voltage Channel Ends
         ade9153A.StopAcal();
         EnergyMeterIC.Timer = 1;
         Serial.println("Autocalibrating Voltage Channel Ended"); 
         EnergyMeterIC.Mode++;   
    break;
    case POWERIC_CALB7: // wait to Read the Calibration Registers
          if(EnergyMeterIC.Timer)EnergyMeterIC.Timer--;
          else EnergyMeterIC.Mode++; 
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
    //  EnergyMeterIC.Timer = 1;
      EnergyMeterIC.Mode++;
    break; 
    case POWERIC_CALB9:
          if(EnergyMeterIC.Timer)EnergyMeterIC.Timer--;
          else EnergyMeterIC.Mode++; 
    break; 
    case POWERIC_CALB10:
      EnergyMeterIC.Mode = POWERIC_NORMAL;
    break;   
    default:
      EnergyMeterIC.Mode = POWERIC_SETUP1;    
    break;
  }  
}

#endif  
