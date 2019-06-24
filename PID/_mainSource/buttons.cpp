#include "Header_Functions.hpp"
//Botões dos Andares
int BTN[] = {32,33,25,26,27};

void initializeButtons(){
   pinMode(BTN[0],INPUT);
   pinMode(BTN[1],INPUT);
   pinMode(BTN[2],INPUT);
   pinMode(BTN[3],INPUT);
   pinMode(BTN[4],INPUT);
}
/*
int readButtons(){
  for(int i = 0;i < 5;i++)
    if(digitalRead(BTN[i])) return (i+1);
  return 0;
}*/
