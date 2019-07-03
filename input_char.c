#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int main(){
int cont = 0;
//asdfafads
char *number;

number = (char *) malloc (10*sizeof(char));

scanf("%s", number);

printf("%s\n", number);

//for (i = 0; i < strlen(number))

//printf("%ld\n", strlen(number));
int potencia = strlen(number) - 1;
for(int i = 0; i < strlen(number); i++, potencia--){
    
    int value = (int) number[i];  
    
    value -=48;      
    cont += value * pow(10, potencia);
  //  printf("%d\n", value);
}

printf("%d\n", cont);
free(number);

return 0;

}