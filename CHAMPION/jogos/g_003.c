#include "../incl/jogos.h"
#define NUMEROCONCELHOS 10
int pontuacao=0;

int calculaRandom(){
    return rand() %  100;
}

void trataSinal(){
    exit(pontuacao);
}

int main(){

    signal(SIGUSR1, trataSinal);

    char titulo[] = "Calcula Soma" ;
    char boasvindas[] = "Seja bem vindo ao Calcula Soma";
    char regras[] = "Escreva o resultado da soma";
    char *resultado[2] = {"Certo","Errado"};
    int i=0,j=0,res=0,numero=0;

    srand(time(NULL));

    fprintf(stdout,"%s\n",titulo);
    fprintf(stdout,"%s\n",boasvindas);
    fprintf(stdout,"%s\n",regras);

    
    while(1){
        i = calculaRandom();
        j = calculaRandom();
        res = i+j;

        fprintf(stdout,"qual o resultado da soma %d + %d ?\n",i,j);
        fscanf(stdin,"%d",&numero);

        if(res == numero){
            fprintf(stdout,"%s\n",resultado[0]);
            pontuacao+=10;
        }else{
            fprintf(stdout,"%s\n",resultado[1]);
            if(pontuacao>=5)
                pontuacao-=5;
        }
        
    }
}