*******************************************************
*	Trabalho de Sistemas Operacionais	      *
*						      *
*Professora: Alba Cristina Melo 		      *
*Alunos: Gabriel Mota Araújo - 13/0009130	      *
*	 Alexander André de Souza Vieira - 13/0039853 *
*******************************************************

-Compilador:
	=>Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/usr/include/c++/4.2.1 Apple LLVM version 9.1.0 (clang-902.0.39.1)

-Especificação da implementação

******************
Solicita_execucao:
******************

O programa recebe os argumentos do processo a ser enviado ao processador, faz a verificação dos argumentos, e os armazena na struct msgem a seguir:
struct msgem{
    long msgt;
    mensagem mensagem;
};

Por sua vez, esta struct possui uma outra struct mensagem, que guarda os dados necessários ao escalonador para criar os processos postergados:
struct mensagem{
    int tipo;
    int horas;
    int minutos;
    int copias;
    int prioridade;
    char nomearq[60];  
};

É criada uma fila de mensagens, fazendo sempre a verificação se ela realmente foi criada, e então a mensagem é enviada ao escalonador, retornando então uma mensagem de erro ou de sucesso conforme o resultado do envio.

************
Escalonador:
************

O escalonador funciona da seguinte forma:
=>Todos os processos a serem executados são armazenados em um vetor(vector<Processo*> processosAExecutar).
=>Os dados necessários para executar corretamente o processo são guardados na classe Processo.(Processo.hpp)
class Processo{
  
public:
    ......
    long int delay;
    int prioridade;
    string nomeExecutavel;
    int pid = 0;
    ......
    
private:
    bool diminuindo = true;
    bool esteveNaFila = false;
};

=>O escalonador percorre o vetor processosAExecutar buscando um processo cujo delay já tenha passado, de acordo com a hora dada pela função gettimeofday().
=>Quando o delay do processo passa, o mesmo é inserido na fila de prioridades( priority_queue<Processo*, vector<Processo*>, MenorPrioridade> filaPrioridade), e é apagado da fila de processos a executar.
=>O programa roda o processo que está à frente pelo tempo do quantum(5 segundos). Ao final do tempo do quantum, é enviado um sinal SIGSTOP ao processo e o mesmo é parado, passando-se então a executar o próximo processo da fila de prioridade.
=>O processo que estava em execução retorna à fila de prioridades, seguindo a política de prioridade da descrição do trabalho.
=>Caso o processo acabe, o programa detecta isso por meio da função waitpid() e executa o próximo processo da fila.

******************
Remove_postergado:
******************

São declaradas as structs:
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


O programa recebe como argumento o job do processo que deseja remover. É criada uma struct msgem mensagemEnviada, onde nela armazena-se o tipo da mensagem (remover postergado), o id do job que se deseja remover e o msgt. O job só é removido se ainda não houver começado a sua execução.
Mais uma vez, é criada uma fila de mensagens, fazendo sempre a verificação se ela realmente foi criada, e então a mensagem é enviada ao escalonador, retornando então uma mensagem de erro ou de sucesso conforme o resultado do envio por meio de uma segunda fila de mensagens.

******************
lista_postergado:
******************

Novamente são declaradas as structs:
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

O programa requisita a lista por meio de uma fila de mensagens ao escalonador. O escalonador então recebe esta mensagem e procura por jobs a executar. Com isto, o escalonador gera uma string da lista de jobs a executar(caso existam), e manda essa string ao processo lista_postergado por meio de uma segunda fila de mensagens.


******************
Shutdown_postergado:
******************

Novamente são declaradas as structs:
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

É criada uma struct msgem mensagemEnviada, onde nela armazena-se o tipo da mensagem igual a 2(solicita o shutdown), e o msgt.
Mais uma vez, é criada uma fila de mensagens, fazendo sempre a verificação se ela realmente foi criada, e então a mensagem é enviada ao escalonador, retornando então uma mensagem de erro ou de sucesso conforme o resultado do envio.
Se ainda houverem processos para serem executados, é impresso ao usuário a mensagem dizendo que tais processos não serão executados. Se existir um processo que está sendo executado naquele momento, é impresso ao usuário uma mensagem dizendo que aquele processo não será terminado. E por fim, é impresso ao usuário todos os processos que foram executados pelo escalonador, contendo o pid, o nome do arquivo executável, o tempo de submissão, o tempo de início e o tempo de término.