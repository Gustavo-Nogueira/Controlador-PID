int h_ref, h_sensor;
int Kp,Ki,Kd;
int A,B,C;
int Ts;
int first_error, second_error = 0, third_error = 0;
int output, last_output = 0;

void loop(){

  first_error = h_ref - h_sensor;

  // Interacao Kp
  A = Kp*(first_error - second_error);
  
  // Interecao Ki
  B = 0.5*Ki*Ts*(first_error + second_error);
  
  // Interacao Kd
  C = (Kd/Ts)*(first_error - 2*second_error  + third_error);
  
  //Saida controle

output = last_output + A + B + C;
  
third_error = second_error;
second_error = first_error;
   
last_output = output;
  
  }
