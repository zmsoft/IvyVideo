#ifndef _MEDIASESSION_H_
#define _MEDIASESSION_H_

#include "IDummyObject.h"
#include "IvyVideo.h"

extern "C"
{
#include "sgs/connection.h"
#include "sgs/context.h"
#include "sgs/session.h"
#include "sgs/map.h"
}

class CMediaSession : 
    public IDummyObject
{
public:
    CMediaSession(IvyMediaSessionSink *sink);
    virtual ~CMediaSession();

    bool init();
    void uninit();

    int joinSession();
    void leaveSession();
    void sendMessage();

protected:
    bool initSGSClient();
    bool uninitSGSClient();
    static void channel_joined_cb(sgs_connection *conn, sgs_channel *channel);
    static void channel_left_cb(sgs_connection *conn, sgs_channel *channel);
    static void channel_recv_msg_cb(sgs_connection *conn, sgs_channel *channel, const uint8_t *msg, size_t msglen);
    static void disconnected_cb(sgs_connection *conn);
    static void logged_in_cb(sgs_connection *conn, sgs_session *session);
    static void login_failed_cb(sgs_connection *conn, const uint8_t *msg, size_t msglen);
    static void reconnected_cb(sgs_connection *conn);
    static void recv_msg_cb(sgs_connection *conn, const uint8_t *msg, size_t msglen);
    static void register_fd_cb(sgs_connection *conn, int fd, short events);
    static void unregister_fd_cb(sgs_connection *conn, int fd, short events);

private:
    IvyMediaSessionSink *mSink;

    std::string mName;
    std::string mPassword;

    std::string mHost;
    unsigned short mPort;

    sgs_context *mContext;
    sgs_connection *mConnection;
    sgs_connection *mSession;
    sgs_channel *mChannel;

    fd_set mReadSet;
    fd_set mWriteSet;
    fd_set mExceptSet;
};

#endif

