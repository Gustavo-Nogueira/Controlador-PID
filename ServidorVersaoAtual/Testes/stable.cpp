#include <iostream>
#define episilon 0.1
#include <cmath>

using namespace std;

float f(float x){
    return ((-sin(x) * 100) / pow(x, 2));
}

int main()
{
    float valor, referencia;
    int prevValue= 0;
    int faixa = 0;    
    float n = 0.1;
    int count = 0;

    for(float x = 3.0; x < 400.0; x+=n){

        valor = f(x); 
        count ++;

        //cout << valor << endl;

        if((valor - prevValue) <= episilon && (valor - prevValue) >= -episilon){

            faixa++;

            faixa == 1? referencia = valor: referencia = 0; 

            (valor <= (referencia+0.1) && valor >= (referencia-0.1)) ? faixa=faixa: faixa = 0;

            cout << "tempo: " << x - 3.0 << "   Faixa: " << faixa << endl;
            

            if(faixa == 100)
            {
                cout << "Estabilizou" << endl << endl;
                break;
            }

        }

        prevValue = valor;
    }

    cout << "Resultados: " << endl << endl;
    cout << "Quantidade de amostras: " << count << endl;
    cout << "Episilon Considerado: " << 0.1 << endl;
    cout << "Janela de +0.1 e -0.1 a partir do ponto de referencia" << endl;
    cout << "Tempo de amostragem: " << count * 0.1 << " segundos" << endl;

    return 0;
}