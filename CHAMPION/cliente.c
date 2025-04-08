#include "incl/cliente.h"
int countdownTempoDeEspera=TEMPO_DE_ESPERA; //TEMPO DE ESPERA ATÉ ENCONTRAR A CONEXÃO COM O ARBITRO
JOGADOR jogador;
ARBITRO arbitro;
char c_fifo_fname[25]; //NOME DO FIFO DESTE CLIENTE
int a_fifo_fd; //IDENTIFICADOR DO FIFO DO ARBITRO
int c_fifo_fd; //IDENTIFICADOR DO FIFO DESTE CLIENTE
//int campeonatoADecorrerNoArbitro=FALSE; //EXISTE CAMPEONATO DO LADO DO ARBITRO?
int estouAParticiparNoCampeonatoADecorrerAtualmente=FALSE; //ESTOU A PARTICIPAR NO CAMPEONATO A DECORRER ATUALMENTE?
int flag_c_fifo_fname_isCriado=FALSE;
int flag_c_fifo_fname_isAberto=FALSE;
int flag_a_fifo_fd_isAberto=FALSE;
int numeroDeBytesEnviados=0;
int numeroDeBytesRecebidos=0;


// *************** ERROS ***************
//MENSAGEM DE ERRO - QUANTIDADE DE ARGUMENTOS ERRADA
void MSG_CLIENTE_ERRO_ARGUMENTO_QUANTIDADE(){
    fprintf(stderr,"\n***ERRO:%d***\nQUANTIDADE DE ARGUMENTOS ERRADA\n",EXIT_ERRO_ARGUMENTO_QUANTIDADE);
}
//MENSAGEM DE ERRO - ERRO NA CONFIGURAÇÃO DO SINAL SIGUSR1
void MSG_CLIENTE_ERRO_SINAL_SIGUSR1_CONFIGURACAO(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL CONFIGURAR O SINAL SIGUSR1\n",EXIT_ERRO_SINAL_SIGUSR1_CONFIGURACAO);
}
//MENSAGEM DE ERRO - ERRO NA CONFIGURAÇÃO DO SINAL SIGUSR2
void MSG_CLIENTE_ERRO_SINAL_SIGUSR2_CONFIGURACAO(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL CONFIGURAR O SINAL SIGUSR2\n",EXIT_ERRO_SINAL_SIGUSR2_CONFIGURACAO);
}
//MENSAGEM DE ERRO - ERRO NA CONEXÃO COM O ÁRBITRO
void MSG_CLIENTE_ERRO_CONEXAO_COM_ARBITRO(){
    fprintf(stderr,"\n***ERRO:%d***\nNÃO FOI POSSIVEL A CONEXÃO COM O ÁRBITRO\n",EXIT_ERRO_SINAL_SIGUSR1_CONFIGURACAO);
}
//MENSAGEM DE ERRO - ERRO NA CRIAÇÃO DO FIFO_CLIENTE
void MSG_CLIENTE_ERRO_FIFO_CLIENTE_CRIACAO(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL CRIAR O FIFO DO CLIENTE\n",EXIT_ERRO_FIFO_CLIENTE_CRIACAO);
}
//MENSAGEM DE ERRO - ERRO AO ABRIR FIFO DO ARBITRO
void MSG_CLIENTE_ERRO_FIFO_ARBITRO_ABRIR(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL ABRIR O FIFO DO ARBITRO\n",EXIT_ERRO_FIFO_ARBITRO_ABRIR);
}
//MENSAGEM DE ERRO - ERRO AO ABRIR FIFO DO CLIENTE
void MSG_CLIENTE_ERRO_FIFO_CLIENTE_ABRIR(){
    fprintf(stderr,"\n***ERRO:%d***\nNAO FOI POSSIVEL ABRIR O FIFO DO CLIENTE\n",EXIT_ERRO_FIFO_CLIENTE_ABRIR);
}

