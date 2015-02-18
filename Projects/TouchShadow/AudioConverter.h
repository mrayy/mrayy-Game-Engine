

/********************************************************************
	created:	2013/09/10
	created:	10:9:2013   12:22
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\AudioConverter.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	AudioConverter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AudioConverter__
#define __AudioConverter__




namespace mray
{

class AudioConverter
{
protected:
public:

	static void ConvertSamples(const float* src,int n,uchar* dst,uint targetBps);
	static uint GetTargetSamplesSize(int srcLen,uint targetBps);
};

}


#endif
