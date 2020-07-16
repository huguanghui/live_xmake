#include "BasicUsageEnvironment.hh"
#include "liveRTSPServer.h"
#include "cache_buff.h"

#define FRAME_MEM_SIZE (1920*1080*2)
#define SIZES (2048)
#define FILE_NAME "./test.h264"

// 使用读文件方式来模拟实时帧写入
static void *receive_thread(void *param)
{
    pthread_detach(pthread_self());

    unsigned int size;
    unsigned char buf[FRAME_MEM_SIZE];
    int running = 1;
    int read_len = 0;

    FILE *fd = fopen(FILE_NAME, "rb");
    if (fd) {
        while (running) {
            read_len = fread(buf, 1, SIZES, fd);
            printf("read_len = %d\n", read_len);
            if (read_len > 0) {
f_try:
                if (PutData((char*)buf, (unsigned int*)&read_len) < 0) {
                    usleep(100*1000);
                    printf("retring...\n");
                    goto f_try;
                }
            }
            if (read_len < SIZES || read_len == 0) {
                fseek(fd, 0, SEEK_SET);
            }
        }
        fclose(fd);
    }

    return NULL;
}

static void *done_thread(void *param)
{
    pthread_detach(pthread_self());

    unsigned int get_len;
    unsigned char buf[FRAME_MEM_SIZE];
    int running = 1;

    while (running) {
d_try:
        get_len = GetData(buf, sizeof(buf));
        if (get_len > 0) {
            printf("get_len = %d\n", get_len);
        }
        else {
            printf("GetData Failed!\n");
            usleep(100*1000);
            goto d_try;
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    TaskScheduler* sch = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*sch);

    UserAuthenticationDatabase* authDB = NULL;

    pthread_t tid_receive = 0;
    pthread_create(&tid_receive, NULL, receive_thread, NULL);

    pthread_t tid_done = 0;
    pthread_create(&tid_done, NULL, done_thread, NULL);

    RTSPServer* rtspServer = NULL;
    portNumBits rtspServerPortNum = 554;
    rtspServer = liveRTSPSrever::createNew(*env, rtspServerPortNum, authDB);
    if (NULL == rtspServer) {
       rtspServerPortNum = 8554; 
       rtspServer = liveRTSPSrever::createNew(*env, rtspServerPortNum, authDB);
    }
    if (NULL == rtspServer) {
       *env << "Faile to create RTSP server: " << env->getResultMsg() << "\n";
       exit(1);
    }

    char* urlPrefix = rtspServer->rtspURLPrefix();
    *env << "URL: " << urlPrefix;

    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
        *env << "(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling, or for HTTP live streaming (for indexed Transport Stream files only).)\n";
    } else {
        *env << "(RTSP-over-HTTP tunneling is not available.)\n";
    }

    env->taskScheduler().doEventLoop(); // does not return

    return 0;
}
