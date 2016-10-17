/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


void userDefinedSignal(int signal){

	if(SIGUSR1 == signal){
		pause();
	}
	if(SIGUSR2 == signal){
		kill(signal, SIGCONT);
	}
}
void *toDoFunction(void * pv){

	pthread_t thread = (pthread_t) pv;

	pause();
	int i = 0;
	while(1)
	{
		printf("Thread %d: count %d\n", (int) thread, i++);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char ** argv){

	struct sigaction signal_action;
	memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = userDefinedSignal;

	sigaction(SIGUSR1, &signal_action, NULL);
	sigaction(SIGUSR2, &signal_action, NULL);

	pthread_t threads[2];
	threads[0] = 0;
	threads[1] = 0;

	pthread_t thread = 0;

	for(thread = 0; thread < 2; thread++){

		int isSuccess = pthread_create(&threads[thread], NULL, toDoFunction,(void *) thread);
		if(isSuccess){

			perror("Error: Problem while creating a thread");
			exit(1);
	    	}
	}

	sleep(1);
	for(thread = 0;thread<=2; thread++){

		if(thread == 2){

		thread = 0;
		}		

		int isSuccess = pthread_kill(threads[thread], SIGUSR2);
		if (isSuccess){

			perror("Error in resuming thread");
			exit(1);
		}

		sleep(4);

		isSuccess = pthread_kill(threads[thread], SIGUSR1);
		if(isSuccess){

			perror("Error in blocking thread");
			exit(1);
		}

	}
	return 0;
}