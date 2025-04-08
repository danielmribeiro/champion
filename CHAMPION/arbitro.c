#include "incl/arbitro.h"
int a_fifo_fd; //identificador do FIFO deste arbitro
int c_fifo_fd; //IDENTIFICADOR DO FIFO DE UM CLIENTE
char c_fifo_fname[50]; //IDENTIFICADOR DO FIFO DE UM CLIENTE
int flag_a_fifo_fd_isAberto=FALSE;
int flag_FIFO_ARBITRO_isCriado=FALSE;
int campeonatoEmBuscaDeJogadores=FALSE;
int campeonatoADecorrer=FALSE; //EXISTE CAMPEONATO ESTÁ A DECORRER?
int MAXPLAYERS=0; //NUMERO MÁXIMO DE JOGADORES (VARIÁVEL COM ATRIBUIÇÃO DE VALOR TARDIA)
char *GAMEDIR; //DIRETORIA ONDE EXISTEM OS JOGOS (VARIÁVEL COM ATRIBUIÇÃO DE VALOR TARDIO)
char jogosDisponiveis[999][TAM_NOME_JOGO]; //LISTA DE JOGOS DETETADOS NO INICIO DE CADA CAMPEONATO
int jogadoresConectados=0; //NUMERO DE JOGADORES NO CAMPEONATO ATUALMENTE
int numeroDeBytesRecebidosDoCliente=0; //USADA NO ATENDIMENTO DO CLIENTE
int numeroDeBytesEnviadosPeloArbitro=0; //USADA NO ATENDIMENTO DO CLIENTE
int numeroDeJogos=0;
int DURACAODOCAMPEONATO = 0; // tempo de duracao de um campeonato (valor recebido do argumento 'd')
int TEMPODEESPERA = 0; // tempo de espera até iniciar o campeonato (valor recebido do argumento 'e')
int tempoDeEsperaManipulavel=0;
int duracaoDoCampeonatoManipulavel=0;
JOGADOR jogador;
ARBITRO arbitro;
CAMPEONATO listaCampeonato[CONSTANTE_MAXIMA_DEFINIDA_NO_ENUNCIADO_DO_TRABALHO];
JOGO listaJogos[CONSTANTE_MAXIMA_DEFINIDA_NO_ENUNCIADO_DO_TRABALHO];
char apoio[TAM_MENSAGEM_ARBITRO];
char* token;

int res;

int iniciaCampeonato=0;


