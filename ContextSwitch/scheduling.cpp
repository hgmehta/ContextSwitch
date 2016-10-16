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


void userDefinedSignal(int signal){ // user defined signal to switch the thread execution.  

	if (SIGUSR1 == signal){
		pause();        //this will pause the current running thread execution. 
	}
}
void *toDoFunction(void * pv){   //thread will execute this function. 

	intptr_t thread = (intptr_t) pv;

	pause();    //this will block the current running thread and pause for 4 seconds.
                    //because the sleep time for currently running thread is 4 seconds.
	int i = 0;  //initial value of i for each thread is zero.
	while(1)
	{
		printf("Thread %d: count %d\n", (int) thread, i++);
		sleep(1);
	}

//	pthread_exit(NULL);
}

int main(int argc, char ** argv){

	struct sigaction signal_action;
	memset(&signal_action, 0, sizeof(signal_action));   //initialize the signal_action with zeros. 
	signal_action.sa_handler = userDefinedSignal;   //on signal call userDefinedSignal function will be called.

	sigaction(SIGUSR1, &signal_action, NULL);   //creating the signal action on signal call 
	sigaction(SIGUSR2, &signal_action, NULL);

	pthread_t threads[2];   //initializing the thread.
	threads[0] = 0;
	threads[1] = 0;

	intptr_t thread = 0;

	for(thread = 0; thread < 2; thread++){  //creating the thread.

		int isSuccess = pthread_create(&threads[thread], NULL, toDoFunction,(void *) thread);
		if(isSuccess){      //checking the error whether the thread is created or not   

			perror("Error: Problem while creating a thread");
			exit(1);
	    	}
	}


	for(thread = 0;thread<=2; thread++){    //for loop will executes continuously. 

		if(thread == 2){

		thread = 0; 
		}		

		int isSuccess = pthread_kill(threads[thread], SIGUSR2); // resume the current thread.
		if (isSuccess){     //checking whether the function executes successfully or not 

			perror("Error in resuming thread");
			exit(1);
		}

		sleep(5);

		isSuccess = pthread_kill(threads[thread], SIGUSR1); //block the current thread
		if(isSuccess){      //checking whether the function executes successfully or not 

			perror("Error in blocking thread");
			exit(1);
		}

	}
	return 0;
}
