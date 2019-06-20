#ifndef MULTENCODER_H
#define MULTENCODER_H


#define CLK1 5
#define DT1 6
#define SW1 7
#define CLK2 2
#define DT2 3
#define SW2 4
/*#define CLK3 4
#define DT3 5
#define SW3 6*/



int stateCLK1, stateCLK2/*, stateCLK3*/;
int stateDT1, stateDT2/*, stateDT3*/;
int Laststate1, Laststate2/*, Laststate3*/; 
unsigned int ampK(unsigned int counter, unsigned int set, int DTstate, int CLKstate);
unsigned int multMod(unsigned int set);
  unsigned int kp = 0,kd = 0/*,ki = 0*/;
  unsigned int kpmult = 0, kdmult = 0/*, kimult = 0*/;  
void setup() {

  pinMode(CLK1,INPUT);
  pinMode(DT1,INPUT);
  pinMode(SW1,INPUT_PULLUP);
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(SW2,INPUT_PULLUP);
  /*pinMode(CLK3,INPUT);
  pinMode(DT3,INPUT);
  pinMode(SW3,INPUT_PULLUP);*/
  Serial.begin(9600);

  Laststate1 = digitalRead(CLK1);
  Laststate2 = digitalRead(CLK2);
  //Laststate3 = digitalRead(CLK3);
}

void loop() {
  
  stateCLK1 = digitalRead(CLK1);
  stateCLK2 = digitalRead(CLK2);
  //stateCLK3 = digitalRead(CLK3);
  stateDT1 = digitalRead(DT1);
  stateDT2 = digitalRead(DT2);
  //stateDT3 = digitalRead(DT3);
  if(stateCLK1 != Laststate1)
  {
    kp = ampK(kp, kpmult, stateDT1, stateCLK1);
         Serial.println("Kp:"); 
         Serial.println(kp);
  }
  if(stateCLK2 != Laststate2)
  {
    kd = ampK(kd, kdmult, stateDT2, stateCLK2);
         Serial.println("Kd:"); 
         Serial.println(kd);
  }
  
  /*if(stateCLK3 != Laststate3)
  {
    ki = ampK(kp, kpmult, stateDT3, stateCLK3);
         Serial.println("Ki:"); 
         Serial.println(ki);
  }*/
  
  if(digitalRead(SW1) == LOW){
    delay(100);
    if(digitalRead(SW1) == HIGH){
      kpmult = multMod(kpmult);
      Serial.println("Kpmult:");
      Serial.println(kpmult);}}
  if(digitalRead(SW2) == LOW){
    delay(100);
    if(digitalRead(SW2) == HIGH){
     kdmult = multMod(kdmult);
      Serial.println("Kdmult:");
      Serial.println(kdmult);}}
  /*if(digitalRead(SW3) == LOW){
    delay(100);
    if(digitalRead(SW3) == HIGH){
       kimult = multMod(kimult);
      Serial.println("Kimult:");
      Serial.println(kimult);}}*/
    Laststate1= stateCLK1;
    Laststate2= stateCLK2;
    //Laststate3= stateCLK3;
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
   }}

unsigned int multMod(unsigned int set){
   if(set == 3){
    set = 0; 
    return set;
   }
   else set++; 
   return set;
   }

#endif