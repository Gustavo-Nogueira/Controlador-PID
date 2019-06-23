#ifndef HEADER_FUNCTIONS_H_INCLUDED
#define HEADER_FUNCTIONS_H_INCLUDED

//////////////////////////////////////////////////////////Structs
#define MAX_SAMPLES 10000  

//Variáveis de Controle
typedef struct{
  unsigned int ki,kp,kd;
}CONTROL_VARIABLES;

//Parâmetros de Desempenho
typedef struct{
    float tUpDown; //Tempo de Subida -> em ms
    float tPeak; //Tempo de Pico -> em ms
    float tAccommodation; //Tempo de Acomodação -> em ms
    float overshoot; //Overshoot -> em mm
    float erRegime; //Erro Regime -> em mm
}PERFORMANCE_PARAMETERS;

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
}SYSTEM_CONFIGURATION_VARIABLES;

//////////////////////////////////////////////////////////Sensor
#include <Wire.h>
#include <VL53L0X.h>

#define SDA 21
#define SCL 22
#define GPIO_SENSOR 23

float filter(float );
void initializeSensor(VL53L0X );

//////////////////////////////////////////////////////////Encoder

//Pinos Potenciômetro Kp
#define CLK1 4
#define DT1 16
#define SW1 17
//Pinos Potenciômetro Kd
#define CLK2 5
#define DT2 18
#define SW2 19
//Pinos Potenciômetro Ki
#define CLK3 12
#define DT3 13
#define SW3 14

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


//////////////////////////////////////////////////////////Botões

void initializeButtons();
int readButtons();

//////////////////////////////////////////////////////////PID

//////////////////////////////////////////////////////////PWM

//////////////////////////////////////////////////////////Parâmetros de Desempenho

//////////////////////////////////////////////////////////Verificador de Estabilidade


#endif
