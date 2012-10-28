#include "VideoEncode.h"

CVideoEncode::CVideoEncode()
{
}

CVideoEncode::~CVideoEncode()
{
}

bool CVideoEncode::init()
{
	// init ffmpeg encode
	mProfile = FFMpegEncoder::createDefaultProfile();
	
	return true;
}

void CVideoEncode::uninit()
{
	
}

void CVideoEncode::onRawFrame(char *data, int size, int width, int height, int format)
{

}

