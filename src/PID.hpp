class PID{
public:
  
  double error;
  double sample;
  double lastSample;
  double kP, kI, kD;      
  double P, I, D;
  double pid;
  
  double setPoint;
  long lastProcess;
  
  PID(double _kP, double _kI, double _kD){
    kP = _kP;
    kI = _kI;
    kD = _kD;
  }
  
  void addNewSample(double _sample){
    sample = _sample;
  }
  
  void setSetPoint(double _setPoint){
    setPoint = _setPoint;
  }
 
  double getProportionalAction(){
    return this->P;
  }

  double getIntegralAction(){
    return this->I;
  }

  double getDerivativeAction(){
    return this->D;
  }

  void setKp(double kp){
    this->kP= kp;
  }
  
  void setKi(double ki){
    this->kI = ki;
  }
  
  void setKd(double kd){
    this->kD = kd;
  }
  
  
  double process(){
    // Implementação PID
    error = setPoint - sample;
    float deltaTime = (millis() - lastProcess) / 1000.0;
    lastProcess = millis();
    
    Serial.print("Erro: ");
    Serial.println(error);
    Serial.print("Delta Tempo: ");
    Serial.println(deltaTime);
    
    P = error * kP;
    I = I + (error * kI) * deltaTime;
    
    //Anti-Windup
    if (I > 1000) I = 1000;
    if (I < -1000) I = -1000;
    
    D = (lastSample - sample) * kD / deltaTime;
    lastSample = sample;
    
    pid = P + I + D + 480.8;    
    return pid;
  }
};
