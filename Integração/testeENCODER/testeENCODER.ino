#define CLK1 4
#define DT1 16
#define SW1 17
//Pinos Potenciômetro Kd
#define CLK2 5
#define DT2 18
#define SW2 19
//Pinos Potenciômetro Ki
#define CLK3 0
#define DT3 34
#define SW3 15

typedef struct{
  unsigned int ki,kp,kd;
}CONTROL_VARIABLES;

typedef struct{
  int stateCLK1, stateCLK2, stateCLK3;
  int stateDT1, stateDT2, stateDT3;
  int Laststate1, Laststate2, Laststate3;
  unsigned int kp = 0,kd = 0,ki = 0;
  unsigned int kpmult = 0, kdmult = 0, kimult = 0;
}ENCODER_VARIABLES;

unsigned int ampK(unsigned int , unsigned int , int , int );
unsigned int multMod(unsigned int );
void initializePotentiometers(ENCODER_VARIABLES *);
void getControlVariables(ENCODER_VARIABLES *,CONTROL_VARIABLES *);

CONTROL_VARIABLES ctrlVariables;
ENCODER_VARIABLES encoderVariables;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initializePotentiometers(&encoderVariables);
}

void loop() {
  // put your main code here, to run repeatedly:
  getControlVariables(&encoderVariables,&ctrlVariables);
}


unsigned int ampK(unsigned int counter, unsigned int set, int DTstate, int CLKstate){
   if(DTstate != CLKstate)
   {
     switch(set){
       case 0:
         counter++;
         return counter;
       break;
       case 1:
         counter = counter + 10;
         return counter;
       break;
       case 2:
         counter = counter + 100;
         return counter;
       break;
       case 3:
         counter = counter + 1000;
         return counter;
       break;
       default:
         counter = 0;
         return counter;

       }
     }
     else
     {
       switch(set){
       case 0:
         counter--;
         return counter;
       break;
       case 1:
         counter = counter - 10;
         return counter;
       break;
       case 2:
         counter = counter - 100;
         return counter;
       break;
       case 3:
         counter = counter - 1000;
         return counter;
       break;
       default:
         counter = 0;
         return counter;

       }
   }
}

unsigned int multMod(unsigned int set){
   if(set == 3){
    set = 0;
    return set;
   }
   else set++;
   return set;
}

void getControlVariables(ENCODER_VARIABLES *encoderVar,CONTROL_VARIABLES *ctrlVar){

  encoderVar->stateCLK1 = digitalRead(CLK1);
  encoderVar->stateCLK2 = digitalRead(CLK2);
  encoderVar->stateCLK3 = digitalRead(CLK3);
  encoderVar->stateDT1 = digitalRead(DT1);
  encoderVar->stateDT2 = digitalRead(DT2);
  encoderVar->stateDT3 = digitalRead(DT3);

  if(encoderVar->stateCLK1 != encoderVar->Laststate1){
    ctrlVar->kp = ampK(ctrlVar->kp, encoderVar->kpmult, encoderVar->stateDT1, encoderVar->stateCLK1);
    Serial.print("Kp:");
    Serial.println(ctrlVar->kp);
  }
  if(encoderVar->stateCLK2 != encoderVar->Laststate2){
    ctrlVar->kd = ampK(ctrlVar->kd, encoderVar->kdmult, encoderVar->stateDT2, encoderVar->stateCLK2);
    Serial.print("Kd:");
    Serial.println(ctrlVar->kd);
  }
  if(encoderVar->stateCLK3 != encoderVar->Laststate3){
    ctrlVar->ki = ampK(ctrlVar->ki, encoderVar->kpmult, encoderVar->stateDT3, encoderVar->stateCLK3);
    Serial.print("Ki:");
    Serial.println(ctrlVar->ki);
  }

  if(digitalRead(SW1) == LOW){
    delay(100);
    if(digitalRead(SW1) == HIGH){
      encoderVar->kpmult = multMod(encoderVar->kpmult);
      Serial.println("Kpmult:");
      Serial.println(encoderVar->kpmult);
    }
  }
  if(digitalRead(SW2) == LOW){
    delay(100);
    if(digitalRead(SW2) == HIGH){
      encoderVar->kdmult = multMod(encoderVar->kdmult);
      Serial.println("Kdmult:");
      Serial.println(encoderVar->kdmult);
    }
  }
  if(digitalRead(SW3) == LOW){
    delay(100);
    if(digitalRead(SW3) == HIGH){
      encoderVar->kimult = multMod(encoderVar->kimult);
      Serial.println("Kimult:");
      Serial.println(encoderVar->kimult);
    }
  }
  encoderVar->Laststate1 = encoderVar->stateCLK1;
  encoderVar->Laststate2 = encoderVar->stateCLK2;
  encoderVar->Laststate3 = encoderVar->stateCLK3;
}

void initializePotentiometers(ENCODER_VARIABLES *encoderVar){
  pinMode(CLK1,INPUT);
  pinMode(DT1,INPUT);
  pinMode(SW1,INPUT_PULLUP);
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(SW2,INPUT_PULLUP);
  pinMode(CLK3,INPUT);
  pinMode(DT3,INPUT);
  pinMode(SW3,INPUT_PULLUP);

  encoderVar->Laststate1 = digitalRead(CLK1);
  encoderVar->Laststate2 = digitalRead(CLK2);
  encoderVar->Laststate3 = digitalRead(CLK3);
}