// *************** INFO ***************
//MENSAGEM DE INFO - CONFIGURAÇÃO DO SINAL SIGUSR1 FEITA COM SUCESSO
void MSG_CLIENTE_INFO_SINAL_SIGUSR1_CONFIGURACAO_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nCONFIGURAÇÃO DO SINAL SIGUSR1 FEITA COM SUCESSO\n");
}
//MENSAGEM DE INFO - CONFIGURAÇÃO DO SINAL SIGUSR2 FEITA COM SUCESSO
void MSG_CLIENTE_INFO_SINAL_SIGUSR2_CONFIGURACAO_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nCONFIGURAÇÃO DO SINAL SIGUSR2 FEITA COM SUCESSO\n");
}
//MENSAGEM DE INFO - CLIENTE A TERMINAR
void MSG_CLIENTE_INFO_SINAL_INTERRUPCAO_COM_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nCLIENTE A TERMINAR DE FORMA CORRETA\n");
}
//MENSAGEM DE INFO - CLIENTE A TENTAR CONEXAO COM O ARBITRO
void MSG_CLIENTE_INFO_TENTAR_CONEXAO_COM_ARBITRO(int countdownTempoDeEspera){
    fprintf(stderr,"\n***INFO***\nA TENTAR DETETAR O ÁRBITRO - %d SEGUNDOS RESTANTES\n",countdownTempoDeEspera);
}
//MENSAGEM DE INFO - CLIENTE DETETOU O ÁRBITRO COM SUCESSO
void MSG_CLIENTE_INFO_CONEXAO_COM_ARBITRO_SUCESSO(){
    fprintf(stderr,"\n***INFO***\nCLIENTE DETETOU O ÁRBITRO COM SUCESSO\n");
}
//MENSAGEM DE INFO - FIFO DO CLIENTE CRIADO COM SUCESSO
void MSG_CLIENTE_INFO_FIFO_CLIENTE_CRIACAO_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO CLIENTE FOI CRIADO COM SUCESSO\n");
}
//MENSAGEM DE INFO - FIFO DO ARBITRO ABRIR COM SUCESSO
void MSG_CLIENTE_INFO_FIFO_ARBITRO_ABRIR_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO ARBITRO FOI ABERTO PARA ESCRITA COM SUCESSO\n");
}
//MENSAGEM DE INFO - FIFO DO CLIENTE ABRIR COM SUCESSO
void MSG_CLIENTE_INFO_FIFO_CLIENTE_ABRIR_SUCESSO(){
    fprintf(stderr, "\n***INFO***\nO FIFO DO CLIENTE FOI ABERTO PARA LEITURA E ESCRITA COM SUCESSO\n");
}
//MENSAGEM DE INFO - DADOS ENVIADOS
void MSG_CLIENTE_INFO_DADOS_ENVIADOS(pid_t pid_cliente, int validID, char *nomeID, char *msg, int numeroDeBytesEnviados){
    fprintf(stderr, "\n***INFO***\nDADOS ENVIADOS DO CLIENTE PARA O ARBITRO:\n\tPID_CLIENTE:%d\n\tVALIDID:%d\n\tUSERNAME:%s\n\tMENSAGEM:%s\n\t[NUMERO DE BYTES ENVIADOS:%d]\n", pid_cliente, validID, nomeID, msg, numeroDeBytesEnviados);
}
//MENSAGEM DE INFO - USERNAME APROVADO
void MSG_CLIENTE_INFO_USERNAME_APROVADO(char *nomeID){
    fprintf(stderr, "\n***INFO***\nO NOMEID \"%s\" FOI APROVADO! ESSE SERÁ O SEU USERNAME!\n",nomeID);
}
//MENSAGEM DE INFO - USERNAME RECUSADO
void MSG_CLIENTE_INFO_USERNAME_RECUSADO(char *msg){
    fprintf(stderr, "\n***INFO***\nO USERNAME \"%s\"FOI RECUSADO!\n",msg);
}
//MENSAGEM DE INFO - ALGO DE ANORMAL COM OS DADOS RECEBIDOS
void MSG_CLIENTE_INFO_DADOS_RECEBIDOS_ANORMAL(int numeroDeBytesRecebidos){
    fprintf(stderr,"\n***INFO***\nSEM RESPOSTA OU RESPOSTA INCOMPREENSIVEL. [NUMERO DE BYTES RECEBIDOS:%d]\n", numeroDeBytesRecebidos);
}
//MENSAGEM DE INFO - MENSAGEM RECEBIDA DO ARBITRO
void MSG_CLIENTE_INFO_MENSAGEM_ARBITRO_RECEBIDA(char *msg){
    fprintf(stderr, "\n***INFO***\nMENSAGEM DO ÁRBITRO: \"%s\"\n",msg);
}
//MENSAGEM DE INFO - MENSAGEM NO FIM DO CAMPEONATO
void MSG_CLIENTE_INFO_CAMPEONATO_TERMINOU(){
    fprintf(stderr, "\n\n***INFO***\nO CAMPEONATO TERMINOU PARA SI\n\n\nCLIQUE \"ENTER\" PARA PROSSEGUIR\n");
}

// *************** PEDIDO *************** 
//MENSAGEM DE PEDIDO - INTRODUZIR NOMEID
void MSG_CLIENTE_PEDIDO_INTRODUZIR_NOMEID(){
    fprintf(stdout,"\n***PEDIDO***\nESCREVA O SEU NOME DE UTILIZADOR (UNICO): [NUMERO MÁXIMO DE CARATERES:%d]\n",TAM_USERNAME-1);
}

