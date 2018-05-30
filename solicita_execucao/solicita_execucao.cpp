#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <iostream>

using namespace std;

struct mensagem{
    int tipo;
    int horas;
    int minutos;
    int copias;
    int prioridade;
    char nomearq[60];
};

struct msgem{
    long msgt;
    mensagem mensagem;
};

int main(int argc, char *argv[]) {
    
    
    int hora, minuto, num_cop, num_prior;
    char nom_arq[60];

    sscanf(argv[1],"%d:%d",&hora,&minuto);
    sscanf(argv[2],"%d",&num_cop);
    sscanf(argv[3],"%d",&num_prior);
    sscanf(argv[4],"%s",nom_arq);
    
    //Verifica parametros de execucao
    if(argc < 5 || argc > 5) {
        printf("Numero de argumentos inválido.\n");
        return 0;
    }
    if(!(num_cop = atoi(argv[2]))) {   // número de vezes que o programa vai executar
        printf("Argumento número de cópias inválido.\n");
        return 0;
    }
    if(!(num_prior = atoi(argv[3]))) {    //prioridade inicial do processo
        printf("Argumento prioridade inválido.\n");
        return 0;
    }
    if(strlen(argv[4]) <= 0) {   //nome do arquivo executável
        printf("Argumento nome do arquivo executável inválido.\n");
        return 0;
    }

    msgem mensagemEnviada;

    mensagemEnviada.mensagem.tipo = 0; //tipo da mensagem de solicitar execução
    mensagemEnviada.mensagem.horas =hora; //hora
    mensagemEnviada.mensagem.minutos = minuto; //minuto
    mensagemEnviada.mensagem.copias = num_cop; //quantidade de copias
    mensagemEnviada.mensagem.prioridade = num_prior; //valor da prioridade
    strcpy(mensagemEnviada.mensagem.nomearq,argv[4]); //nome do arquivo executável
    mensagemEnviada.msgt = 1;

    int idfila;

    if ((idfila = msgget(0x0190, IPC_CREAT|0x1B0)) < 0){
        printf("Erro na criação da fila\n");
        exit(1);
    }
    else{
        if(msgsnd(idfila, &mensagemEnviada, sizeof(mensagemEnviada), 0) < 0){
            cout << "Erro ao enviar mensagem." << endl;
        }
        else{
            cout << "Mensagem enviada com sucesso." << endl;
        }
    }

    return 0;
}
