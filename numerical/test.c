#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int i;
	for(i=0; i<1000; i++){
		if(system("./main no prints")==-1){
			printf("Error in the execution number (%d)", i);
			exit(-1);
		}
			
	}
	exit(0);
}
