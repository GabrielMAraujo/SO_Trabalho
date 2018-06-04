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

struct msgem2{
    long msgt;
    char textoLista[2000];
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

            int idFila2 = 0;
            //Tenta acessar fila de msgs para enviar vetor
            idFila2 = msgget(0x0191, 0x1B0);
            //Loop até conseguir acesso à fila de mensagens
            while(idFila2 <= 0){
                idFila2 = msgget(0x0191, 0x1B0);
            }
            
            cout << "Fila obtida com sucesso" << endl;

            msgem2 mensagemRecebida;

            //Espera receber mensagem
            msgrcv(idFila2, &mensagemRecebida, sizeof(mensagemRecebida), 0, 0);
            //Imprimir mensagem
            cout << mensagemRecebida.textoLista;
            msgctl(idFila2, IPC_RMID, NULL);
        }
    }

    return 0;
}