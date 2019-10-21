#include "Header_Functions.hpp"

double ampK(double counter, int DTstate, int CLKstate, double maxv, double pass){
   if(DTstate != CLKstate){ counter += pass;
    if(counter > maxv ){
      counter = maxv;
    }
   return counter;}
     else { counter -= pass;
      if(counter<0){
        counter = 0;
      }
     return counter;}
}


void getControlVariables(ENCODER_VARIABLES *encoderVar,CONTROL_VARIABLES *ctrlVar){

  encoderVar->stateCLK1 = digitalRead(CLK1);
  encoderVar->stateCLK2 = digitalRead(CLK2);
  encoderVar->stateCLK3 = digitalRead(CLK3);
  encoderVar->stateDT1 = digitalRead(DT1);
  encoderVar->stateDT2 = digitalRead(DT2);
  encoderVar->stateDT3 = digitalRead(DT3);
  const double kpmax = 2;
  const double kppass = 0.025;
  const double kimax = 1;
  const double kipass = 0.025;
  const double kdmax = 1;
  const double kdpass = 0.0125;
  if(encoderVar->stateCLK1 != encoderVar->Laststate1){
    ctrlVar->kp = ampK(ctrlVar->kp, encoderVar->stateDT1, encoderVar->stateCLK1,kpmax,kppass);
    Serial.print("Kp:");
    Serial.println(ctrlVar->kp,5);
  }
  if(encoderVar->stateCLK2 != encoderVar->Laststate2){
    ctrlVar->kd = ampK(ctrlVar->kd, encoderVar->stateDT2, encoderVar->stateCLK2,kdmax,kdpass);
    Serial.print("Kd:");
    Serial.println(ctrlVar->kd,5);
  }
  if(encoderVar->stateCLK3 != encoderVar->Laststate3){
    ctrlVar->ki = ampK(ctrlVar->ki, encoderVar->stateDT3, encoderVar->stateCLK3,kimax,kipass);
    Serial.print("Ki:");
    Serial.println(ctrlVar->ki,5);
  }
  
  encoderVar->Laststate1 = encoderVar->stateCLK1;
  encoderVar->Laststate2 = encoderVar->stateCLK2;
  encoderVar->Laststate3 = encoderVar->stateCLK3;
}

void initializePotentiometers(ENCODER_VARIABLES *encoderVar){
  pinMode(CLK1,INPUT);
  pinMode(DT1,INPUT);
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(CLK3,INPUT);
  pinMode(DT3,INPUT);

  encoderVar->Laststate1 = digitalRead(CLK1);
  encoderVar->Laststate2 = digitalRead(CLK2);
  encoderVar->Laststate3 = digitalRead(CLK3);
}
