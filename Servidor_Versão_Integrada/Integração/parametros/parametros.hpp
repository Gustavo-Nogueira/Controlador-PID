#ifndef PARAMETROS_HPP
#define PARAMETROS_HPP
#define tempoAmostragem 0.1

typedef struct Param{
    int overshoot;
    float  tSubida;
    float  tDescida;
    float  tAcomod;
    int erroRegime;
}Parametros;
using namespace std;


//Máximo valor obtido do vetor
extern int extrai_overshoot(int data[], int size)
{
    int max = 0;

    for(int i = 0; i < size; i++){
        if(data[i] > max) max = data[i]; 
    }

    return max;
}

extern float extrai_t_subida(int data[], int size)
{
    int pos = 0;
    float andar = 4;

    // Considerando que os valores do sensor virão na ordem de mm e que há quatro andares, no exemplo... 90% de 4000 mm (4º andar) é 3600
    data[5] = 3600;

    for(int i = 0; i < size; i++){
        
        if(data[i] == (andar * 0.9 * 1000))
            return pos * tempoAmostragem;
        pos++;
    }
    
}

extern float extrai_t_descida(int data[], int size)
{
    //int andar = funcaoSeletoraAndar();
    int andar = 1;
    int pos = 0;

    //Considerando que o tempo de acomodação seja 10% acima do andar desejado, no exemplo o tempo de acomodação para o 1º andar é respectivo a altura de 1100 mm
    data[5] = 1100;

    for(int i = 0; i < size; i++){

        if(data[i] == (andar * 1.1 * 1000))
            return pos * tempoAmostragem;
        
        pos++;
    }
}

extern float extrai_t_acomodacao(int data[], int size)
{
    return size * tempoAmostragem;
}

#endif