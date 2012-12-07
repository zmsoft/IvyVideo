#include "MediaSession.h"
#include "LogTrace.h"

CMediaSession::CMediaSession(IvyMediaSessionSink *sink) : mSink(sink)
{
}

CMediaSession::~CMediaSession()
{
    uninit();
}

bool CMediaSession::init()
{
    if (!mSink) {
        LOGE("CMediaSession::init(), mSink is null");
        return false;
    }

    if (!initSGSClient()) {
        uninit();
        LOGE("CMediaSession::init(), initSGSClient failed");
        return false;
    }
    
    return true;
}

void CMediaSession::uninit()
{
    mSink = NULL;
    uninitSGSClient();
}

void CMediaSession::setUserProfile(const std::string &name, const std::string &passwd)
{
    mName = name;
    mPassword = passwd;
}

void CMediaSession::setServiceHost(const std::string &host, unsigned short port)
{
    mHost = host;
    mPort = port;
}

int CMediaSession::joinSession()
{
    return 0;
}

void CMediaSession::leaveSession()
{}

void CMediaSession::sendMessage()
{}


/**
 * Process for sgs client
 */

bool CMediaSession::initSGSClient()
{
    LOGI("CMediaSession::initSGSClient(), begin");

    mContext = sgs_ctx_create_ex(mHost.c_str(), mPort, (void *)this, register_fd_cb, unregister_fd_cb);
    if (!mContext) {
        LOGE("CMediaSession::initSGSClient(), failed to sgs_ctx_create");
        return false;
    }

    sgs_ctx_set_channel_joined_cb(mContext, channel_joined_cb);
    sgs_ctx_set_channel_left_cb(mContext, channel_left_cb);
    sgs_ctx_set_channel_recv_msg_cb(mContext, channel_recv_msg_cb);
    sgs_ctx_set_disconnected_cb(mContext, disconnected_cb);
    sgs_ctx_set_logged_in_cb(mContext, logged_in_cb);
    sgs_ctx_set_login_failed_cb(mContext, login_failed_cb);
    sgs_ctx_set_reconnected_cb(mContext, reconnected_cb);
    sgs_ctx_set_recv_msg_cb(mContext, recv_msg_cb);

    mConnection = sgs_connection_create(mContext);
    if (!mConnection) {
        LOGE("CMediaSession::initSGSClient(), failed to sgs_connection_create");
        return false;
    }

    int ret = sgs_connection_login(mConnection, mName.c_str(), mPassword.c_str());
    if (ret != 0) {
        LOGE("CMediaSession::initSGSClient(), failed to sgs_connection_login");
        return false;
    }

    return true;
}

bool CMediaSession::uninitSGSClient()
{
    LOGI("CMediaSession::uninitSGSClient(), begin");

    if (mConnection) {
        sgs_connection_destroy(mConnection);
        mConnection = NULL;
    }

    if (mContext) {
        sgs_ctx_destroy(mContext);
        mContext = NULL;
    }

    return true;
}

void CMediaSession::channel_joined_cb(sgs_connection *conn, sgs_channel *channel)
{}

void CMediaSession::channel_left_cb(sgs_connection *conn, sgs_channel *channel)
{}

void CMediaSession::channel_recv_msg_cb(sgs_connection *conn, sgs_channel *channel, const uint8_t *msg, size_t msglen)
{}

void CMediaSession::disconnected_cb(sgs_connection *conn)
{}

void CMediaSession::logged_in_cb(sgs_connection *conn, sgs_session *session)
{}

void CMediaSession::login_failed_cb(sgs_connection *conn, const uint8_t *msg, size_t msglen)
{}

void CMediaSession::reconnected_cb(sgs_connection *conn)
{}

void CMediaSession::recv_msg_cb(sgs_connection *conn, const uint8_t *msg, size_t msglen)
{}

void CMediaSession::register_fd_cb(sgs_connection *conn, int fd, short events)
{}

void CMediaSession::unregister_fd_cb(sgs_connection *conn, int fd, short events)
{}

