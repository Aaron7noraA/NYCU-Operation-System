#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int counter = 1;
int main(){
	printf("Main Process ID : %d\n\n", getpid());
	int PID1, PID2, PID3, PID4, PID5, PID6;

	PID1 = fork();
	wait(NULL);
	if(PID1 == 0){
		printf("Fork 1. I'm the child %d", getpid() );
		printf(", my parent is %d.\n", getppid());
		PID2 = fork();
		wait(NULL);
		if(PID2 == 0){
			printf("Fork 4. I'm the child %d", getpid() );
			printf(", my parent is %d.\n", getppid());	
			PID3 = fork();
			wait(NULL);
			if(PID3 == 0){
				printf("Fork 5. I'm the child %d", getpid() );
				printf(", my parent is %d.\n", getppid());
			}			
		}
		
	}
	else{
		PID4 = fork();
		wait(NULL);
		if(PID4 == 0){
			printf("Fork 2. I'm the child %d", getpid() );
			printf(", my parent is %d.\n", getppid());	
			PID5 = fork();
			wait(NULL);
			if(PID5 == 0){
				printf("Fork 3. I'm the child %d", getpid() );
				printf(", my parent is %d.\n", getppid());	
			}			
		}
		else{
			PID6 = fork();
			wait(NULL);
			if(PID6 == 0){
				printf("Fork 3. I'm the child %d", getpid() );
				printf(", my parent is %d.\n", getppid());
			}	
		}
	}
	return 0;
}