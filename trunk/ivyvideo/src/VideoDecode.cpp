#include "VideoDecode.h"

CVideoDecode::CVideoDecode()
{
}

CVideoDecode::~CVideoDecode()
{
}

bool CVideoDecode::init()
{
    // init ffmpeg decode
    return true;
}

void CVideoDecode::uninit()
{

}

void CVideoDecode::onPacketData(char *data, int size)
{

}
