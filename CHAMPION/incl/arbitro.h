#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

// VARIAVEIS DE AMBIENTE DEFAULT
#define MAXPLAYERS_DEFAULT 30
#define GAMEDIR_DEFAULT strcat(getenv("PWD"),"/jogos"); //getenv(GAMEDIRdefault) é um valor não constante

#define CONSTANTE_MAXIMA_DEFINIDA_NO_ENUNCIADO_DO_TRABALHO 30 //para facilitar o código, optamos por utilizar este valor para a dimensao da lista de jogadores no campeonato(fazendo depois dentro do código a comparação com o MAXPLAYERS), visto que no enunciado diz que o numero maximo de jogadores nao será maior que 30
