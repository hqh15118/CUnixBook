#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXNITEMS 1000000
#define MAXNTHREADS 10

int nitems;

struct {
    pthread_mutex_t mutex;
    int             buff[MAXNITEMS];
    int             nput;
    int             nval;
} shared = {
        PTHREAD_MUTEX_INITIALIZER
};

//define 2 method -- produce and consume
//thread run method
void *produce(void *) , *consume(void *);

int main(int argc, char **argv) {
    int i , nthreads , count[MAXNTHREADS];
    //create 10 producer and 1 consume thread
    pthread_t  tid_produce[MAXNTHREADS] , t_id_consume;
    nthreads = MAXNTHREADS;
    nitems = MAXNITEMS;
    //create thread
    for (i = 0; i < nthreads; ++i) {
        count[i] = 0;
        pthread_create(&tid_produce[i],NULL,produce,&count[i]);
    }
    //wait for all the producer threads created
    for (i = 0; i < nthreads; ++i) {
        pthread_join(tid_produce[i] , NULL);
        printf("count[%d] = %d\n" , i, count[i]);
    }
    //start , then wait for the consumer thread
    pthread_create(&t_id_consume,NULL,consume,NULL);
    pthread_join(t_id_consume,NULL);

    exit(0);
}

void * produce(void *arg){
    for (;;){
        pthread_mutex_lock(&shared.mutex);
        if (shared.nput >= nitems){
            pthread_mutex_unlock(&shared.mutex);
            return(NULL);
        }
        shared.buff[shared.nput] = shared.nval;
        shared.nput ++;
        shared.nval ++;
        pthread_mutex_unlock(&shared.mutex);
        *((int *)arg) += 1;
    }
}

void *consume(void *arg){
    int i;
    for (i = 0; i < nitems; ++i) {
        if (shared.buff[i] != i){
            printf("buff[%d] = %d\n" , i , shared.buff[i]);
        }
    }
    return NULL;
}
