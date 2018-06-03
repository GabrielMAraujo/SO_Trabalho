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
    
    
    int job;

    sscanf(argv[1],"%d",&job);

    msgem mensagemEnviada;

    mensagemEnviada.mensagem.tipo = 3; //tipo da mensagem de remover postergado
    mensagemEnviada.mensagem.horas = job; //O campo horas vai guardar o número do job
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