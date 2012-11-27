#include "RtpPacket.h"
#include "IvyCommon.h"

CRtpPacket::CRtpPacket(char *data, int len) : mData(data), mSize(len)
{
}

CRtpPacket::~CRtpPacket()
{
    mSize = 0;
    mData = NULL;
}

bool CRtpPacket::packSglData(const char *data, int len, char *sglPkt, int &sglLen, int type, int seq, int ts)
{
    returnf_if_fail((data != NULL) && (len <= MAX_NALU_SIZE));
    returnf_if_fail((sglPkt != NULL) && (sglLen >= (len + MAX_RTPHEAD_SIZE)));
    returnf_if_fail((seq >= 0) && (seq <= 65535));

    memset(sglPkt, 0, sglLen);

    // process RTP head
    rtp_hdr_t *pHdr = (rtp_hdr_t *)sglPkt;
    pHdr->v = RTP_VERSION;
    pHdr->pt = type;
    pHdr->seq = seq;
    pHdr->ts = ts;

    // process NAL unit
    char *pBody = sglPkt + RTP_HEAD_SIZE;
    pBody[0] = 0x0005;
    memcpy(pBody+1, data, len);
    sglLen = RTP_HEAD_SIZE + 1 + len;

    return true;
}

bool CRtpPacket::packFuData(const char *data, int len, char *&fuPkt, int *&fuLen, int type, int seq, int ts)
{
    returnf_if_fail((data != NULL) && (len <= MAX_NALU_SIZE));
    returnf_if_fail((fuPkt[0] != NULL) && (fuPkt[1] != NULL));
    returnf_if_fail(fuLen[0] >= (len + MAX_RTPHEAD_SIZE));
    returnf_if_fail(fuLen[1] >= (len + MAX_RTPHEAD_SIZE));
    returnf_if_fail((seq >= 0) && (seq <= 65535));

    return false;
}

char *CRtpPacket::getHead()
{
    returnv_if_fail(mData != NULL, NULL);
    return mData;
}

char *CRtpPacket::getBody()
{
    returnv_if_fail(mData != NULL, NULL);
    return mData + getHeadSize();
}

int CRtpPacket::getHeadSize()
{
    returnv_if_fail(mData != NULL, -1);
    return 0;
}

int CRtpPacket::getBodySize()
{
    returnv_if_fail(mData != NULL, -1);
    return 0;
}

int CRtpPacket::getSeq()
{
    returnv_if_fail(mData != NULL, -1);
    return 0;
}

int CRtpPacket::getMask()
{
    returnv_if_fail(mData != NULL, -1);
    return 0;
}

void CRtpPacket::setSeq()
{
}

void CRtpPacket::setMask()
{
}

