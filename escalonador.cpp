#include "escalonador.hpp"

timeval tempo;
timeval tempoInicial;
timeval tempoInicialQuantum;
bool executando = false;
int pidFork;
int status;
int idFila = 0;
Processo* processoAtual;
escalonador* esc;
int contJob = 0;

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
    mensagem processosAExecutar[100];
};

struct msgem mensagemRecebida;

void escalonador::VerificaDelay(){
    
    for(int i = (int)processosAExecutar.size() - 1; i >= 0; i--){
        if(((tempo.tv_sec - tempoInicial.tv_sec) >= processosAExecutar[i]->delay) && (tempo.tv_usec >= tempoInicial.tv_usec) ){
            
            //Inserir na fila de prioridade
            filaPrioridade.push(processosAExecutar[i]);
            
            //Apaga processo do vetor
            processosAExecutar.erase(processosAExecutar.begin() + i);
        }
    }
}

void escalonador::VerificaQuantum(){
//    cout << tempo.tv_sec - tempoInicialQuantum << endl;
    
    //Verificação por quantum
    if((tempo.tv_sec - tempoInicialQuantum.tv_sec >= QUANTUM) && (tempo.tv_usec >= tempoInicialQuantum.tv_usec) ){
        cout << "QUANTUM" << endl;
        processoAtual->pid = pidFork;
        kill(pidFork, SIGSTOP);
        executando = false;
        
        //Rodar prioridade do processo
        processoAtual->RecalculaPrioridade();
        
        //Colocar no final da fila com prioridade
        filaPrioridade.push(processoAtual);
    }
    
    //Verificação por final do processo
    if(waitpid(pidFork, &status, WNOHANG) > 0){
        cout << "Fim do job" << endl;
        executando = false;
        esc->processosJaExecutados.push_back(processoAtual);
    }
}

void escalonador::ExecutaDaFila(){
    
    if(!filaPrioridade.empty()){
        
        processoAtual = ((Processo*)filaPrioridade.top());
        
        if(processoAtual->pid == 0){
    
            //Cria cópia do processo
            pidFork = fork();
            executando = true;
            tempoInicialQuantum = tempo;
            if(pidFork == 0){
                execl(((Processo*)filaPrioridade.top())->nomeExecutavel.c_str(), ((Processo*)filaPrioridade.top())->nomeExecutavel.c_str(), (char*) 0);
                
            }
            
            else{
                filaPrioridade.pop();
            }
            
        }
        
        else{
            //Continuar a execução do processo
            pidFork = processoAtual->pid;
            tempoInicialQuantum = tempo;
            kill(pidFork, SIGCONT);
            filaPrioridade.pop();
            executando = true;
        }
        
    }
}

void escalonador::ObtemMsg(){
    //Tenta obter fila de mensagens do solicita_execução
    
    if(idFila <= 0){
        idFila = msgget(0x0190, 0x1B0);
        if(idFila > 0){
            cout << "Obteve fila de mensagens de id " << idFila << endl;
        }
    }
    
    if(msgrcv(idFila, &mensagemRecebida, sizeof(mensagemRecebida), 0, IPC_NOWAIT) < 0){
//        std::cout << "não recebeu msg" << std::endl;
    }
    else{
        cout<<"Recebeu mensagem do tipo " << mensagemRecebida.mensagem.tipo << endl;
        
        if(mensagemRecebida.mensagem.tipo == 0){
            //Mensagem que solicita execução
            long int segundos = (mensagemRecebida.mensagem.horas * 3600) + (mensagemRecebida.mensagem.minutos * 60); //Segundos de delay
            
            
            for(int i = 0; i < mensagemRecebida.mensagem.copias;i++){
                //Cria novo processo na fila
                esc->processosAExecutar.push_back(new Processo(segundos, mensagemRecebida.mensagem.prioridade, mensagemRecebida.mensagem.nomearq, contJob));
            }
            //Incrementa contador de job
            contJob++;
        }
        
        else if(mensagemRecebida.mensagem.tipo == 1){
            //Mensagem que solicita lista de processos postergados
            
            int idFila2;
            
            //Cria fila de msgs para enviar vetor
            idFila2 = msgget(0x0191, IPC_CREAT | 0x1B0);
            if(idFila2 > 0){
                cout << "Criou fila de mensagens de id " << idFila2 << endl;
                
                msgem2 mensagemEnviada;
                
                for(int i = 0; i < processosAExecutar.size(); i++){
                    mensagemEnviada.processosAExecutar[i].horas = processosAExecutar[i]->delay;
                    mensagemEnviada.processosAExecutar[i].prioridade = processosAExecutar[i]->prioridade;
                    strcpy(mensagemEnviada.processosAExecutar[i].nomearq,  processosAExecutar[i]->nomeExecutavel.c_str());
                }
                
                mensagemEnviada.msgt = 1;
                
                while((msgsnd(idFila2, &mensagemEnviada, sizeof(mensagemEnviada), IPC_NOWAIT)) < 0){
//                    cout << "Erro ao enviar mensagem." << endl;
//                    perror("Erro ao enviar mensagem: ");
                }
                cout << "Mensagem enviada com sucesso." << endl;
                
            }else{
                cout << "Não foi possível criar fila de mensagens" << endl;
            }
            
        }
        
        else if(mensagemRecebida.mensagem.tipo == 2){
            //Mensagem que pede shutdown
            cout << "O escalonador será encerrado." << endl;
            
            if(processosAExecutar.size() > 0 && filaPrioridade.size() > 0){
            
                cout << "Os seguintes processos: " << endl;
                
                for(int i = 0; i < processosAExecutar.size(); i++){
                    cout << processosAExecutar[i]->nomeExecutavel << endl;
                }
                
                for(int i = 0; i < filaPrioridade.size(); i++){
                    cout << ((Processo*)filaPrioridade.top())->nomeExecutavel;
                    filaPrioridade.pop();
                }
                cout << "não serão executados." << endl;
                
            }
            
            if(executando){
                cout << "O processo " << processoAtual->nomeExecutavel << " não terminará sua execução." << endl;
            }
            
            if(processosJaExecutados.size() > 0){
                cout << "Processos executados pelo escalonador: " << endl;
                
                cout << "job | arq_exec | hhmm | copias | pri" << endl;
                
                for (int i = 0; i < processosJaExecutados.size(); i++){
                    
                    int horas;
                    int minutos;
                    
                    horas = processosJaExecutados[i]->delay / 3600;
                    minutos = (processosJaExecutados[i]->delay % 3600) / 60;
                    
                    cout << processosJaExecutados[i]->job<< " | " << processosJaExecutados[i]->nomeExecutavel << " | " << horas << ":" << minutos << " | " << " "/*INSERIR NUM COPIAS AQUI*/ << " | " << processosJaExecutados[i]->prioridade << endl;
                }
            }
            
            //Fim do escalonador
            exit(1);
        }
    }
    
}


void dummy(int a){
    
}



int main(){
    
    gettimeofday(&tempo, nullptr);
    tempoInicial = tempo;
    
    esc = new escalonador();

    while(1){
        gettimeofday(&tempo, nullptr);
        esc->ObtemMsg();
        esc->VerificaDelay();
        
        if(!executando){
            esc->ExecutaDaFila();
        }
        else{
            esc->VerificaQuantum();
        }
    }
    
}
