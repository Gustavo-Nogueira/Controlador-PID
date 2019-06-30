
//Botões dos Andares
int BTN[] = {27,26,25,33,32};
float dist_floor[] = {677.0f,551.0f,426.0f,301.0f,175.0f};//Distâncias em mm
 
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
    if(digitalRead(BTN[i]) == HIGH){
    return (i+1);
   }
  }
}
  return 0;
}

float distanceReference(int btn){
  return (dist_floor[btn-1]);
}


void setup(){
  Serial.begin(115200);
  initializeButtons();
  
}

int btn;
void loop(){
  do{
    btn = readButtons();
    Serial.print("BOTAO: ");
    Serial.println(btn);
  }while(!btn);
  Serial.println("CLICOUUUUUUUUUUUUUUUUUUU");
  delay(2000);
}

