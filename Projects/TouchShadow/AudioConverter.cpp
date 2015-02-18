

#include "stdafx.h"
#include "AudioConverter.h"



namespace mray
{

void AudioConverter::ConvertSamples(const float* src,int n,uchar* dst,uint targetBps)
{
	uint dstSz=GetTargetSamplesSize(n,targetBps);
	uint B=targetBps>>3;
	int maxVal=2<<(targetBps-2);
	for(int i=0;i<n;++i)
	{
		ulong v=0;
		if(*src<-1)
			v=-maxVal;
		else if(*src>1)
			v=maxVal-1;
		else
			v=floor((*src)*maxVal);

		for(int j=B-1;j>=0;--j)
		{
			dst[j]=v&0xFF;
			v>>=8;
		}
		dst+=B;
	}
}
uint AudioConverter::GetTargetSamplesSize(int srcLen,uint targetBps)
{
	return srcLen*(targetBps>>3);
}

}

