while(1){
    // seleção dos parametros de entrada
    while(!press_button_level{
        // seleção dos Ks
        Ks = read(encoders)
        // seleção do andar
        inputDesejado = read(buttons_level)
    }

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


bool medida_estavel(input){

    int max, min;
    static int count = 0;
    static float data_vec[100];
    int eps = 1; //mm

    // preenche vetor inicial
    if (count < 100){
        data_vec(count) = input;
        count++;
        return false;
    }

    // shift left
    for (int i = 0; i < 100 - 1; i++){
        data_vec(i) = data_vec(i+1);
    }
    data_vec(99) = input;

    // checa max e min e compara com uma oscilação aceitável
    if (max(data_vec) - min(data_vec) > eps){
        return false;
    } else {
        return true;
    }

}

bool medida_estavel(){

}

bool check_Min_Max; //True: Busca um ponto de Máximo | False: Busca um ponto de Mínimo.
float point_Max = -INDF,point_Min = INDF;
int lastData = andarAtual;

if(refSignal > atualSignal) check_Min_Max = true;
else check_Min_Max = false;

while(....){
    ....
    inputSensor = read(sensor);

    point_Max = max(inputSensor,point_Max);
    point_Min = min(inputSensor,point_Min);

    if((inputSensor <= refSignal)&&(lastData >= refSignal)){//Passou pela Referência. Descendo até o ponto de Mínimo
        if(point_Max - point_Min < eps) return true;
        point_Min = INDF;
    }

    if((inputSensor >= refSignal)&&(lastData <= refSignal)){ //Passou pela Referência. Subindo até o ponto de Máximo
        if(point_Max - point_Min < eps) return true;
        point_Max = -INDF;
    }

    lastData = inputSensor;
}


//bool search_Min_Max;
if(inputSensor - refSignal < eps) check_Min_Max = !check_Min_Max; 

//bool passed_ref = false;