// *************** AJUDA ***************
//MENSAGEM DE AJUDA - FORMATAÇÃO DOS ARGUMENTOS 
void MSG_CLIENTE_AJUDA_ARGUMENTO_FORMATACAO(){
    fprintf(stderr,"\n***AJUDA***\nDeve iniciar o cliente com o formato:\n./cliente\n");
}

void handler_SIGUSR1(){
    MSG_CLIENTE_INFO_SINAL_INTERRUPCAO_COM_SUCESSO();
    if(flag_c_fifo_fname_isAberto==TRUE){
        close(c_fifo_fd);
        flag_c_fifo_fname_isAberto=FALSE;
    }
    if(flag_a_fifo_fd_isAberto==TRUE){
        close(a_fifo_fd);
        flag_a_fifo_fd_isAberto=FALSE;
    }
    if(flag_c_fifo_fname_isCriado==TRUE){
        unlink(c_fifo_fname);
        flag_c_fifo_fname_isCriado=FALSE;
    }
    exit(EXIT_SUCCESS);
}

void handler_SIGUSR2(){
    estouAParticiparNoCampeonatoADecorrerAtualmente=FALSE;
    jogador.validID=FALSE;
    strcpy(jogador.nomeID,"");
    MSG_CLIENTE_INFO_CAMPEONATO_TERMINOU();
}

void handler_alarm_esperaConexaoComArbitro(){
    if(countdownTempoDeEspera!=0){
        MSG_CLIENTE_INFO_TENTAR_CONEXAO_COM_ARBITRO(countdownTempoDeEspera);
        countdownTempoDeEspera--;
        alarm(1);
    }else{
        MSG_CLIENTE_ERRO_CONEXAO_COM_ARBITRO();
        exit(EXIT_ERRO_CONEXAO_COM_ARBITRO);
    }
}

void * testaContinuamenteConexaoComArbitro(){
    while(TRUE){
        if(access(FIFO_ARBITRO, F_OK)!=0){
            alarm(1);
            estouAParticiparNoCampeonatoADecorrerAtualmente=FALSE;
            while(access(FIFO_ARBITRO, F_OK)!=0){} //VAI TENTANDO ACEDER AO ÁRBITRO
            alarm(0);
            countdownTempoDeEspera=TEMPO_DE_ESPERA;
            MSG_CLIENTE_INFO_CONEXAO_COM_ARBITRO_SUCESSO();
        }
    }
}

void enviaMensagemParaOArbitro(){
    if(strcasecmp(jogador.msg,"shutdown")==0){
        kill(getpid(), SIGUSR1);
    }

//ENVIA AO ARBITRO
    numeroDeBytesEnviados=write(a_fifo_fd, &jogador, sizeof(jogador));
    MSG_CLIENTE_INFO_DADOS_ENVIADOS(jogador.pid_cliente, jogador.validID, jogador.nomeID, jogador.msg, numeroDeBytesEnviados);
}

void obtemRespostaDoArbitro(){
    numeroDeBytesRecebidos = read(c_fifo_fd, &arbitro, sizeof(arbitro));
    if(numeroDeBytesRecebidos == sizeof(arbitro)){
        MSG_CLIENTE_INFO_MENSAGEM_ARBITRO_RECEBIDA(arbitro.msg);
    }else{
        MSG_CLIENTE_INFO_DADOS_RECEBIDOS_ANORMAL(numeroDeBytesRecebidos);
    }
}

