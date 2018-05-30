#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int main(){
	
	printf("Processo teste\n");
    
    for(int i = 0; i < 40; i++){
        sleep(1);
        printf("rodou por %d segundos\n", i+1);
    }
    
}
