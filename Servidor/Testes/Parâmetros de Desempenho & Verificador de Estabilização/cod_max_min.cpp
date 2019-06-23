#include <bits/stdc++.h>
using namespace std;

float f(float x){
	float answer = (-(sin(x)*100.0/pow(x,2)) + 0);
return answer;
}


int main(){

int refSignal = 3;
int origin = 1;
bool stage = false;
float eps = 0.1;
float dataSensor;
float teste = 3.0;
float maxi = -1000000, mini = f(3);
int i = 1;
if (refSignal > origin) {

	stage = true;
}

else {
	stage = false;
}

while(1){
	if (i++ == 500) break;
	cout << i << endl;	
	dataSensor = f(teste);
	cout << dataSensor << endl;
	//cout << dataSensor << endl;	
	cout <<  maxi << " " << mini << endl;
	
	if(stage){ 
		if (dataSensor < maxi){
			//cout << "aqui: " << maxi - mini << endl;
			if(abs(maxi - mini) < eps) break;
			mini = 1000;
                stage = false;
		}
			
		
		if (dataSensor > maxi) {
			maxi = dataSensor;
		}
	}
	else{
		if(dataSensor > mini){
			//cout << "aqui: " << maxi - mini << endl;

			if(abs(maxi - mini) < eps) break;
			maxi = -1000000;
                        stage = true;
		}
		
		if (dataSensor < mini){
		       	mini = dataSensor;	
		}
		}
	
	teste += 0.1;
}
return 0;
}
