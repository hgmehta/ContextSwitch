// Queue
#include<sched.h>   // scheduling parameter including library.
#include <pthread.h>
struct thread_attribute
{  

    pthread_t thread_id ;
    void *stack_addr;
    size_t stack_size, guard_size ;
    int policy ;
    
    sched_param param;

};