// *************** ERROS ***************
//MENSAGEM DE ERRO - QUANTIDADE DE ARGUMENTOS ERRADA
void MSG_ARBITRO_ERRO_ARGUMENTO_QUANTIDADE(){
    fprintf(stderr,"\n***ERRO:%d***\nQUANTIDADE DE ARGUMENTOS ERRADA\n",EXIT_ERRO_ARGUMENTO_QUANTIDADE);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO DESCONHECIDO
void MSG_ARBITRO_ERRO_ARGUMENTO_DESCONHECIDO(){
    fprintf(stderr,"\n***ERRO:%d***\nARGUMENTO DESCONHECIDO - %c\n",EXIT_ERRO_ARGUMENTO_DESCONHECIDO ,optopt);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTOS 'D' E 'E' NÃO EXISTEM
void MSG_ARBITRO_ERRO_ARGUMENTO_D_E_NAO_EXISTE(){
    fprintf(stderr,"\n***ERRO:%d***\nOS ARGUMENTOS '-D' E '-E' NÃO FORAM DETETADOS\n",EXIT_ERRO_ARGUMENTO_D_E_NAO_EXISTE);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO 'D' NÃO EXISTE
void MSG_ARBITRO_ERRO_ARGUMENTO_D_NAO_EXISTE(){
    fprintf(stderr,"\n***ERRO:%d***\nO ARGUMENTO '-D' NÃO FOI DETETADO\n",EXIT_ERRO_ARGUMENTO_D_NAO_EXISTE);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO 'E' NÃO EXISTE
void MSG_ARBITRO_ERRO_ARGUMENTO_E_NAO_EXISTE(){
    fprintf(stderr,"\n***ERRO:%d***\nO ARGUMENTO '-E' NÃO FOI DETETADO\n",EXIT_ERRO_ARGUMENTO_E_NAO_EXISTE);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO ACONTECIMENTO INESPERADO
void MSG_ARBITRO_ERRO_ARGUMENTO_INESPERADO(){
    fprintf(stderr,"\n***ERRO:%d***\nALGO DE INESPERADO OCORREU NA LEITURA DOS ARGUMENTOS\n",EXIT_ERRO_ARGUMENTO_INESPERADO);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTOS 'D' E 'E' COM VALORES IMPOSSIVEIS
void MSG_ARBITRO_ERRO_ARGUMENTO_D_E_VALOR(){
    fprintf(stderr,"\n***ERRO:%d***\nO VALOR DOS ARGUMENTOS '-D' E '-E' SÃO IMPOSSIVEIS.\nVALOR MINIMO: 1\n",EXIT_ERRO_ARGUMENTO_D_E_VALOR);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO 'D' COM VALOR IMPOSSIVEL
void MSG_ARBITRO_ERRO_ARGUMENTO_D_VALOR(){
    fprintf(stderr,"\n***ERRO:%d***\nO VALOR DO ARGUMENTO '-D' É IMPOSSIVEL.\nVALOR MINIMO: 1\n",EXIT_ERRO_ARGUMENTO_D_VALOR);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ARGUMENTO 'E' COM VALOR IMPOSSIVEL
void MSG_ARBITRO_ERRO_ARGUMENTO_E_VALOR(){
    fprintf(stderr,"\n***ERRO:%d***\nO VALOR DO ARGUMENTO '-E' É IMPOSSIVEL.\nVALOR MINIMO: 1\n",EXIT_ERRO_ARGUMENTO_E_VALOR);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ERRO NA CONFIGURAÇÃO DO SINAL SIGINT
void MSG_ARBITRO_ERRO_SINAL_SIGINT_CONFIGURACAO(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL CONFIGURAR O SINAL SIGINT\n",EXIT_ERRO_SINAL_SIGINT_CONFIGURACAO);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ERRO NA CRIAÇÃO DO FIFO_ARBITRO
void MSG_ARBITRO_ERRO_FIFO_ARBITRO_CRIACAO(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL CRIAR O FIFO DO ARBITRO\n",EXIT_ERRO_FIFO_ARBITRO_CRIACAO);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ERRO AO ABRIR FIFO DO ARBITRO
void MSG_ARBITRO_ERRO_FIFO_ARBITRO_ABRIR(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL ABRIR O FIFO DO ARBITRO\n",EXIT_ERRO_FIFO_ARBITRO_ABRIR);
    fflush(stderr);
}
//MENSAGEM DE ERRO - ERRO AO ABRIR FIFO DO CLIENTE
void MSG_ARBITRO_ERRO_FIFO_CLIENTE_ABRIR(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL ABRIR O FIFO DO CLIENTE\n",EXIT_ERRO_FIFO_CLIENTE_ABRIR);
    fflush(stderr);
}


// *************** INFO ***************
//MENSAGEM DE INFO - ARGUMENTOS DETETADOS COM SUCESSO
void MSG_ARBITRO_INFO_ARGUMENTO_LEITURA_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nOS ARGUMENTOS FORAM INTRODUZIDOS COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - VALOR DOS ARGUMENTOS DETETADOS COM SUCESSO
void MSG_ARBITRO_INFO_ARGUMENTO_VALOR_LEITURA_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nOS VALORES DOS ARGUMENTOS FORAM INTRODUZIDOS COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - CONFIGURAÇÃO DO SINAL SIGINT FEITA COM SUCESSO
void MSG_ARBITRO_INFO_SINAL_SIGINT_CONFIGURACAO_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nCONFIGURAÇÃO DO SINAL SIGINT FEITA COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - ARBITRO A TERMINAR (INTERROMPIDO VIA TECLADO)
void MSG_ARBITRO_INFO_SINAL_INTERRUPCAO_COM_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nARBITRO A TERMINAR (INTERROMPIDO VIA TECLADO)\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - DURACAO DO CAMPEONATO
void MSG_ARBITRO_INFO_DURACAO_DO_CAMPEONATO(int DURACAODOCAMPEONATO){
    fprintf(stderr, "\n***INFO***\nDURAÇÃO DO CAMPEONATO: %d\n", DURACAODOCAMPEONATO);
    fflush(stderr);
}
//MENSAGEM DE INFO - TEMPO DE ESPERA
void MSG_ARBITRO_INFO_TEMPO_DE_ESPERA(int TEMPODEESPERA){
    fprintf(stderr, "\n***INFO***\nTEMPO DE ESPERA: %d\n", TEMPODEESPERA);
    fflush(stderr);
}
//MENSAGEM DE INFO - VARIAVEL DE AMBIENTE GAMEDIR DETETADA
void MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_GAMEDIR_DETETADA(char *GAMEDIR){
    fprintf(stderr, "\n***INFO***\nA VARIÁVEL DE AMBIENTE GAMEDIR FOI DETETADA COM SUCESSO.\nGAMEDIR ENCONTRA-SE EM: %s\n",GAMEDIR);
    fflush(stderr);
}
//MENSAGEM DE INFO - VARIAVEL DE AMBIENTE GAMEDIR NÃO DETETADA
void MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_GAMEDIR_NAO_DETETADA(char *GAMEDIR){
    fprintf(stderr, "\n***INFO***\nA VARIÁVEL DE AMBIENTE GAMEDIR NÃO FOI DETETADA.\nGAMEDIR VAI TOMAR O VALOR DA DIRETORIA ATUAL: %s\n",GAMEDIR);
    fflush(stderr);
}
//MENSAGEM DE INFO - VARIAVEL DE AMBIENTE MAXPLAYERS DETETADA
void MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_MAXPLAYERS_DETETADA(int MAXPLAYERS){
    fprintf(stderr, "\n***INFO***\nA VARIÁVEL DE AMBIENTE MAXPLAYERS FOI DETETADA COM SUCESSO.\nMAXPLAYERS POSSUI O VALOR: %d\n",MAXPLAYERS);
    fflush(stderr);
}
//MENSAGEM DE INFO - VARIAVEL DE AMBIENTE MAXPLAYERS NÃO DETETADA
void MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_MAXPLAYERS_NAO_DETETADA(int MAXPLAYERS){
    fprintf(stderr, "\n***INFO***\nA VARIÁVEL DE AMBIENTE MAXPLAYERS NÃO FOI DETETADA.\nMAXPLAYERS VAI TOMAR O VALOR PREDEFINIDO: %d\n",MAXPLAYERS);
    fflush(stderr);
}
//MENSAGEM DE INFO - FIFO DO ARBITRO CRIADO COM SUCESSO
void MSG_ARBITRO_INFO_FIFO_ARBITRO_CRIACAO_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO ARBITRO FOI CRIADO COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - FIFO DO ARBITRO ABRIR COM SUCESSO
void MSG_ARBITRO_INFO_FIFO_ARBITRO_ABRIR_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO ARBITRO FOI ABERTO COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - FIFO DO CLIENTE ABRIR COM SUCESSO
void MSG_ARBITRO_INFO_FIFO_CLIENTE_ABRIR_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO CLIENTE FOI ABERTO COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - FIFO DO CLIENTE FECHADO COM SUCESSO
void MSG_ARBITRO_INFO_FIFO_CLIENTE_FECHADO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO CLIENTE FOI FECHADO COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - ALGO DE ANORMAL ACONTECEU COM OS DADOS RECEBIDOS
void MSG_ARBITRO_INFO_DADOS_RECEBIDOS_ANORMAL(int numeroDeBytes){
    fprintf(stderr, "\n***INFO***\nSEM RESPOSTA OU RESPOSTA INCOMPREENSIVEL. [NUMERO DE BYTES RECEBIDOS:%d]\n",numeroDeBytes);
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DO SEGUINTE CLIENTE RECEBIDA
void MSG_ARBITRO_INFO_MENSAGEM_CLIENTE_RECEBIDA(int validID, char *nomeJogador, pid_t pidCliente, char *mensagem){
    fprintf(stderr, "\n***INFO***\nRECEBIDA MENSAGEM DO CLIENTE:\nVALIDID:%d\nNOMEID:%s\nPID_CLIENTE:%d\nMENSAGEM:%s\n",validID, nomeJogador, pidCliente, mensagem);
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DE RESPOSTA ENVIADA AO CLIENTE COM SUCESSO
void MSG_ARBITRO_INFO_ENVIO_RESPOSTA_CLIENTE_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nA RESPOSTA FOI ENVIADA COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DE COUNTDOWN DE TEMPO DE ESPERA NO LOBBY
void MSG_ARBITRO_INFO_CAMPEONATO_COUNTDOWN_TEMPO_DE_ESPERA(int tempo){
    fprintf(stderr, "\n***INFO***\nFALTAM %d SEGUNDOS PARA FECHAR O LOBBY DE ACESSO AO CAMPEONATO\n",tempo);
    fflush(stderr);
}
//MENSAGEM DE INFO - ERRO AO ENVIAR A RESPOSTA AO CLIENTE
void MSG_ARBITRO_INFO_ENVIO_RESPOSTA_CLIENTE_ERRO_INESPERADO(){
    fprintf(stderr,"\n***INFO***\nHOUVE UM ERRO INESPERADO NO ENVIO DE RESPOSTA AO CLIENTE\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DE INICIO DO CAMPEONATO
void MSG_ARBITRO_INFO_CAMPEONATO_INICIO_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO ÁRBITRO DARÁ INICIO AO CAMPEONATO COM SUCESSO\n");
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DE COUNTDOWN DA DURAÇÃO DO CAMPEONATO
void MSG_ARBITRO_INFO_CAMPEONATO_COUNTDOWN_DURACAO_DO_CAMPEONATO(int tempo){
    fprintf(stderr, "\n***INFO***\nFALTAM %d SEGUNDOS PARA ACABAR O CAMPEONATO\n",tempo);
    fflush(stderr);
}
//MENSAGEM DE INFO - MENSAGEM DE FIM DO CAMPEONATO
void MSG_ARBITRO_INFO_CAMPEONATO_FIM_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO ÁRBITRO ACABOU O CAMPEONATO COM SUCESSO\n");
    fflush(stderr);
}


// *************** UI ***************
//MENSAGEM DE UI - LISTAR JOGADOR E JOGO
void MSG_ARBITRO_UI_LISTAR_JOGADOR_E_JOGO(char *nomeDoJogador, char *nomeDoJogo){
    fprintf(stdout,"\t%s\t|\t%s\n",nomeDoJogador,nomeDoJogo);
    fflush(stdout);
}
//MENSAGEM DE UI - LISTAR JOGO
void MSG_ARBITRO_UI_LISTAR_JOGO(char *nomeDoJogo){
    fprintf(stdout,"\t%s\n",nomeDoJogo);
    fflush(stdout);
}
void MSG_ARBITRO_UI_KICK_JOGADOR(char *nomeDoJogador){
    fprintf(stdout,"\tKICK AO JOGADOR \"%s\" EFETUADO COM SUCESSO\n",nomeDoJogador);
    fflush(stdout);
}

// *************** AJUDA ***************
//MENSAGEM DE AJUDA - FORMATAÇÃO DOS ARGUMENTOS 
void MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO(){
    fprintf(stderr,"\n***AJUDA***\nDeve iniciar o arbitro com:\ne -> tempo de espera\nd -> duracao do campeonato\n\npor favor use o formato:\n./arbitro -e <numero> -d <numero>\nou\n./arbitro -d <numero> -e <numero>\n");
    fflush(stderr);
}

void cleanListaCampeonato(){
    for(int i=0; i< sizeof(listaCampeonato)/sizeof(listaCampeonato[0]);i++){
        if(listaCampeonato[i].pid_cliente!=0){
            fprintf(stdout,"enviei mensagem ao %d para terminar do campeonato",listaCampeonato[i].pid_cliente);
            kill(listaCampeonato[i].pid_cliente, SIGUSR2);
        }
        memset(&listaCampeonato[i],0,sizeof(listaCampeonato[i]));
    }
    jogadoresConectados=0;
}


void handler_SIGINT(){
    MSG_ARBITRO_INFO_SINAL_INTERRUPCAO_COM_SUCESSO();
    if(flag_a_fifo_fd_isAberto==TRUE){
        close(a_fifo_fd);
        flag_a_fifo_fd_isAberto=FALSE;
    }
    if(flag_FIFO_ARBITRO_isCriado==TRUE){
        unlink(FIFO_ARBITRO);
        flag_FIFO_ARBITRO_isCriado=FALSE;
    }
    exit(EXIT_SUCCESS);
}

void atualizarJogosDisponiveis(){
    DIR *diretoriaOndeEstaoOsJogos;
    struct dirent * entrada;
    if ((diretoriaOndeEstaoOsJogos = opendir(GAMEDIR)) == NULL)
        fprintf(stdout,"\nerro em opendir()");
    else {
        fprintf(stdout,"\n--Conteúdo de %s --\n",GAMEDIR);
        numeroDeJogos=0;
        while ((entrada = readdir(diretoriaOndeEstaoOsJogos)) != NULL)
            if(entrada->d_name[0]=='g' && entrada->d_name[1]=='_' && entrada->d_name[strlen(entrada->d_name) - 2] != '.'){
                //fprintf(stdout,"  %s\n", entrada->d_name);
                strcpy(jogosDisponiveis[numeroDeJogos],entrada->d_name);
                numeroDeJogos++;
            }
        closedir(diretoriaOndeEstaoOsJogos);
    }
}

int isNomeDoJogadorOcupado(){
    for(int i=0; i<MAXPLAYERS;i++){
        if(strcasecmp(listaCampeonato[i].nomeJogador, jogador.msg)==0){
            return TRUE;
        }
    }
    return FALSE;
}

void kickJogador(char* nome){
    int encontrou=0;
    for(int i=0; i < jogadoresConectados; i++){
        if(strcasecmp(nome,listaCampeonato[i].nomeJogador)==0){
            encontrou=TRUE;
            kill(listaCampeonato[i].pid_cliente, SIGUSR2);
            MSG_ARBITRO_UI_KICK_JOGADOR(listaCampeonato[i].nomeJogador);
        }
        if(encontrou == TRUE){
            listaCampeonato[i]=listaCampeonato[i+1];
        }
    }
    jogadoresConectados-=encontrou;
}

void handler_alarm_countdownLobbyCampeonato(){
    if(tempoDeEsperaManipulavel!=0){
        MSG_ARBITRO_INFO_CAMPEONATO_COUNTDOWN_TEMPO_DE_ESPERA(tempoDeEsperaManipulavel);
        tempoDeEsperaManipulavel--;
        alarm(1);
    }else{
        tempoDeEsperaManipulavel=TEMPODEESPERA;
        campeonatoEmBuscaDeJogadores=FALSE;
        campeonatoADecorrer=TRUE;
        MSG_ARBITRO_INFO_CAMPEONATO_INICIO_SUCESSO();
        //alarm(0);
    }
}

int idNaLista(char * nome){
    for(int i=0; i<MAXPLAYERS;i++){
        if(strcasecmp(listaCampeonato[i].nomeJogador, nome)==0){
            return i;
        }
    }
    return -1;
}

void handler_alarm_countdownAteAoFimDoCampeonato(){
    if(duracaoDoCampeonatoManipulavel!=0){
        MSG_ARBITRO_INFO_CAMPEONATO_COUNTDOWN_DURACAO_DO_CAMPEONATO(duracaoDoCampeonatoManipulavel);
        duracaoDoCampeonatoManipulavel--;
        alarm(1);
    }else{
        duracaoDoCampeonatoManipulavel=DURACAODOCAMPEONATO;
        campeonatoADecorrer=FALSE;
        MSG_ARBITRO_INFO_CAMPEONATO_FIM_SUCESSO();
        iniciaCampeonato = 0;
        alarm(0);
    }
}

char* buscaMyGame(){
    for(int i=0; i<MAXPLAYERS;i++){
        if(strcasecmp(listaCampeonato[i].nomeJogador, jogador.nomeID)==0){
            if(strcasecmp(listaCampeonato[i].nomeJogo, "")==0){
                return "nenhum jogo atribuido";
            }else{
                return listaCampeonato[i].nomeJogo;
            }
        }
    }
    return "ERRO: JOGADOR NÃO ENCONTRADO NO CAMPEONATO";
}

char * jogoRandom(){
    int numeroAtribuido =rand() %  numeroDeJogos;
    return jogosDisponiveis[numeroAtribuido];
}

void * atendeComandosCliente(){
    while(TRUE){
        numeroDeBytesRecebidosDoCliente = read(a_fifo_fd, &jogador, sizeof(jogador));
        if(numeroDeBytesRecebidosDoCliente<sizeof(jogador)){
            MSG_ARBITRO_INFO_DADOS_RECEBIDOS_ANORMAL(numeroDeBytesRecebidosDoCliente);
        }
        MSG_ARBITRO_INFO_MENSAGEM_CLIENTE_RECEBIDA(jogador.validID, jogador.nomeID, jogador.pid_cliente, jogador.msg);
        if(jogador.validID==FALSE){
            if(campeonatoEmBuscaDeJogadores==FALSE){
                sprintf(apoio, "NÃO FOI POSSIVEL ENTRAR NO CAMPEONATO PORQUE JÁ ESTÁ A DECORRER. FALTA(M) %d SEGUNDO(S) PARA O CAMPEONATO TERMINAR.",duracaoDoCampeonatoManipulavel);
                strcpy(arbitro.msg, apoio);
            }else{
                if(jogadoresConectados<MAXPLAYERS){
                    if(isNomeDoJogadorOcupado(listaCampeonato)==FALSE){
                        strcpy(arbitro.msg,"1");
                        listaCampeonato[jogadoresConectados].pid_cliente = jogador.pid_cliente;
                        strcpy(listaCampeonato[jogadoresConectados].nomeJogador , jogador.msg); //O username do jogador ainda está no jogador.msg, ele será atualizado no futuro para jogador.nomeID
                        strcpy(listaCampeonato[jogadoresConectados].nomeJogo, jogoRandom());
                        ++jogadoresConectados;
                    }else{
                        strcpy(arbitro.msg,"0");
                    }
                }else{
                    sprintf(apoio, "O LOBBY DO CAMPEONATO ESTÁ CHEIO! ESPERE QUE ALGUÉM DESISTA E TENTE A SUA SORTE DE NOVO OU TENTE ENTRAR NO PROXIMO CAMPEONATO! FALTA(M) %d SEGUNDO(S) PARA O INICIO DO LOBBY DO PRÓXIMO CAMPEONATO",duracaoDoCampeonatoManipulavel+tempoDeEsperaManipulavel);
                    strcpy(arbitro.msg, apoio);
                }
            }
        }else{
            if(jogador.msg[0]=='#'){
                if(strcasecmp(jogador.msg,"#mygame")==0){
                    strcpy(arbitro.msg,buscaMyGame(listaCampeonato));
                }else if(strcasecmp(jogador.msg, "#quit")==0){
                    kickJogador(jogador.nomeID);
                    strcpy(arbitro.msg, "Desistência do campeonato aprovada com sucesso!");
                }else{
                    strcpy(arbitro.msg, "Instrução direcionada ao árbitro desconhecida");
                }
            }else if(campeonatoADecorrer == TRUE){
                    // Reencaminha mensagem para o jogo
                    //encontrar o cliente na lista userList e usar esse file Descriptor para ser igual à da thread.
                    int idL= idNaLista(jogador.nomeID);
                    char str[100],nl = '\n';
                    int readRes;
                    //strncat(mensagemForServer.palavra, &nl, 1);
                    printf("\nOLA\n");
                    jogador.msg[strlen(jogador.msg)+1] = '\0';
                    jogador.msg[strlen(jogador.msg)] = '\n';
                    //printf("Vou mandar '%s'\n", mensagemForServer.palavra);
                    res = write(*jogador.msg, jogador.msg, strlen(jogador.msg));
                    printf("\nOLA2\n");
                    usleep(250); // The program needs to rest a bit as well.
                    printf("\nOLA3\n");
                    memset(jogador.msg, '\0', sizeof(jogador.msg));

                    //char *nomeJogador = (char*) nomeJogad;
                    int valorRetornado;
                    pid_t childpid;
                    char game[100];

                    int writepipe[2] = {-1. - 1}; // pipe (write) arbitro > jogo
                    int readpipe[2] = {-1. - 1}; // pipe de retorno (read) jogo > arbitro

                    char *nomeDoJogo;

                    for(int i=0; i<MAXPLAYERS;i++){
                        if(strcasecmp(listaCampeonato[i].nomeJogador, jogador.nomeID)==0){
                            nomeDoJogo=listaCampeonato[i].nomeJogo;
                            break;
                        }
                    }
                    
                    writepipe[READ_END] = -1;
                    if (pipe(readpipe) < 0 || pipe(writepipe) < 0) {
                        printf("!!!!! FATAL: comunication not allowed !!!!!\n");
                        return 0;
                    }
                    int id=fork();

                    switch(id){
                        case -1:
                            fprintf(stderr, "erro");
                            return 0;
                            break;
                        case 0:
                            idL=idNaLista(jogador.nomeID);
                            listaCampeonato[idL].pid_jogo=getpid();
                            //printf("Estou no filho! Vou fechar o readpipe[READ_END]!\n");
                            close(readpipe[READ_END]);
                            //printf("Estou no filho! Fechei o readpipe[READ_END] e vou fechar o writepipe[WRITE_END]!\n");
                            close(writepipe[WRITE_END]);
                            //printf("Estou no filho! Fechei o writepipe[WRITE_END] e vou fazer o dup2 do STDIN !\n");
                            dup2(writepipe[READ_END], STDIN_FILENO);
                            //printf("Estou no filho! Fiz o dup2 do STDIN e vou fazer o dup2 do STDOUT!\n");
                            dup2(readpipe[WRITE_END], STDOUT_FILENO);
                            //printf("Estou no filho! Fiz o dup2 do STDOUT e vou fechar o writepipe[READ_END]!\n");
                            close(writepipe[READ_END]);
                            //printf("Estou no filho! Fechei o writepipe[READ_END] e vou fechar o readpipe[WRITE_END]!\n");
                            close(readpipe[WRITE_END]);
                            //printf("Estou no filho! Fechei o readpipe[WRITE_END] e a proxima linha é o execl()!\n");

                            //execl(GAMEDIR, strcat("./",nomeDoJogo), NULL, NULL);

                            sprintf(game, "%s/%s", GAMEDIR, listaCampeonato[idL].nomeJogo);
                            execl(game, listaCampeonato[idL].nomeJogo, NULL, NULL);
                            //printf("Estou no filho! O execl já passou. UPSSSS!\n");

                            break;
                                
                        default:
                            //printf("Estou no pai! Vou fechar o writepipe[READ_END]!\n");
                            close(writepipe[READ_END]);
                            //printf("Estou no pai! Fechei o writepipe[READ_END] e vou fechar o readpipe[WRITE_END]!\n");
                            close(readpipe[WRITE_END]);

                            //printf("Estou no pai! Vou escrever coisas no write!\n");
                            write(writepipe[WRITE_END], jogador.msg, strlen(jogador.msg));
                            //printf("Estou no pai! Escrevi coisas no write mas não fiz fechei!\n");
                            close(writepipe[WRITE_END]);
                            //printf("Estou no pai! Escrevi coisas no write e já o fechei!\n");

                            //printf("Estou no pai! O nbytes não tem nada!\n");
                            int nbytes = read(readpipe[READ_END], arbitro.msg, sizeof(arbitro));
                            //printf("Estou no pai! O nbytes está a lojar um valor!\n");
                            close(readpipe[READ_END]);
                            //printf("Vou fazer Wait(NULL)!\n");
                            wait(NULL);
                            //printf("Fiz Wait(NULL)!\n");
                            //printf("Output nybtes, estado: %d, %s", nbytes, estado);
                            //printf("Output estado: %s", estado);
                            printf("Mensagem Arbitro: %s\n", arbitro.msg);

                            break;
                    }
                } else {
                    strcpy(arbitro.msg, "Comando nao reconhecido.\n\n");
            }
        }

    //OBTEM FILENAME DO FIFO PARA A RESPOSTA */
        sprintf(c_fifo_fname, FIFO_CLIENTE, jogador.pid_cliente);

    //ABRE FIFO DO CLIENTE PARA ESCRITA
        c_fifo_fd = open(c_fifo_fname, O_WRONLY);
        if(c_fifo_fd == -1){
            MSG_ARBITRO_ERRO_FIFO_CLIENTE_ABRIR();
        }else{
            MSG_ARBITRO_INFO_FIFO_CLIENTE_ABRIR_SUCESSO();

            /* ---- ENVIA RESPOSTA ---- */
            numeroDeBytesEnviadosPeloArbitro = write(c_fifo_fd, &arbitro, sizeof(arbitro));
            if(numeroDeBytesEnviadosPeloArbitro == sizeof(arbitro)){
                MSG_ARBITRO_INFO_ENVIO_RESPOSTA_CLIENTE_SUCESSO();
            }else{
                MSG_ARBITRO_INFO_ENVIO_RESPOSTA_CLIENTE_ERRO_INESPERADO();
            }
            close(c_fifo_fd);//FECHA FIFO DO CLIENTE
            MSG_ARBITRO_INFO_FIFO_CLIENTE_FECHADO();
        }
    }
}


void * gerirCampeonato(){
    while(TRUE){
        signal(SIGALRM, handler_alarm_countdownLobbyCampeonato);
        tempoDeEsperaManipulavel=TEMPODEESPERA;
        duracaoDoCampeonatoManipulavel=DURACAODOCAMPEONATO;
        atualizarJogosDisponiveis();
        campeonatoEmBuscaDeJogadores=TRUE;
        while(jogadoresConectados<2){}
        alarm(1);
        while(campeonatoADecorrer==FALSE){};
        alarm(0);
        signal(SIGALRM, handler_alarm_countdownAteAoFimDoCampeonato);
        alarm(1);
        while(campeonatoADecorrer==TRUE){}
        cleanListaCampeonato();
    }
}


void menuAdministrador(){
    char adminCmd[TAM_USERNAME+10];
        while(TRUE){
            scanf("%s", adminCmd);

            if(strcasecmp(adminCmd, "exit")==0){
                kill(getpid(), SIGINT); //SAIR DE FORMA ORDEIRA
            }

            if(strcasecmp(adminCmd, "games")==0){
                atualizarJogosDisponiveis();
                for (int i = 0; i < numeroDeJogos; i++){
                    MSG_ARBITRO_UI_LISTAR_JOGO(jogosDisponiveis[i]);
                }
            }

            if(adminCmd[0]=='k'&&adminCmd[1]!='\0'){
                token = strtok(adminCmd, "k");
                for (int l = 0; l < jogadoresConectados; l++) {
                    if (strcmp(listaCampeonato[l].nomeJogador, token) == 0){
                        kickJogador(listaCampeonato[l].nomeJogador);
                    }
                }
            }

            if(adminCmd[0]=='s'&&adminCmd[1]!='\0'){
                ///////////////////////////////////////////////////////////////////////////////////////suspender comunicação entre jogdador e jogo (o clnte é informado do que se passa)
            }

            if(adminCmd[0]=='r'&&adminCmd[1]!='\0'){
                //////////////////////////////////////////////////////////////////////////////////retomar comunicação entre jogdador e jogo (o clnte é informado do que se passa)
            }

            if(strcasecmp(adminCmd, "end")==0){
                duracaoDoCampeonatoManipulavel=0;
            }

            if(strcasecmp(adminCmd, "players")==0){//LISTAR JOGADORES EM JOGO (JOGADOR E JOGO CORRESPONDENTE)
                for (int i = 0; i < jogadoresConectados; i++){
                    MSG_ARBITRO_UI_LISTAR_JOGADOR_E_JOGO(listaCampeonato[i].nomeJogador, listaCampeonato[i].nomeJogo);
                }
            }
        }
}

int main(int argc, char *argv[], char **envp){
    int option = 0; // usado no tratamento dos argumentos
    int d_flag=FALSE, e_flag=FALSE; // bandeiras relativas aos argumentos 'd' e 'e' para indicar se cada um dos argumentos já foram analisados.
    char *str = NULL; //usada na validação das variáveis de ambiente
    pthread_t threadAtendimentoClientes, threadGerirCampeonato;
    time_t t;
    srand((unsigned) time(&t));

//LER E TRATAR ARGUMENTOS    
    //VALIDAÇÃO DA QUANTIDADE DE ARGUMENTOS
    if(argc!=5){
        MSG_ARBITRO_ERRO_ARGUMENTO_QUANTIDADE();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_QUANTIDADE);
    }
    //LER ARGUMENTOS
    while((option = getopt(argc, argv, "d:e:"))!=-1){ //enquanto houver argumentos faz o ciclo (lê o argumento 2, depois o 1, depois o 0 e depois sai ao chegar ao -1)    
        switch(option){
            case 'd' :
                DURACAODOCAMPEONATO = atof(optarg);
                d_flag=TRUE;
                break;
            case 'e' :
                TEMPODEESPERA = atof(optarg);
                e_flag=TRUE;
                break;
            case '?' :
                MSG_ARBITRO_ERRO_ARGUMENTO_DESCONHECIDO();
                MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
                exit(EXIT_ERRO_ARGUMENTO_DESCONHECIDO);
        }
    }
    //CONFIRMAÇÂO DA LEITURA CORRETA DOS ARGUMENTOS
    if(d_flag==FALSE && e_flag==FALSE){
        MSG_ARBITRO_ERRO_ARGUMENTO_D_E_NAO_EXISTE();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_D_E_NAO_EXISTE);
    }else if(d_flag==TRUE && e_flag==FALSE){
        MSG_ARBITRO_ERRO_ARGUMENTO_E_NAO_EXISTE();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_E_NAO_EXISTE);
    }else if(d_flag==FALSE && e_flag==TRUE){
        MSG_ARBITRO_ERRO_ARGUMENTO_D_NAO_EXISTE();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_D_NAO_EXISTE);
    }else if(d_flag==TRUE && e_flag==TRUE){
        MSG_ARBITRO_INFO_ARGUMENTO_LEITURA_SUCESSO();
    }else{
        MSG_ARBITRO_ERRO_ARGUMENTO_INESPERADO();
        exit(EXIT_ERRO_ARGUMENTO_INESPERADO);
    }
    //CONFIRMAÇÃO DA LEITURA CORRETA DO VALOR DOS ARGUMENTOS
    if(DURACAODOCAMPEONATO<1 && TEMPODEESPERA<1){
        MSG_ARBITRO_ERRO_ARGUMENTO_D_E_VALOR();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_D_E_VALOR);
    }else if(DURACAODOCAMPEONATO<1 && TEMPODEESPERA>0){
        MSG_ARBITRO_ERRO_ARGUMENTO_D_VALOR();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_D_VALOR);
    }else if(DURACAODOCAMPEONATO>0 && TEMPODEESPERA<1){
        MSG_ARBITRO_ERRO_ARGUMENTO_E_VALOR();
        MSG_ARBITRO_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_E_VALOR);
    }else if(DURACAODOCAMPEONATO>0 && TEMPODEESPERA>0){
        MSG_ARBITRO_INFO_ARGUMENTO_VALOR_LEITURA_SUCESSO();
    }else{
        MSG_ARBITRO_ERRO_ARGUMENTO_INESPERADO();
        exit(EXIT_ERRO_ARGUMENTO_INESPERADO);
    }
    //VALORES RECEBIDOS DOS ARGUMENTOS
    MSG_ARBITRO_INFO_DURACAO_DO_CAMPEONATO(DURACAODOCAMPEONATO);
    MSG_ARBITRO_INFO_TEMPO_DE_ESPERA(TEMPODEESPERA);

//DETEÇÃO DAS VARIÁVEIS DE AMBIENTE
    MAXPLAYERS = MAXPLAYERS_DEFAULT; //valor default do MAXPLAYERS (é tratado nas validações)
    GAMEDIR = GAMEDIR_DEFAULT; //valor default do GAMEDIR (é tratado nas validações)
    
    str = getenv("GAMEDIR");
    if(str!=NULL){ //Se a variável de ambiente existir
        strcpy(GAMEDIR,str); //o valor da variável de ambiente passará para o GAMEDIR
        MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_GAMEDIR_DETETADA(GAMEDIR);
    }else{ // se a variável de ambiente não existir
        MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_GAMEDIR_NAO_DETETADA(GAMEDIR);
    }

    str = getenv("MAXPLAYERS");
    if(str!=NULL){ //Se a variável de ambiente existir
        MAXPLAYERS = atoi(str); //o valor da variável de ambiente passará para o MAXPLAYERS
		MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_MAXPLAYERS_DETETADA(MAXPLAYERS);
    }else{ // se a variável de ambiente não existir
        MSG_ARBITRO_INFO_VARIAVEL_AMBIENTE_MAXPLAYERS_NAO_DETETADA(MAXPLAYERS);
    }

    

//CONFIGURAR O SINAL PARA FECHAR O ÁRBITRO EM SEGURANÇA
    if(signal(SIGINT, handler_SIGINT)==SIG_ERR){
        MSG_ARBITRO_ERRO_SINAL_SIGINT_CONFIGURACAO();
        exit(EXIT_ERRO_SINAL_SIGINT_CONFIGURACAO);
    }else{
        MSG_ARBITRO_INFO_SINAL_SIGINT_CONFIGURACAO_SUCESSO();
    }

//CRIAÇÃO DO FIFO DO ÁRBITRO
    if(mkfifo(FIFO_ARBITRO, 0666) == -1){ //0666 (octal) == binary 0b 110 110 110 == permissions rw-rw-rw-
        MSG_ARBITRO_ERRO_FIFO_ARBITRO_CRIACAO();
        exit(EXIT_ERRO_FIFO_ARBITRO_CRIACAO);
    }
    flag_FIFO_ARBITRO_isCriado=TRUE;
    MSG_ARBITRO_INFO_FIFO_ARBITRO_CRIACAO_SUCESSO();
    
//ABRE O FIFO DO ARBITRO PARA LEITURA E ESCRITA
    a_fifo_fd = open(FIFO_ARBITRO, O_RDWR);
    if(a_fifo_fd == -1){
        MSG_ARBITRO_ERRO_FIFO_ARBITRO_ABRIR();
        unlink(FIFO_ARBITRO); //ELIMINA O FIFO DO ARBITRO
        flag_FIFO_ARBITRO_isCriado=FALSE;
        exit(EXIT_ERRO_FIFO_ARBITRO_ABRIR);
    }
    flag_a_fifo_fd_isAberto=TRUE;
    MSG_ARBITRO_INFO_FIFO_ARBITRO_ABRIR_SUCESSO();

    pthread_create(&threadAtendimentoClientes, NULL, atendeComandosCliente, NULL);
    pthread_create(&threadGerirCampeonato, NULL, gerirCampeonato, NULL);
    menuAdministrador(listaCampeonato);
    //pthread_join(threadAtendimentoClientes, NULL);
    //pthread_join(threadGerirCampeonato, NULL);

    exit(EXIT_SUCCESS);
}