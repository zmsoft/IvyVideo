#ifndef _H264INFO_H_
#define _H264INFO_H_

class H264Info
{
public:
	H264Info(const char *data, int len);

	int findNALUStartCodeSize(const char *data, int len);
	int findNALU(const char *data, int len, int &nalSize);
	void getNRI(const char *data, int len, int &nri);
	int getNALUType(const char *data, int len, int &type);

private:
	const char *mData;
	int  mLen;
};

#endif
