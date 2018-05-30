all: escalonador

escalonador: escalonador.o Processo.o
	g++ escalonador.o Processo.o -o escalonador

Processo.o: Processo.cpp
	g++ -c Processo.cpp

escalonador.o: escalonador.cpp
	g++ -c escalonador.cpp

clean:
	rm *.o
