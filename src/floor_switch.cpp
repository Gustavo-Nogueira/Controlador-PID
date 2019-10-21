#include "Header_Functions.hpp"
//Botões dos Andares
int BTN[] = {27,26,25,33,32};
float dist_floor[] = {35.0, 160.0, 286.0, 412.0, 535.0};//Distâncias em mm
 
void initializeButtons(){
   pinMode(BTN[0],INPUT);
   pinMode(BTN[1],INPUT);
   pinMode(BTN[2],INPUT);
   pinMode(BTN[3],INPUT);
   pinMode(BTN[4],INPUT);
}

int readButtons(){
  for(int i = 0;i < 5;i++){
    if(digitalRead(BTN[i]) == LOW){
      delay(50);
      if(digitalRead(BTN[i]) == HIGH) return (i+1);
    }
  }
  return 0;
}
float distanceReference(int btn){
  return (dist_floor[btn-1]);
}

