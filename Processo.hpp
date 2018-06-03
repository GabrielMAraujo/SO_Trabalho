#ifndef Processo_hpp
#define Processo_hpp

#include <iostream>
#include <string>
#include <time.h>

#endif /* Processo_hpp */

using namespace std;

typedef struct tempo{
    int horas;
    int minutos;
}Tempo;

class Processo{
  
public:
    Processo(long int _delay, int _prioridade, string _nomeExecutavel, int _job, int _copiasJob);
    void RecalculaPrioridade();
    
    long int delay;
    int prioridade;
    string nomeExecutavel;
    int pid = 0;
    int job;
    Tempo tempoSubmissao;
    Tempo tempoInicio;
    Tempo tempoTermino;
    int copiasJob;
    
private:
    bool diminuindo = true;
    bool esteveNaFila = false;
};
