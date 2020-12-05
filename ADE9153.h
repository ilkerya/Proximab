
#ifdef AD9153_PROTOTYPE

// Call in 2 seconds
void PowerIC_Operation(void){ 
                Serial.print("PowerIC_Mode : "); 
                Serial.println(PowerIC_Mode); 
                Serial.print("PowerIC.Timer : "); 
                Serial.println(PowerIC.Timer);     
  switch(PowerIC_Mode){   
    case POWERIC_SETUP1:
      pinMode(LED, OUTPUT);
      pinMode(USER_INPUT, INPUT);
      pinMode(ADE9153A_RESET_PIN, OUTPUT);
      digitalWrite(ADE9153A_RESET_PIN, HIGH);  
      digitalWrite(ADE9153A_RESET_PIN, LOW);
      delay(10);
      Serial.println("Reset Done");
      digitalWrite(ADE9153A_RESET_PIN, HIGH);;
      PowerIC_Mode = POWERIC_SETUP2;//
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
          PowerIC_Mode = POWERIC_NORMAL ; 
        }
    break;
    case 3://POWERIC_NORMAL:           // Running Normal Mode
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
            
      if (digitalRead(USER_INPUT) == LOW)PowerIC_Mode=POWERIC_CALB1;// Set to calibration   
       
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

#endif  
