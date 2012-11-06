#include "IvyCommon.h"
#include "IvyVideo.h"
#include "LogTrace.h"
#include "IvyClient.h"


int main(int argc, char **argv)
{
    // step-1
    LOGI("native_init called, begin");
    CIvyClient::inst()->init();

    // step-2
    const char *cstr = "mpeg4";
    LOGI("native_setvideoencodeparams called, codec: %s", cstr);
    CIvyClient::inst()->setVideoEncodeParams(cstr);
    
    // step-3: start self video
    CIvyClient::inst()->startSelfVideo();


    // step-4: loop for encoding from yuv file
    RawFrameFormat frameFormat;
    frameFormat.fmt = 17;
    frameFormat.width = 160;
    frameFormat.height = 90;
    frameFormat.orientation = 0;

    char *data = NULL;
    int len = 0;
    CIvyClient::inst()->onRawFrame((char *)data, len, frameFormat);

    // step-5
    CIvyClient::inst()->stopSelfVideo();
    CIvyClient::inst()->uninit();

    return 0;
}
