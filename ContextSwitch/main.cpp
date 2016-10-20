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
#include "threadattribute_queue.cpp"

using namespace std;

volatile int running_threads = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
static void error(int en, char* msg){
    printf("%d : %s",en,msg);
}

static void stack_attributes(pthread_attr_t *attr, char *prefix,void *arg)
{   
    thread_attribute q = * (thread_attribute *) arg;
    
    int s;
    size_t stack_size, guard_size;
    void *stack_addr;

    s = pthread_attr_getguardsize(attr, &guard_size);
    if (s != 0)
        error(s, "error: thread attribute getguardsize fail");

    printf("%sGuard size of thread ID %d          = %d bytes\n", prefix,pthread_self(), guard_size);
    
    q.guard_size=guard_size;
    
    s = pthread_attr_getstack(attr, &stack_addr, &stack_size);
    if (s != 0)
        error(s, "pthread_attr_getstack");

    printf("%sStack address of thread ID %d       = %p", prefix,pthread_self(), stack_addr);

    q.stack_addr=stack_addr;
    
    if (stack_size > 0)
        printf(" (EOS = %p)", (char *) stack_addr + stack_size);
    printf("\n");

    printf("%sStack size of thread ID %d        = 0x%x (%d) bytes\n",
            prefix,pthread_self(), stack_size, stack_size);
    
    q.stack_size=(int)stack_size;
}

static void display_thread_attributes(pthread_t thread, char *prefix,void *arg )
{
    thread_attribute q = * (thread_attribute *) arg;
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
    printf("\tPolicy of thread ID %d             = %d\n",pthread_self(),policy);
    printf("\tPriority of thread ID %d           = %d\n",pthread_self(),param.__sched_priority);
    
    // Initializing parameters of object
    q.policy = policy ;
    q.thread_id = pthread_self();
          printf("AA Thread ID %d",q.thread_id);
    q.param.__sched_priority=param.__sched_priority;
    
    /* key previously created */
    value = pthread_getspecific(key);
    printf("\tThread ID           = %d\n",pthread_self());
    s = pthread_getattr_np(thread, &attr);
    if (s != 0)
        error(s, "error: Thread get attribute fail");

    stack_attributes(&attr, prefix,&q);

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        error(s, "error: Thread attribute destroy");
}

void *thread_create1(void *arg){
     thread_attribute q = * (thread_attribute *) arg;
//    pthread_mutex_lock(&running_mutex);
    running_threads = 1;
//    pthread_mutex_unlock(&running_mutex);
        while (running_threads > 0){
           sleep(1);
    }
    
    printf("Attributes of created thread1:\n");
    display_thread_attributes(pthread_self(), "\t",&q);
 //   exit(EXIT_SUCCESS);         /* Terminate all threads */
}

void *thread_create2(void *arg){

    thread_attribute q = * (thread_attribute *) arg;
//    pthread_mutex_lock(&running_mutex);
    running_threads = 0;
//    pthread_mutex_unlock(&running_mutex);
    printf("Attributes of created thread2:\n");
    display_thread_attributes(pthread_self(), "\t",&q);
}

int main()
{
    int s;
    queue<thread_attribute> thread_queue ;
    pthread_t thr[2];
    pthread_attr_t attr;
    pthread_attr_t *attrp[2];
    attrp[0] = NULL;
    attrp[1] = NULL;
    // Creating queue datatype thread attribute.
    struct thread_attribute thread_a[2] ;
      pthread_create(&thr[0], attrp[0], thread_create1,&thread_a[0]);
      
      pthread_create(&thr[1], attrp[1], thread_create2,&thread_a[1]);
    for (int i = 1; i >= 0; i--)
       pthread_join(thr[i], NULL);

//    pause();    /* Terminates when other thread calls exit() */
      printf("Thread Id %d",thread_a[1].thread_id);
      thread_queue.push(thread_a[0]);
      thread_queue.push(thread_a[1]);

}