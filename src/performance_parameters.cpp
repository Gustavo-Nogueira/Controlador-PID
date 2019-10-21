#include "Header_Functions.hpp"

PERFORMANCE_PARAMETERS extractPerformanceParameter(SYSTEM_CONFIGURATION_VARIABLES confVar,float *dataSensor){
    PERFORMANCE_PARAMETERS perfParameters;

    float maxData = -INF;
    int indexMax;
    //Overshoot & Tempo de pico
    for(int i = 0; i < confVar.totSamples; i++){
        if (maxData < dataSensor[i]) {
            maxData = dataSensor[i];
            indexMax = i;
        }
    }
    //Tempo de Pico
    perfParameters.tPeak = indexMax * confVar.rateSamples;
    //Overshoot
    perfParameters.overshoot = (maxData/dataSensor[confVar.totSamples]) * 100;
    //Erro de Regime 
    perfParameters.erRegime = dataSensor[confVar.totSamples] - confVar.refSignal;   
    //Tempo de acomadação
    perfParameters.tAccommodation = confVar.totSamples * confVar.rateSamples;
    //Tempo de Subida
    float value = dataSensor[confVar.totSamples] * 0.9;
    for(int i = 0; i < confVar.totSamples; i++){
      if (value >= dataSensor[i] ){
        perfParameters.tUp = i * confVar.rateSamples;                                                                                                                                                                                                                     
        break;
      }
    }

    return perfParameters;
}
