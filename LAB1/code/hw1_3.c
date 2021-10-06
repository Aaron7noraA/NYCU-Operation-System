#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void)
{
	char *arg[MAX_LINE/2+1]; /*command line arguments*/
	int should_run = 1; /*flag to determine when to exit program*/
	char buffer[MAX_LINE];


	while(should_run){
		printf("osh>");
		fflush(stdout);

		// get the input
		read(STDIN_FILENO, buffer, MAX_LINE);

		// start to analyze the input
		int i = 0;
		char *ID = strtok(buffer, " ");
		while(ID != NULL){
			arg[i] = ID;
			char *piv = strchr(arg[i], '\n');
			if(piv != NULL){
				*piv = '\0';
			}
			ID = strtok(NULL, " ");
			i++;
		}
		// get the whole command on terminal
		// for(int j=0; j<i; j++){
		// 	printf("%s\n", arg[j]);
		// }
		// the last array need to br NULL
		arg[i] = NULL;

		// check exit command
		if(strcmp(arg[0], "exit") == 0){
			//break;
			return 0;
		}

		// check if need to be run concurrent?
		int background = 0;
		if(strcmp(arg[i-1],"&") == 0){
			arg[i-1] = NULL;
			background = 1;
		}


		// open an new process
		pid_t PID = fork();

		// fork failure
		if(PID < 0){
			printf("fork failed!\n");
			exit(1);
		}

		//child process
		else if(PID == 0){
			if(execvp(arg[0], arg) == -1){
				printf("ERROR: command not found\n");
			}
			exit(1);
		}

		// parent process 
		// if concurrent => do nothing
		else{
			if(!background){
				//wait(NULL);
				waitpid(PID, NULL, 0);
			}
		}
	}

	return 0;
}


