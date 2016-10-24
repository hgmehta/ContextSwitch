/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: harsh
 *
 * Created on 12 October, 2016, 11:47 AM
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <queue>

#include "threadQueueStructure.h"
using namespace std;

threadAttribute t[2];
volatile int running_threads = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

void error(int en, char *msg){  //This function prints error message
    printf("%s",en,msg);
}

static void stack_attributes(pthread_attr_t *attr, char *prefix)    //this function get the stack data of thread
{
    int s;
    size_t stack_size, guard_size;  //stores the stack size and guard size of thread
    void *stack_addr;   //stores the stack address.
    
    s = pthread_attr_getguardsize(attr, &guard_size);   
    if (s != 0)
        error(s, "error: thread attribute getguardsize fail");
    
    printf("%sGuard size of thread ID %d          = %d bytes\n", prefix,pthread_self(), guard_size);
    if(running_threads == 0){
        t[0].nguard_size = guard_size;
    }else{
        t[1].nguard_size = guard_size;
    }
    s = pthread_attr_getstack(attr, &stack_addr, &stack_size);
    if (s != 0)
        error(s, "pthread_attr_getstack");

    printf("%sStack address of thread ID %d       = %p", prefix,pthread_self(), stack_addr);
    if(running_threads == 0){
        t[0].nstack_addr = stack_addr;
        t[0].nstack_size = stack_size;
    }else{
        t[1].nstack_addr = stack_addr;
        t[1].nstack_size = stack_size;
    }
    if (stack_size > 0)
        printf(" (EOS = %p)", (char *) stack_addr + stack_size);
    printf("\n");

    printf("%sStack size of thread ID %d        = 0x%x (%d) bytes\n",
            prefix,pthread_self(), stack_size, stack_size);
}

static void display_thread_attributes(pthread_t thread, char *prefix)
{
    
    int s;
    pthread_attr_t attr;
    pthread_key_t key;
    void *value;
    
    sched_param param;
    int priority;
    int policy;
    int ret;
    /* scheduling parameters of target thread */
    ret = pthread_getschedparam (pthread_self(), &policy, &param);
    /* schedule_priority contains the priority of the thread */
    printf("\tPolicy of thread ID %d             = %s\n",pthread_self(),
            (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
            (policy == SCHED_RR)    ? "SCHED_RR" :
            (policy == SCHED_OTHER) ? "SCHED_OTHER" :
            "???");
    if(running_threads == 0){
        t[0].npolicy = policy;
    }else{
        t[1].npolicy = policy;
    }
    printf("\tPriority of thread ID %d           = %d\n",pthread_self(),param.__sched_priority);
    /* key previously created */
    if(running_threads == 0){
        t[0].npriority = param.__sched_priority;
    }else{
        t[1].npriority = param.__sched_priority;
    }
//    value = pthread_getspecific(key);
    printf("\tThread ID           = %d\n",pthread_self());
    if(running_threads == 0){
        t[0].nthread_id = pthread_self();
    }else{
        t[1].nthread_id = pthread_self();
    }
    s = pthread_getattr_np(thread, &attr);
    if (s != 0)
        error(s, "error: Thread get attribute fail");

    stack_attributes(&attr, prefix);

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        error(s, "error: Thread attribute destroy");
}

void *thread_create1(void *arg){

    pthread_mutex_lock(&running_mutex);
    running_threads = 1;
    pthread_mutex_unlock(&running_mutex);
        while (running_threads > 0){
           sleep(1);
    }
    
    printf("Attributes of created thread1:\n");
    display_thread_attributes(pthread_self(), "\t");
    //exit(EXIT_SUCCESS);         /* Terminate all threads */
}

void *thread_create2(void *arg){

    pthread_mutex_lock(&running_mutex);
    running_threads = 0;
    pthread_mutex_unlock(&running_mutex);
    printf("Attributes of created thread2:\n");
    display_thread_attributes(pthread_self(), "\t");
}

int main()
{
    int s;
    queue <threadAttribute> threadQueue;
    pthread_t thr[2];   //two thread references
    pthread_attr_t *attrp[2];      //contain thread attribute
    attrp[0] = NULL;    //initialize with null value
    attrp[1] = NULL;
    
      pthread_create(&thr[0], attrp[0], thread_create1, NULL);  //creating two threads
      pthread_create(&thr[1], attrp[1], thread_create2, NULL);
    for (int i = 0; i <= 1; i++)
       pthread_join(thr[i], NULL);  //waiting for completing two threads
    
      threadQueue.push(t[0]);   //pushing the thread data in a queue
      threadQueue.push(t[1]);
      
      threadAttribute t1 =  threadQueue.front();
      printf("\npriority %d \nstack size 0x%x \nstack address = %p \nthread id = %d",t1.npriority,t1.nstack_size,t1.nstack_addr,t1.nthread_id);
      printf("\nPolice ID %s", (t1.npolicy == SCHED_FIFO)  ? "SCHED_FIFO" :
            (t1.npolicy == SCHED_RR)    ? "SCHED_RR" :
            (t1.npolicy == SCHED_OTHER) ? "SCHED_OTHER" :
            "???");
      printf("\nGuard size of thread ID          = %d bytes\n", t1.nguard_size);
      
//      printf("\npriority %d \nstack size 0x%x \nstack address = %p \nthread id = %d",t[1].npriority,t[1].nstack_size,t[1].nstack_addr,t[1].nthread_id);
//      printf("\nPolice ID %s", (t[1].npolicy == SCHED_FIFO)  ? "SCHED_FIFO" :
//            (t[1].npolicy == SCHED_RR)    ? "SCHED_RR" :
//            (t[1].npolicy == SCHED_OTHER) ? "SCHED_OTHER" :
//            "???");
//      printf("\nGuard size of thread ID          = %d bytes\n", t[1].nguard_size);
//    pause();    /* Terminates when other thread calls exit() */
}