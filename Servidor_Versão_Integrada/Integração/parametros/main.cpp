#include <iostream>
#include "parametros.hpp"

using namespace std;

int main()
{
    Parametros paramCtrl;
    int data[10];

// Variável para setar tamanho do vetor
    int len = sizeof(data)/sizeof(int);

//Vetor com dados de distância para teste
    for(int i = 0; i < 10; i++)
        data[i] = i * 90;

    //Overshoot
    paramCtrl.overshoot = extrai_overshoot(data, len);

    cout <<  "Overshoot: " << paramCtrl.overshoot << " mm"<< endl;

    //Tempo de Subida
    paramCtrl.tSubida = extrai_t_subida(data, len);

    cout <<  "Tempo de Subida: " << paramCtrl.tSubida << " segundos" << endl;

    //Tempo de Descida
    paramCtrl.tDescida = extrai_t_descida(data, len);

    cout <<  "Tempo de Descida: " << paramCtrl.tDescida << " segundos" << endl;

    //Tempo de acomodação
    paramCtrl.tAcomod = extrai_t_acomodacao(data, len);

    cout <<  "Tempo de acomodacao: " << paramCtrl.tAcomod << " segundos" << endl;


    return 0;
}