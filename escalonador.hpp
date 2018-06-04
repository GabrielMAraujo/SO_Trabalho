#define QUANTUM 5

#include "Processo.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <vector>
#include <queue>
#include <signal.h>
#include <sys/msg.h>
#include <sstream>

using namespace std;

struct MenorPrioridade{
    bool operator()(const Processo* lhs ,const Processo* rhs) const
    {
        return lhs->prioridade > rhs->prioridade;
    }
};

class escalonador{
  
public:
    vector<Processo*> processosAExecutar;
//    priority_queue<Processo*> filaPrioridade;
    priority_queue<Processo*, vector<Processo*>, MenorPrioridade> filaPrioridade;
    vector<Processo*> processosJaExecutados;
    
    void VerificaDelay();
    void VerificaQuantum();
    void ExecutaDaFila();
    void ObtemMsg();
    
};
