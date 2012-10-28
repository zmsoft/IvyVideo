#ifndef _FPSCOUNTER_H_
#define _FPSCOUNTER_H_

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
}

class FPSCounter
{
public:

	FPSCounter() 
	{
		name = strdup("");
		SetUpdateInterval(1000);
		Reset();
	};
	~FPSCounter() 
	{
		free(name);
	};

	void SetName(char* str)
	{
		free(name);
		name = strdup(str);
	}
	void SetUpdateInterval(int msec)
	{
		msUpdateInterval = msec;
	}

	void BeforeProcess()
	{
		t1 = av_gettime();
	}

	void AfterProcess()
	{
		t2 = av_gettime();
		msTimeElapsed += t2-t1;
		fps = (float)frameCount * 1000000 / msTimeElapsed;

		if (msTimeElapsed > msUpdateInterval)
		{
			Report();
			Reset();
		}
	}
	void FrameFinished()
	{
		frameCount++;
	}
	void Reset()
	{
		msTimeElapsed = 0;
		frameCount = 0;
		fps = 0;
	}
	
	void Report()
	{
		printf("%s ",name);
		printf("FPS=%.2f,time per frame=%.0f\n",fps,1000/fps);
	}
	
protected:
	int64_t t1,t2,msTimeElapsed,msUpdateInterval,frameCount;
	char* name;
	float fps;
};

#endif // _FPSCOUNTER_H_
