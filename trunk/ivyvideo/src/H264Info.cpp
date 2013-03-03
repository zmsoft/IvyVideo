#include "H264Info.h"

H264Info::H264Info(const char *data, int len) : mData(data), mLen(len)
{}

// for H264: 0x000001 or 0x00000001
int H264Info::findNALUStartCodeSize(const char *data, int len)
{
    for (int k=2; k < len; k++) {
        if (data[k] == 1 && data[k-1] == 0 && data[k-2] == 0) {
			return k+1;
		}
    }

	return -1;
}

// return 1: ok but the Last NAL unit, 0: ok, -1: error
int H264Info::findNALU(const char *data, int len, int &nalSize)
{
	int startCodeSize = findNALUStartCodeSize(data, len);

	nalSize = 0;
	for(int k=startCodeSize; k < len - 2; k += 2) {
		if (data[k] == 0) {
			if ((data[k+1] == 1 && data[k-1] == 0) ||
				(data[k+2] == 1 && data[k+1] == 0))
			{
				// find a header and resuce size by preceding zeros
				while(data[k-1] == 0) { k--; }
				nalSize = k;
			}
			return 0;
		}
	}

	// the Last NAL unit
	int payloadSize = len - startCodeSize;
	if (payloadSize > 0) {
		return 1;
	}	

	return -1;
}

//
// find the NRI, the NAL unit header(1 bute)
//  -----------------------------
// |  start code  |F|NRI|  TYPE  |
//  -----------------------------
// start code: 0x000001 or 0x00000001
// NRI(2b) - nal_ref_idc:  00  - the NAL unit is not used to reconstruct reference pictures.
//  					  >00  - the NAL unit is required to reconstruct reference picture
//								 in the same layer, or contains a parameter set.
void H264Info::getNRI(const char *data, int len, int &nri)
{
	int startCodeSize = findNALUStartCodeSize(data, len);
	const int type = data[startCodeSize] & 0x1f;
	
	// NALU type of 5, 7, and 8 should have NRI to 11.
	if (type == 5 || type == 7 || type == 8) {
		nri = 0x60;
	}else {
		nri = data[startCodeSize] & 0x60;
	}
}

int H264Info::getNALUType(const char *data, int len, int &type)
{
	int startCodeSize = findNALUStartCodeSize(data, len);
	type = data[startCodeSize] & 0x1f;
	if (type == 0) {
		return -1;
	}

	return 0;
}
