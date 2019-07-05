int h_ref, h_sensor;
int Kp,Ki,Kd;
int A,B,C;
int Ts;
int first_error, second_error = 0, third_error = 0;
int output, last_output = 0;

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
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

#define HIGH_SPEED
//#define HIGH_ACCURACY

// Pin definitions
//int myLed = 5;
int intPin = 23;

bool newData = false;

uint32_t delt_t = 0, count = 0, sumCount = 0;  // used to control display output rate
float deltat = 0.0f, sum = 0.0f;          // integration interval for both filter schemes
uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;                         // used to calculate integration interval


void setup()
{
  Serial.begin(115200);
  delay(4000);
  
 Wire.begin(21, 22, 400000); // SDA (21), SCL (22) on ESP32, 400 kHz rate
 
// Set up the led indicator
  //pinMode(myLed, OUTPUT);
  //digitalWrite(myLed, LOW);
  pinMode(intPin, INPUT);

  I2Cscan();
  
  delay(1000);
  
  sensor.init();
  sensor.setTimeout(500);

  #if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  #endif

  #if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(1000);  // minimum timing budget 50 ms
  #elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
  #endif

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();  

  //pinMode(2,INPUT);
  pinMode(2, OUTPUT);//Definimos o pino 2 (LED) como sa�da.
 
  ledcAttachPin(2, 0);//Atribuimos o pino 2 ao canal 0.
  ledcSetup(0, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
  
}
//filter(sensor.readRangeContinuousMillimeters());

int i = 1023;

void loop(){
  h_sensor=filter(sensor.readRangeContinuousMillimeters());
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
ledcWrite(0, i-output);//Escrevemos no canal 0, o duty cycle "i".   
last_output = output;

  }
void myinthandler()
{
  newData = true; // set the new data ready flag to true on interrupt
}

// I2C scan function
void I2Cscan()
{
// scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
    
}
