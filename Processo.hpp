#ifndef Processo_hpp
#define Processo_hpp

#include <iostream>
#include <string>

#endif /* Processo_hpp */

using namespace std;

class Processo{
  
public:
    Processo(long int _delay, int _prioridade, string _nomeExecutavel, int _job);
    void RecalculaPrioridade();
    
    long int delay;
    int prioridade;
    string nomeExecutavel;
    int pid = 0;
    int job;
    
private:
    bool diminuindo = true;
    bool esteveNaFila = false;
};
