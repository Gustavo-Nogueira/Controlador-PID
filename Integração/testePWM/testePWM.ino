#include<stdio.h>
#include<stdlib.h>
void setup()
{
   Serial.begin(9600);
  //pinMode(2,INPUT);
  pinMode(2, OUTPUT);//Definimos o pino 2 (LED) como saída.
 
  ledcAttachPin(2, 0);//Atribuimos o pino 2 ao canal 0.
  ledcSetup(0, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
  
}

//int i = 1023-256-32;
void loop()
{
  String str = "";
  if(Serial.available()> 0){
    /*str += chr; 
    int i = str.toInt();*/
    char chr1 = Serial.read();
    char chr2 = Serial.read();
    char chr3 = Serial.read();
    
    int i = 10*10*(chr1 - '0');
    i += 10*(chr2 - '0');
    i += (chr3 - '0');
    
    
  }
  Serial.println(i);
  ledcWrite(0, (1023 - i));//Escrevemos no canal 0, o duty cycle "i".
  
  
/*for ( i = 0; i < 1024; i++)
  {    ledcWrite(0, i);//Escrevemos no canal 0, o duty cycle "i".
    delay(200);
    Serial.println(i);
  }
 
  for ( i = 1023; i > 0; i--)
  {
    ledcWrite(0, i);
    delay(200);
    Serial.println(i);
  }*/
}
