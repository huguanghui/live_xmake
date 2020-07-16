#include "liveRTSPServer.h"
#include "liveMedia.hh"
#include <string.h>

liveRTSPSrever* liveRTSPSrever::createNew(UsageEnvironment& env, Port ourPort, 
        UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
{
    int ourSocket = setUpOurSocket(env, ourPort);
    if (ourSocket == -1) {
        return NULL;
    }

    return new liveRTSPSrever(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds);
}

liveRTSPSrever::liveRTSPSrever(UsageEnvironment& env, int ourSocket, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
 : RTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds)
{

}

liveRTSPSrever::~liveRTSPSrever() 
{
}

static char *stream_supports[] = {"stream00", "stream01", "stream02"};

ServerMediaSession* liveRTSPSrever::lookupServerMediaSession(char const* streamName, Boolean isFirstLookupInSession)
{
    int i = 0;

    for (i = 0; i < 3; ++i) {
        if (0 == strcmp(streamName, stream_supports[i])) {
           break; 
        }     
    }

    if (3 == i) {
        printf("Unknown streamName: %s\n", streamName);
        return NULL;
    }

    ServerMediaSession* sms = RTSPServer::lookupServerMediaSession(streamName);
    Boolean const smsExists = sms != NULL;
    // exist & isFirst - remove
    if (smsExists && isFirstLookupInSession) {
        removeServerMediaSession(sms); 
        sms = NULL;
    }

    printf("streamName[%s], isFirstLookupInSession[%d] sms[%p]\n", streamName, isFirstLookupInSession, sms);
    // not exist - create
    if (NULL == sms)
    {
        char const* descStr = "stream, by live555 Media Server";
        sms = ServerMediaSession::createNew(envir(), streamName, streamName, descStr);
        sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(envir(), "./test.h264", False));
        addServerMediaSession(sms);
    }

    return sms;
}
