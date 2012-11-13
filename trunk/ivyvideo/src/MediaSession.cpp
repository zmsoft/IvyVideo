#include "MediaSession.h"

CMediaSession::CMediaSession(IvyMediaSessionSink *sink) : mSink(sink)
{
}

CMediaSession::~CMediaSession()
{
    uninit();
    mSink = NULL;
}

bool CMediaSession::init()
{
    if (!mSink) {
        return false;
    }

    mContext = sgs_ctx_create(mHost.c_str(), mPort, register_fd_cb, unregister_fd_cb);
    if (!mContext) {
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
        return false;
    }

    int ret = sgs_connection_login(mConnection, mName.c_str(), mPassword.c_str());
    if (ret != 0) {
        return false;
    }

    return true;
}

void CMediaSession::uninit()
{
    if (mConnection) {
        sgs_connection_destroy(mConnection);
        mConnection = NULL;
    }

    if (mContext) {
        sgs_ctx_destroy(mContext);
        mContext = NULL;
    }
}

int CMediaSession::joinSession()
{
    return 0;
}

void CMediaSession::leaveSession()
{}

void CMediaSession::sendMessage()
{}

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

