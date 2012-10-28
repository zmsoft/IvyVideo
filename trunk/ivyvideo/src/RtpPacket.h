#ifndef _RTPPACKET_H_
#define _RTPPACKET_H_

class CRtpPacket
{
public:
	char *getRtpHeader();
	char *getRtpBody();
		
	int getHeaderSize();
	int getBodySize();
	unsigned short getRtpSeq();
	int getRtpMask();
	
	void setRtpSeq();
	void setRtpMask();
	bool setRtpBody(char *data);
	
private:
	char *mData;
	int mSize;
};

#endif