int main(int argc){
    pthread_t threadTestaConexaoComArbitro, threadComandosOffline;

//VALIDAÇÃO DA QUANTIDADE DE ARGUMENTOS
    if(argc!=1){
        MSG_CLIENTE_ERRO_ARGUMENTO_QUANTIDADE();
        MSG_CLIENTE_AJUDA_ARGUMENTO_FORMATACAO();
        exit(EXIT_ERRO_ARGUMENTO_QUANTIDADE);
    }

//CONFIGURAR O SINAL PARA FECHAR O CLIENTE EM SEGURANÇA
    if(signal(SIGUSR1, handler_SIGUSR1)==SIG_ERR){
        MSG_CLIENTE_ERRO_SINAL_SIGUSR1_CONFIGURACAO();
        exit(EXIT_ERRO_SINAL_SIGUSR1_CONFIGURACAO);
    }else{
        MSG_CLIENTE_INFO_SINAL_SIGUSR1_CONFIGURACAO_SUCESSO();
    }

    if(signal(SIGUSR2, handler_SIGUSR2)==SIG_ERR){
        MSG_CLIENTE_ERRO_SINAL_SIGUSR2_CONFIGURACAO();
        exit(EXIT_ERRO_SINAL_SIGUSR2_CONFIGURACAO);
    }else{
        MSG_CLIENTE_INFO_SINAL_SIGUSR2_CONFIGURACAO_SUCESSO();
    }

    signal(SIGALRM, handler_alarm_esperaConexaoComArbitro);

    if(access(FIFO_ARBITRO, F_OK)!=0){
        alarm(1);
        while(access(FIFO_ARBITRO, F_OK)!=0){} //VAI TENTANDO ACEDER AO ÁRBITRO
        alarm(0);
        countdownTempoDeEspera=TEMPO_DE_ESPERA;
        MSG_CLIENTE_INFO_CONEXAO_COM_ARBITRO_SUCESSO();
    }

//CRIA O FIFO DO CLIENTE
    jogador.pid_cliente = getpid();
    sprintf(c_fifo_fname, FIFO_CLIENTE, jogador.pid_cliente);
    if(mkfifo(c_fifo_fname, 0777) == -1){
        MSG_CLIENTE_ERRO_FIFO_CLIENTE_CRIACAO();
        exit(EXIT_ERRO_FIFO_CLIENTE_CRIACAO);
    }
    flag_c_fifo_fname_isCriado=TRUE;
    MSG_CLIENTE_INFO_FIFO_CLIENTE_CRIACAO_SUCESSO();

//ABRE O FIFO DO ARBITRO PARA ESCRITA 
    a_fifo_fd = open(FIFO_ARBITRO, O_WRONLY);
    if(a_fifo_fd == -1){
        MSG_CLIENTE_ERRO_FIFO_ARBITRO_ABRIR();
        unlink(c_fifo_fname); //ELIMINA O FIFO DO CLIENTE
        flag_c_fifo_fname_isCriado=FALSE;
        exit(EXIT_ERRO_FIFO_ARBITRO_ABRIR);
    }
    flag_a_fifo_fd_isAberto=TRUE;
    MSG_CLIENTE_INFO_FIFO_ARBITRO_ABRIR_SUCESSO();

//ABRE O FIFO DO CLIENTE PARA LEITURA E ESCRITA
    c_fifo_fd = open(c_fifo_fname, O_RDWR);
    if(c_fifo_fd == -1){
        MSG_CLIENTE_ERRO_FIFO_CLIENTE_ABRIR();
        close(a_fifo_fd); //FECHA O FIFO DO ARBITRO
        flag_a_fifo_fd_isAberto=FALSE;
        unlink(c_fifo_fname); //ELIMINA O FIFO DO CLIENTE
        flag_c_fifo_fname_isCriado=FALSE;
        exit(EXIT_ERRO_FIFO_CLIENTE_ABRIR);
    }
    flag_c_fifo_fname_isAberto=TRUE;
    MSG_CLIENTE_INFO_FIFO_CLIENTE_ABRIR_SUCESSO();

    //VERIFICA SE O ARBITRO ESTÁ LIGADO CONTINUAMENTE
    pthread_create(&threadTestaConexaoComArbitro, NULL, testaContinuamenteConexaoComArbitro, NULL);

    while(TRUE){
            do{
            //TENTA ENTRAR COM UM USERNAME
                MSG_CLIENTE_PEDIDO_INTRODUZIR_NOMEID();

                fgets(jogador.msg, TAM_USERNAME, stdin);
                jogador.msg[strlen(jogador.msg)-1]='\0';
                jogador.validID=FALSE;

                enviaMensagemParaOArbitro();                

            //OBTEM A RESPOSTA DO ARBITRO
                obtemRespostaDoArbitro();
                if(numeroDeBytesRecebidos == sizeof(arbitro)){
                    if(atoi(arbitro.msg)==TRUE){
                        jogador.validID=TRUE;
                        strncpy(jogador.nomeID, jogador.msg, TAM_USERNAME);
                        MSG_CLIENTE_INFO_USERNAME_APROVADO(jogador.nomeID);
                    }else{
                        MSG_CLIENTE_INFO_USERNAME_RECUSADO(jogador.msg);
                    }
                }
            }while(jogador.validID==FALSE);

            estouAParticiparNoCampeonatoADecorrerAtualmente=TRUE;

            while(estouAParticiparNoCampeonatoADecorrerAtualmente==TRUE){
                fprintf(stdout,"\n%s@CHAMPION: ", jogador.nomeID);
                fgets(jogador.msg,TAM_MENSAGEM_JOGADOR,stdin);
                jogador.msg[strlen(jogador.msg)-1]='\0';
                if(estouAParticiparNoCampeonatoADecorrerAtualmente==TRUE){
                    enviaMensagemParaOArbitro();
                    obtemRespostaDoArbitro(); 
                }
                   
            }
    }

    
    pthread_join(threadTestaConexaoComArbitro, NULL);

    exit(EXIT_SUCCESS);
}