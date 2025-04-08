#include "../incl/jogos.h"

int pontuacao=0;

int calculaRandom(){
    return rand() %  10;
}

void trataSinal(){
    exit(pontuacao);
}

int main(){

    signal(SIGUSR1, trataSinal);

    char titulo[] = "Par ou Impar" ;
    char boasvindas[] = "Seja bem vindo ao Par ou Impar";
    char regras[] = "Escreva se quer par ou impar depois tu e o computador irão escolher um numero ganha quem advinhar se o resultado da soma é par ou impar";
    char *resultado[2] = {"Ganhaste","Perdeste"};
    char *estado[2] = {"par","impar"};
    int i=0;
    char palpite[30];
    int numero;

    srand(time(NULL));

    fprintf(stdout,"%s\n",titulo);
    fprintf(stdout,"%s\n",boasvindas);
    fprintf(stdout,"%s\n",regras);
    
    while(1){
        i = calculaRandom();


            fprintf(stdout,"Par ou Impar?\n");
            fscanf(stdin, "%s", palpite);


        fflush(stdin);

        if(strcasecmp(estado[0],palpite)==0){
            fprintf(stdout,"Tu escolheste %s ,agr escolhe um Numero?\n",estado[0]);
            fscanf(stdin,"%d", &numero);
            fprintf(stdout,"O computador escolheu %d ,a soma dos valores deram %d , logo:\n",i,i+numero);
            if((numero+i)%2==0){
                fprintf(stdout,"%s\n",resultado[0]);
                pontuacao+=10;
            }
            else{
                fprintf(stdout,"%s\n",resultado[1]);
            }
        }else if(strcasecmp(estado[1],palpite)==0){
            fprintf(stdout,"Tu escolheste %s ,agr escolhe um Numero?\n",estado[1]);
            fscanf(stdin,"%d", &numero);
            fprintf(stdout,"O computador escolheu %d ,a soma dos valores deram %d, logo:\n",i,i+numero);
            if((numero+i)%2==1){
                fprintf(stdout,"%s\n",resultado[0]);
                pontuacao+=10;
            }
            else{
                
                fprintf(stdout,"%s\n",resultado[1]);
            }
        }
        
    }
}