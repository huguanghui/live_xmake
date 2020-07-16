#ifndef __CACHE_BUFF__
#define __CACHE_BUFF__

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NODE{
    void *msg;
    unsigned int msgsize;
    struct NODE *next;
}Node_t, *pNode_t;

typedef struct {
    pthread_mutex_t qlock;
    sem_t sem;
    int q_num;
    pNode_t q_head;
}Quene_t, *pQuene_t;

int InitDataList();

int PutData(void *msg, unsigned int *size);
unsigned int GetData(unsigned char *framebuf, unsigned int size);
unsigned int  GetNextFrameSize();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef __CACHE_BUFF__ */
