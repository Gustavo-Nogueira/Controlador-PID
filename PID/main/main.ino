#include "servidor.h"
//#include "parameters.h"
//#include "MultEncoder.h"
//#include "sensorVL53XL0V2.h"

void setup(){

  servidorSetup();
  
}

void loop(){

  servidorLoop();

  /*
// seleção dos parametros de entrada
  while(!press_button_level{
      // seleção dos Ks
      Ks = read(encoders)
      // seleção do andar
      inputDesejado = read(buttons_level)

       // refresh das variáveis
  dataVector = []; // limpa vetor de dados
  paramsCtrl = paramsCtrlClear; // limpa parâmetros de controle
  status = refresh; // limpa dados e limpa gráfico no servidor
  server.handleClient();

  while(status != estabilizado && status != nao_estabilizavel){
        // controlador funcionando
        inputSensor = read(sensor);
        potenciaPWM = controlador_pid(inputSensor,inputDesejado);
        setPWM(potenciaPWM);
        dataVector = guardaVetor(inputSensor)
        if(!medida_estavel(inputSensor = read(sensor))){ // parte transiente
            status = estabilizando; // servidor atualiza os dados lidos
            ligaLEDAndar(0);
        }
        else{ // parte em regime
            status = estabilizado // servidor atualiza parametros de controle
            paramsCtrl.overshoot = extrai_overshoot(dataVector);
            paramsCtrl.tSubida = extrai_t_subida(dataVector);
            paramsCtrl.tAcomod = extrai_t_acomodacao(dataVector);
            paramsCtrl.erroRegime = extrai_erroRegime(dataVector(LENGTH), inputDesejado);

            ligaLEDAndar(1)
        }
        if(t > TMAX){
            status = nao_estabilizavel // para de plotar e manda alerta
            ligaLEDAndar(0)
        }
        t++;
        server.handleClient();
    }

 
  
}
*/
  }
