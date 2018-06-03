#include "Processo.hpp"

Processo::Processo(long int _delay, int _prioridade, string _nomeExecutavel, int _job, int _copiasJob){
    delay = _delay;
    prioridade = _prioridade;
    nomeExecutavel = _nomeExecutavel;
    job = _job;
    copiasJob = _copiasJob;
    
    time_t rawtime;
    struct tm* timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    tempoSubmissao.horas = timeinfo->tm_hour;
    tempoSubmissao.minutos = timeinfo->tm_min;
}

//Recalcular prioridade do processo
void Processo::RecalculaPrioridade(){
    
    if(esteveNaFila){
        esteveNaFila = false;
        
        if(prioridade == 1){
            diminuindo = true;
        }
        
        else if(prioridade == 3){
            diminuindo = false;
        }
        
        //Mudar prioridade de acordo com o padrão estabelecido
        if(diminuindo){
            prioridade++;
        }
        else{
            prioridade--;
        }
    }
    
    else{
        //Manter na mesma prioridade
        esteveNaFila = true;
    }
    
    cout << "Prioridade: " << prioridade << " - Esteve na fila: " << esteveNaFila << " - Diminuindo: " << diminuindo << endl;
    
}

