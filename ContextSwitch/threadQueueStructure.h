/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threadQueueStructure.h
 * Author: harsh
 *
 * Created on 24 October, 2016, 1:32 PM
 */

#ifndef THREADQUEUESTRUCTURE_H
#define THREADQUEUESTRUCTURE_H

#include <stdlib.h>
struct threadAttribute
{
    pthread_t nthread_id;
    int npriority;
    int npolicy;
    size_t nstack_size, nguard_size;
    void *nstack_addr;
};


#endif /* THREADQUEUESTRUCTURE_H */

