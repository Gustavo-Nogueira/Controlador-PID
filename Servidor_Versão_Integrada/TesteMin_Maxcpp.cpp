#include<bits/stdc++.h>
#define INDF 1000000
using namespace std;

int main(){

    int eps = 2,andarAt = 0;;
    int data[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,4,5,6,7,8,9,10,11,12,13,14,15,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,3,4,5,6,7,8,9,10,11,12,13,12,11,10,9,8,7,6,5,4,5,6,7,8,9,10,11,12,11,10,9,8,7,6,7,8,9,10,11,10,9,8,7,8,9,10,11,10,9,8,9,10,11,10,9,10,10};
    bool estavel = false;
    bool checkMaxMin,checkLast;
    int point_Max = -INDF, point_Min = INDF;
    int lastData = 0,refSignal = 10,inputSensor,ctd = 0;
    FILE *fp = fopen("testefunct.txt","w");

    if(andarAt < refSignal){
        checkMaxMin = false;
        checkLast = false;
    }
    else{
        checkMaxMin = true;
        checkLast = true;
    }

    while(!estavel){
        inputSensor = data[ctd];
        fprintf(fp,"%d %d\n",ctd,data[ctd]);

        point_Max = max(inputSensor,point_Max);
        point_Min = min(inputSensor,point_Min);

        if(inputSensor >=  refSignal) checkMaxMin = true;
        else checkMaxMin = false;

        if(inputSensor == refSignal){
            if(point_Max - point_Min < eps) estavel = true;
            cout << "FOI IGUAL\n";
        }

        cout << "Anterior: " << lastData << " | Atual: " << data[ctd] << " \t MAX: " << point_Max << " | MIN: " << point_Min << endl;
        if((!checkMaxMin)&&(checkLast)){//Passou pela Referência. Descendo até o ponto de Mínimo
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Descendo ate o ponto de Minimo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            if(point_Max - point_Min < eps){
                    estavel = true;
            }
            point_Min = INDF;
        }


        if(((checkMaxMin)&&(!checkLast))){ //Passou pela Referência. Subindo até o ponto de Máximo
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Subindo ate o ponto de Maximo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            if(point_Max - point_Min < eps) {
                estavel = true;
            }
            point_Max = -INDF;
        }

        checkLast = checkMaxMin;
        ctd++;
    }

    fclose(fp);
    return 0;
}
/*

while(!estavel){
        inputSensor = data[ctd];
        fprintf(fp,"%d %d\n",ctd,data[ctd]);

        point_Max = max(inputSensor,point_Max);
        point_Min = min(inputSensor,point_Min);

        cout << "Anterior: " << lastData << " | Atual: " << data[ctd] << " \t MAX: " << point_Max << " | MIN: " << point_Min << endl;
        if(((inputSensor <= refSignal)&&(lastData >= refSignal))){//Passou pela Referência. Descendo até o ponto de Mínimo
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Descendo ate o ponto de Minimo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            if(point_Max - point_Min < eps){
                    estavel = true;
            }
            point_Min = INDF;
        }

        if(((inputSensor >= refSignal)&&(lastData <= refSignal))){ //Passou pela Referência. Subindo até o ponto de Máximo
            cout << "---------------------------------------------------------------------------------------------------\n";
            cout << "Passou pela Referencia. Subindo ate o ponto de Maximo" << endl;
            cout << "---------------------------------------------------------------------------------------------------\n";
            if(point_Max - point_Min < eps) {
                estavel = true;
            }
            point_Max = -INDF;
        }

        lastData = inputSensor;
        ctd++;
    }


*/
