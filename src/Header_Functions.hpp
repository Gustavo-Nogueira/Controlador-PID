#ifndef HEADER_FUNCTIONS_H_INCLUDED
#define HEADER_FUNCTIONS_H_INCLUDED

//////////////////////////////////////////////////////////Structs
#define PWM_RANGE_MIN 205 
#define PWM_RANGE_MAX 614
#define PWM_MAX 1023
#define INF 1000000

//Parâmetros de Desempenho
typedef struct{
    float tUp; //Tempo de Subida -> em ms
    float tPeak; //Tempo de Pico -> em ms
    float tAccommodation; //Tempo de Acomodação -> em ms
    float overshoot; //Overshoot -> em mm
    float erRegime; //Erro Regime -> em mm
}PERFORMANCE_PARAMETERS;

//Variáveis de configuração do sistema
typedef struct{
    float refSignal;
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
//Pinos Potenciômetro Kd
#define CLK2 18
#define DT2 19
//Pinos Potenciômetro Ki
#define CLK3 17
#define DT3 5

//Variáveis de Controle
typedef struct{
    double ki,kp,kd;
}CONTROL_VARIABLES;

typedef struct{
  int stateCLK1, stateCLK2, stateCLK3;
  int stateDT1, stateDT2, stateDT3;
  int Laststate1, Laststate2, Laststate3;
}ENCODER_VARIABLES;

double ampK(double , int , int , double , double );
void initializePotentiometers(ENCODER_VARIABLES *);
void getControlVariables(ENCODER_VARIABLES *,CONTROL_VARIABLES *);

//////////////////////////////////////////////////////////Botões

void initializeButtons();
float distanceReference(int );
int readButtons();

//////////////////////////////////////////////////////////Parâmetros de Desempenho
PERFORMANCE_PARAMETERS extractPerformanceParameter(SYSTEM_CONFIGURATION_VARIABLES ,float *);

#endif
