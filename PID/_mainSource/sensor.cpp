#include "Header_Functions.hpp"

//Exponential Moving Average (EMA)
float filter(float input){
  float alpha = 0.5f;
  static float S;
  static bool first = true;

  if (first){
    S = input;
    first = false;
  }else{
    S = alpha * input + (1 - alpha) * S;
  }
  return S;
}

void initializeSensor(VL53L0X sensor){
  Wire.begin(SDA, SCL, 400000); // taxa de 400 kHz

  pinMode(GPIO_SENSOR, INPUT);

  delay(1000);

  sensor.init();
  sensor.setTimeout(500);

  #if defined LONG_RANGE
  // Diminui o limite da taxa do sinal de retorno (o padrão é 0,25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // Aumenta os períodos de pulso do laser (os padrões são 14 e 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif

  #if defined HIGH_SPEED
  //A taxa é padrão 33 ms
  sensor.setMeasurementTimingBudget(1000);  // temporização: 1ms
  #elif defined HIGH_ACCURACY
  sensor.setMeasurementTimingBudget(200000);// temporização: 200 ms
  #endif

  sensor.startContinuous();
}
