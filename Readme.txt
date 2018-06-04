*******************************************************
*	Trabalho de Sistemas Operacionais	      *
*						      *
*Professora: Alba Cristina Melo 		      *
*Alunos: Gabriel Mota Ara�jo - 13/0009130	      *
*	 Alexander Andr� de Souza Vieira - 13/0039853 *
*******************************************************

-Compilador:
	=>Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/usr/include/c++/4.2.1 Apple LLVM version 9.1.0 (clang-902.0.39.1)

-Especifica��o da implementa��o

******************
Solicita_execucao:
******************

O programa recebe os argumentos do processo a ser enviado ao processador, faz a verifica��o dos argumentos, e os armazena na struct msgem a seguir:
struct msgem{
    long msgt;
    mensagem mensagem;
};

Por sua vez, esta struct possui uma outra struct mensagem, que guarda os dados necess�rios ao escalonador para criar os processos postergados:
struct mensagem{
    int tipo;
    int horas;
    int minutos;
    int copias;
    int prioridade;
    char nomearq[60];  
};

� criada uma fila de mensagens, fazendo sempre a verifica��o se ela realmente foi criada, e ent�o a mensagem � enviada ao escalonador, retornando ent�o uma mensagem de erro ou de sucesso conforme o resultado do envio.

************
Escalonador:
************

O escalonador funciona da seguinte forma:
=>Todos os processos a serem executados s�o armazenados em um vetor(vector<Processo*> processosAExecutar).
=>Os dados necess�rios para executar corretamente o processo s�o guardados na classe Processo.(Processo.hpp)
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

=>O escalonador percorre o vetor processosAExecutar buscando um processo cujo delay j� tenha passado, de acordo com a hora dada pela fun��o gettimeofday().
=>Quando o delay do processo passa, o mesmo � inserido na fila de prioridades( priority_queue<Processo*, vector<Processo*>, MenorPrioridade> filaPrioridade), e � apagado da fila de processos a executar.
=>O programa roda o processo que est� � frente pelo tempo do quantum(5 segundos). Ao final do tempo do quantum, � enviado um sinal SIGSTOP ao processo e o mesmo � parado, passando-se ent�o a executar o pr�ximo processo da fila de prioridade.
=>O processo que estava em execu��o retorna � fila de prioridades, seguindo a pol�tica de prioridade da descri��o do trabalho.
=>Caso o processo acabe, o programa detecta isso por meio da fun��o waitpid() e executa o pr�ximo processo da fila.

******************
Remove_postergado:
******************

S�o declaradas as structs:
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


O programa recebe como argumento o job do processo que deseja remover. � criada uma struct msgem mensagemEnviada, onde nela armazena-se o tipo da mensagem (remover postergado), o id do job que se deseja remover e o msgt. O job s� � removido se ainda n�o houver come�ado a sua execu��o.
Mais uma vez, � criada uma fila de mensagens, fazendo sempre a verifica��o se ela realmente foi criada, e ent�o a mensagem � enviada ao escalonador, retornando ent�o uma mensagem de erro ou de sucesso conforme o resultado do envio por meio de uma segunda fila de mensagens.

******************
lista_postergado:
******************

Novamente s�o declaradas as structs:
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

O programa requisita a lista por meio de uma fila de mensagens ao escalonador. O escalonador ent�o recebe esta mensagem e procura por jobs a executar. Com isto, o escalonador gera uma string da lista de jobs a executar(caso existam), e manda essa string ao processo lista_postergado por meio de uma segunda fila de mensagens.


******************
Shutdown_postergado:
******************

Novamente s�o declaradas as structs:
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

� criada uma struct msgem mensagemEnviada, onde nela armazena-se o tipo da mensagem igual a 2(solicita o shutdown), e o msgt.
Mais uma vez, � criada uma fila de mensagens, fazendo sempre a verifica��o se ela realmente foi criada, e ent�o a mensagem � enviada ao escalonador, retornando ent�o uma mensagem de erro ou de sucesso conforme o resultado do envio.
Se ainda houverem processos para serem executados, � impresso ao usu�rio a mensagem dizendo que tais processos n�o ser�o executados. Se existir um processo que est� sendo executado naquele momento, � impresso ao usu�rio uma mensagem dizendo que aquele processo n�o ser� terminado. E por fim, � impresso ao usu�rio todos os processos que foram executados pelo escalonador, contendo o pid, o nome do arquivo execut�vel, o tempo de submiss�o, o tempo de in�cio e o tempo de t�rmino.