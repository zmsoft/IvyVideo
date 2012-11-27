#ifndef _RTPPACKET_H_
#define _RTPPACKET_H_

#include <stdlib.h>

/**
 *  RTP protocol
 */

#define RTP_VERSION         2
#define MAX_RTP_SIZE        1200
#define MAX_NALU_SIZE       1136
#define MAX_RTPHEAD_SIZE    64

typedef struct {
    unsigned int v:2;         /* protocol version */
    unsigned int p:1;         /* padding flag */
    unsigned int x:1;         /* header extension flag */
    unsigned int cc:4;        /* CSRC count */
    unsigned int m:1;         /* marker bit */
    unsigned int pt:7;        /* payload type */
    unsigned int seq:16;      /* sequence number */
    unsigned int ts;               /* timestamp */
    unsigned int ssrc;             /* synchronization source */
    unsigned int csrc[1];          /* optional CSRC list */
} rtp_hdr_t;
#define RTP_HEAD_SIZE   (sizeof(rtp_hdr_t)-4)

class CRtpPacket
{
public:
    CRtpPacket(char *data, int len);
    virtual ~CRtpPacket();

    static bool packSglData(const char *data, int len, char *sglPkt, int &sglLen, int type, int seq, int ts);
    static bool packFuData(const char *data, int len, char *&fuPkts, int *&fuLens, int type, int seq, int ts);

    char *getHead();
    char *getBody();

    int getHeadSize();
    int getBodySize();
    int getSeq();
    int getMask();

    void setSeq();
    void setMask();

private:
    char *mData;
    int mSize;
};

#endif
