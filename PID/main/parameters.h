#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<bits/stdc++.h>
#define MAX_SAMPLES 1000000
#define INDF 1000000

using namespace std;
/*
//Parâmetros de Desempenho
typedef struct{
    float tUpDown; //Tempo de Subida -> em ms
    float tPeak; //Tempo de Pico -> em ms
    float tAccommodation; //Tempo de Acomodação -> em ms
    float overshoot; //Overshoot -> em mm
    float erRegime; //Erro Regime -> em mm
}PERFORMANCE_PARAMETERS;
*/

//Variáveis de configuração do sistema
typedef struct{
    //int refSignal;// Andar desejado  -> em mm
    //int origin;// Origem -> em mm
    //int dataSensor[MAX_SAMPLES];// Dados do sensor -> em mm
    float refSignal;
    float origin;
    float dataSensor[MAX_SAMPLES];
    int rateSamples;// Taxa de Amostragem -> em ms
    int totSamples;// Total de amostras
    string systemStatus;// Estabilizado,Estabilizando,Sem Estabilização.
}SYSTEM_CONFIGURATION_VARIABLES;

//Variáveis auxiliares para verificação de estabilidade
typedef struct{
    float epsilon;// Tolerância aceitável
    float point_Max;// Último ponto de máximo
    float point_Min;// Último ponto de mínimo
}STABILITY_VARIABLES;

float f(float );
bool checkStability(float ,float ,float );
PERFORMANCE_PARAMETERS extractPerformanceParameter(SYSTEM_CONFIGURATION_VARIABLES );

STABILITY_VARIABLES stabVar;
float vx[MAX_SAMPLES];
float referenceSig;

int main(){
    PERFORMANCE_PARAMETERS perfParm;
        SYSTEM_CONFIGURATION_VARIABLES configVar;

    float h = 0.1f;//taxa de amostragem
    int index = 0;

    cout << "Sinal de referencia: ";
    cin >> referenceSig;

    //Variáveis auxiliares para verificação de estabilidade
    stabVar.epsilon = 0.1;
    stabVar.point_Max = -INDF;
    stabVar.point_Min = INDF;

    configVar.dataSensor[0] = 0;//Sentinela
    float x = 3.0;

    //Variáveis de Configuração do sistema
    configVar.refSignal = referenceSig;//Andar, termo independente de f(x).
    configVar.rateSamples = 1;//1ms
    configVar.origin = f(x);

    do{
        cout << "Index: " <<  index << " | X: " << x;
        configVar.dataSensor[++index] = f(x);
        vx[index] = x;
        x += h;
    }while(!checkStability(configVar.dataSensor[index-1],configVar.dataSensor[index],configVar.refSignal));

    configVar.totSamples = index;
    perfParm = extractPerformanceParameter(configVar);
/*
    cout << "---------------------------RESULTADO---------------------------" << endl;
    cout << "Tempo de Pico: "<< perfParm.tPeak << " | X: " << vx[(int)perfParm.tPeak] << endl;
    cout << "Tempo de Subida: "<< perfParm.tUpDown << " | X: " << vx[(int)perfParm.tUpDown] << endl;
    cout << "Tempo de Acomodacao: " << perfParm.tAccommodation << " | X: " << vx[(int)perfParm.tAccommodation] << endl;
    cout << "Erro Regime: " <<  perfParm.erRegime << endl;
    cout << "Overshoot: " << perfParm.overshoot << endl;
*/
    return 0;
}

//Função de teste.
float f(float x){
    return (-(sin(x)*100.0f/pow(x,2))+referenceSig);
}

PERFORMANCE_PARAMETERS extractPerformanceParameter(SYSTEM_CONFIGURATION_VARIABLES confVar){
    PERFORMANCE_PARAMETERS dataPerform;
    dataPerform.overshoot = confVar.dataSensor[0];;
    int indexMax;
    int indexApprox;

    dataPerform.overshoot = -INDF;
    //Overshoot & Tempo de pico
    for(int i = 0; i < confVar.totSamples; i++){
        if (dataPerform.overshoot < confVar.dataSensor[i]) {
            dataPerform.overshoot = confVar.dataSensor[i];
            indexMax = i;
        }
    }
    dataPerform.tPeak = indexMax * confVar.rateSamples;

    //Tempo de Subida
    if(confVar.origin < confVar.refSignal){
        int percent_90 = round(0.9*confVar.refSignal);
        int distance_approx = INDF;//Para guardar a distância que mais se aproxima de 90% da referência.
        for(int i = 0;i < confVar.totSamples;i++){
            if(abs(percent_90 - confVar.dataSensor[i]) < distance_approx){
                distance_approx = abs(percent_90 - confVar.dataSensor[i]);
                indexApprox = i;
            }
        }
    }
    else{//Tempo de Descida
        int percent_90 = round(1.1*confVar.refSignal);
        int distance_approx = INDF;//Para guardar a distância que mais se aproxima de 90% da referência.
        for(int i = 0;i < confVar.totSamples;i++){
            if(abs(percent_90 - confVar.dataSensor[i]) < distance_approx){
                distance_approx = abs(percent_90 - confVar.dataSensor[i]);
                indexApprox = i;
            }
        }
    }
    dataPerform.tUpDown = indexApprox * confVar.rateSamples;

    //Tempo de Acomodação
    dataPerform.tAccommodation= confVar.totSamples * confVar.rateSamples;

    //Erro Regime
    dataPerform.erRegime = confVar.dataSensor[confVar.totSamples] - confVar.refSignal;

  return dataPerform;
}

bool checkStability(float lastData,float inputSensor,float refSignal){
    stabVar.point_Max = max(inputSensor,stabVar.point_Max);
    stabVar.point_Min = min(inputSensor,stabVar.point_Min);

    cout << " | Anterior: " << lastData << " | Atual: " << inputSensor << " \t MAX: " << stabVar.point_Max << " | MIN: " << stabVar.point_Min << endl;

    if(inputSensor == refSignal){
        if(stabVar.point_Max - stabVar.point_Min < stabVar.epsilon) return true;

        if(lastData > refSignal){//Passou pela Referência. Descendo até o ponto de Mínimo
/*          cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Descendo ate o ponto de Minimo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            */
            stabVar.point_Min = INDF;
        }
        else{
            if(lastData < refSignal){//Passou pela Referência. Descendo até o ponto de Mínimo
            /*
                cout << "---------------------------------------------------------------------------------------------------\n";
                cout << "Passou pela Referencia. Subindo ate o ponto de Maximo" << endl;
                cout << "---------------------------------------------------------------------------------------------------\n";
            */    
                stabVar.point_Max = -INDF;
            }
            else{
                //Sugestão: tirar média dos 10 últimos valores recebidos
                stabVar.point_Min = INDF;
                stabVar.point_Max = -INDF;
            }
        }
    }
    else{
        if((inputSensor < refSignal)&&(lastData > refSignal)){//Passou pela Referência. Descendo até o ponto de Mínimo
            /*
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Descendo ate o ponto de Minimo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            */
            stabVar.point_Min = INDF;
        }
        if((inputSensor > refSignal)&&(lastData < refSignal)){ //Passou pela Referência. Subindo até o ponto de Máximo
            /*
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Subindo ate o ponto de Maximo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            */
            if(stabVar.point_Max - stabVar.point_Min < stabVar.epsilon) return true;
            stabVar.point_Max = -INDF;
        }
    }
    return false;
}


#endif