#ifndef _LIVE_RTSP_SERVER_H
#define _LIVE_RTSP_SERVER_H 

#ifndef _RTSP_SERVER_HH
#include "RTSPServer.hh"
#endif

class liveRTSPSrever: public RTSPServer {
public:
    static liveRTSPSrever* createNew(UsageEnvironment& env, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds = 65);

protected:
    liveRTSPSrever(UsageEnvironment& env, int ourSocket, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds);
    virtual ~liveRTSPSrever();

protected:
    virtual ServerMediaSession* lookupServerMediaSession(
            const char* streamName, Boolean isFirstLookupInSession);
};

#endif /* ifndef __LIVE_RTSP_SERVER__ */
