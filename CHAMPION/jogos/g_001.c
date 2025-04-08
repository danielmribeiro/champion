#include "../incl/jogos.h"
#define NUMEROCONCELHOS 10
int pontuacao=0;

int calculaRandom(){
    return rand() %  NUMEROCONCELHOS;
}

void trataSinal(){
    exit(pontuacao);
}

int main(){

    signal(SIGUSR1, trataSinal);

    char *distrito[NUMEROCONCELHOS]={"Coimbra","Lisboa","Porto","Faro","Braga","Beja","Aveiro","Guarda","Leiria","Viseu"};
    char *concelhos[NUMEROCONCELHOS]={"Figueira da Foz","Sintra","Vila Nova de Gaia","Tavira","Fafe","Cuba","Vagos","Seia","Pombal","Lamego"};
    char titulo[] = "IDENTIFICA O DISTRITO" ;
    char boasvindas[] = "Seja bem vindo ao IDENTIFICA O DISTRITO";
    char regras[] = "Escreva o nome do distrito a que este concelho pertence.";
    char *resultado[2] = {"Certo","Errado"};
    int i=0,numeroDePerguntas=0;
    char palpite[30];

    srand(time(NULL));

    fprintf(stdout,"%s\n",titulo);
    fprintf(stdout,"%s\n",boasvindas);
    fprintf(stdout,"%s\n",regras);

    strcpy(palpite,"djsaidjsa");
    while(1){
        i = calculaRandom();

        fprintf(stdout,"Em que distrito fica %s ? %s\n",concelhos[i], palpite);
        fgets(palpite, 30, stdin);
        palpite[strlen(palpite)-1]='\0';

        if(strcasecmp(distrito[i],palpite)==0){
            fprintf(stdout,"%s\n",resultado[0]);
            pontuacao+=10;
        }else{
            fprintf(stdout,"%s\n",resultado[1]);
            if(pontuacao>=5)
                pontuacao-=5;
        }
        
    numeroDePerguntas++;
    }
}