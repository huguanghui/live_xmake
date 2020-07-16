#include "cache_buff.h"

#define FRAME_MEM_SIZE (1920*1080*2)
#define MAX_QUENE_LENGTH 10

static Quene_t gListData;
static unsigned char availibleNode[MAX_QUENE_LENGTH] = {0};
static Node_t nodeMemStore[MAX_QUENE_LENGTH];
static unsigned char availibleFrame[MAX_QUENE_LENGTH] = {0};
static char frameMemStore[MAX_QUENE_LENGTH][FRAME_MEM_SIZE];

int InitDataList()
{
    if (pthread_mutex_init(&gListData.qlock, NULL) != 0) {
        return -1;
    }
    memset(availibleNode, 0, sizeof(availibleNode));
    memset(nodeMemStore, 0, sizeof(nodeMemStore));
    memset(availibleFrame, 0, sizeof(availibleFrame));
    memset(frameMemStore, 0, sizeof(frameMemStore));
    return 0;
}

static int dataLock()
{
    return pthread_mutex_lock(&gListData.qlock);
}

static int dataUnlock()
{
    return pthread_mutex_unlock(&gListData.qlock);
}

static void *mallocNode()
{
    unsigned char i = 0;
    for (i = 0; i < MAX_QUENE_LENGTH; ++i) {
        if (availibleNode[i] == 0) {
           availibleNode[i] = 1;
           return &nodeMemStore[i];
        }
    }

    return NULL;
}

static void freeNode(pNode_t node)
{
    unsigned char i = 0;    
    for (i = 0; i < MAX_QUENE_LENGTH; ++i) {
        if (&nodeMemStore[i] == node) {
            availibleNode[i] = 0;
        }
    }
}

static void *mallocFrame()
{
    unsigned char i = 0;
    for (i = 0; i < MAX_QUENE_LENGTH; ++i) {
        if (availibleFrame[i] == 0) {
           availibleFrame[i] = 1;
           return &frameMemStore[i];
        }
    }
    return NULL;
}

static void freeFrame(void *frame)
{
    unsigned char i = 0;
    for (i = 0; i < MAX_QUENE_LENGTH; ++i) {
        if (&frameMemStore[i] == frame) {
            availibleFrame[i] = 0;
        }
    }
}

int PutData(void *msg, unsigned int *size)
{
    int ret = -1;
    dataLock();
    if (gListData.q_num < MAX_QUENE_LENGTH) {
        pNode_t node = (pNode_t)mallocNode();
        if (node == NULL) {
            ret = -1;
        }
        else
        {
            node->msg = mallocFrame();
            if (NULL == node->msg) {
                freeNode(node);
                ret = -1;
            }
            else
            {
                int i = gListData.q_num;
                pNode_t n = gListData.q_head;
                memcpy(node->msg, msg, *size);
                node->msgsize = *size;
                node->next = NULL;
                if (i == 0) {
                   gListData.q_head = node; 
                }
                else
                {
                    while(i-1)
                    {
                        n = n->next;
                        i--;
                    }
                    n->next = node;
                }
                gListData.q_num += 1;
                ret = 0; 
            }
        }
    }
    dataUnlock();

    return ret;
}

static int removeDate()
{
    int ret = -1;

    dataLock();
    if (0 == gListData.q_num) {
        ret = -1;
    } else {
        pNode_t n = gListData.q_head;   
        gListData.q_head = gListData.q_head->next;
        gListData.q_num -= 1;
        freeFrame(n->msg);
        n->msgsize = 0;
        freeNode(n);
        ret = 0;
    } 
    dataUnlock();
    return ret;
}

unsigned int GetData(unsigned char *framebuf, unsigned int size)
{
    unsigned int ret = 0;

    dataLock();
    if (gListData.q_num == 0) {
        ret = 0;
        dataUnlock();
        return ret;
    } else {
        if (size < gListData.q_head->msgsize) {
           dataUnlock();
           removeDate();
           return ret;
        }    
        memcpy(framebuf, gListData.q_head->msg, gListData.q_head->msgsize);
        ret = gListData.q_head->msgsize; 
        dataUnlock();
        removeDate();
        return ret;
    }
}

unsigned int  GetNextFrameSize()
{
    unsigned int size = 0;
    dataLock();
    if (gListData.q_num != 0) {
        size = gListData.q_head->msgsize; 
    }
    dataUnlock();

    return size;
}
