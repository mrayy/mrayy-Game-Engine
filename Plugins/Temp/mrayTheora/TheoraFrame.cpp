#include "stdafx.h"


#include "TheoraFrame.h"
#include "TheoraVideo.h"
#include "TheoraCommon.h"
#include <theora/theoradec.h>


namespace mray{
namespace video{

#define MAX( a, b ) ((a > b) ? a : b)
#define MIN( a, b ) ((a < b) ? a : b)
#define CLIP_RGB_COLOR( c ) MAX( MIN(c, 255), 0 )

	class TheoraFrameHelper
	{
	public:

		static void decodeRGB(TheoraFrame*frame,th_ycbcr_buffer yuv);
		static void decodeGrey(TheoraFrame*frame,th_ycbcr_buffer yuv);
		static void decodeYUV(TheoraFrame*frame,th_ycbcr_buffer yuv);

		static void decodeFrame(TheoraFrame*frame,th_ycbcr_buffer yuv);
	};


	void TheoraFrameHelper::decodeFrame(TheoraFrame*frame,th_ycbcr_buffer yuv){
		switch(frame->m_owner->getOutputType()){
		case ETO_RGB:decodeRGB(frame,yuv);break;
		case ETO_GREY:decodeGrey(frame,yuv);break;
		case ETO_YUV:decodeYUV(frame,yuv);break;
		}
		frame->ready=true;
	}



	void TheoraFrameHelper::decodeRGB(TheoraFrame*frame,th_ycbcr_buffer yuv){
		int t,y;
		uchar *ySrc=yuv[0].data, *ySrc2=yuv[0].data,*ySrcEnd,
			*uSrc=yuv[1].data, *uSrc2=yuv[1].data,
			*vSrc=yuv[2].data, *vSrc2=yuv[2].data;
		uint *out=(uint*) frame->m_buffer;
		int r, g, b, cu, cv, bU, gUV, rV, rgbY;

		const math::vector2di& frameSize=frame->m_owner->GetFrameSize();
		const math::vector2di& theoraSize=frame->m_owner->GetTheoraFrameSize();

		int slice=(frameSize.x+(frameSize.x-theoraSize.x));
		out+=slice*(frameSize.y-1);

		TheoraCommon*cmn=&TheoraCommon::instance;

		uint *pOut;

		for (y=0;y<yuv[0].height;y++)
		{
			t=0; ySrc=ySrc2; ySrcEnd=ySrc2+yuv[0].width; uSrc=uSrc2; vSrc=vSrc2;
			pOut=out;
			while (ySrc != ySrcEnd)
			{
				//get corresponding lookup values
				rgbY = cmn->YTable[*ySrc];
				if ((t=!t) == 1)
				{
					cu=*uSrc; cv=*vSrc;
					rV   = cmn->RVTable[cv];
					gUV  = cmn->GUTable[cu] + cmn->GVTable[cv];
					bU   = cmn->BUTable[cu];
					uSrc++; vSrc++;
				}
				//scale down - brings are values back into the 8 bits of a byte
				r = CLIP_RGB_COLOR((rgbY + rV ) >> 13);
				g = CLIP_RGB_COLOR((rgbY - gUV) >> 13);
				b = CLIP_RGB_COLOR((rgbY + bU ) >> 13);
				*pOut=(((r << 8) | g) << 8) | b;
				pOut++;
				ySrc++;
			}
// 			if (frameSize.x-frame->theoraSize.x)
// 			{
// 				pOut+=frameSize.x-theoraSize.x;
// 			}

			out-=slice;
			ySrc2+=yuv[0].stride;
			if (y%2 == 1) { uSrc2+=yuv[1].stride;; vSrc2+=yuv[2].stride; }
		}
	}
	void TheoraFrameHelper::decodeGrey(TheoraFrame*frame,th_ycbcr_buffer yuv){
		uchar*ySrc=yuv[0].data,*ySrc2=yuv[0].data,*ySrcEnd;
		uint cy;
		uchar* out=frame->m_buffer;
		int y;

		const math::vector2di& frameSize=frame->m_owner->GetFrameSize();
		const math::vector2di& theoraSize=frame->m_owner->GetTheoraFrameSize();

		int slice=(frameSize.x+(frameSize.x-theoraSize.x))*4;
		out+=slice*(frameSize.y-1);

		uchar *pOut;

		for (y=0;y<yuv[0].height;y++)
		{
			ySrc=ySrc2; ySrcEnd=ySrc2+yuv[0].width;
			pOut=out;
			while (ySrc != ySrcEnd)
			{
				cy=*ySrc;
				pOut[0]=pOut[1]=pOut[2]=cy;
				pOut+=4;
				ySrc++;
			}
// 			if (frame->m_owner->GetFrameSize().x-frame->m_owner->GetTheoraFrameSize().x)
// 			{
// 				out+=(frame->m_owner->GetFrameSize().x-frame->m_owner->GetTheoraFrameSize().x)*4;
// 			}
			out-=slice;
			ySrc2+=yuv[0].stride;
		}
	}
	void TheoraFrameHelper::decodeYUV(TheoraFrame*frame,th_ycbcr_buffer yuv){
		int t,y;
		uchar *ySrc=yuv[0].data, *ySrc2=yuv[0].data,*ySrcEnd,
			*uSrc=yuv[1].data, *uSrc2=yuv[1].data,
			*vSrc=yuv[2].data, *vSrc2=yuv[2].data;
		uint *out=(uint*) frame->m_buffer;

		const math::vector2di& frameSize=frame->m_owner->GetFrameSize();
		const math::vector2di& theoraSize=frame->m_owner->GetTheoraFrameSize();

		int slice=(frameSize.x+(frameSize.x-theoraSize.x));
		out+=slice*(frameSize.y-1);

		uint *pOut;

		for (y=0;y<yuv[0].height;y++)
		{
			t=0; ySrc=ySrc2; ySrcEnd=ySrc2+yuv[0].width; uSrc=uSrc2; vSrc=vSrc2;
			pOut=out;
			while (ySrc != ySrcEnd)
			{
				*pOut=(((*ySrc << 8) | *uSrc) << 8) | *vSrc;
				pOut++;
				ySrc++;
				if (t=!t == 1) { uSrc++; vSrc++; }
			}
// 			if (frame->m_owner->GetFrameSize().x-frame->m_owner->GetTheoraFrameSize().x)
// 			{
// 				out+=frame->m_owner->GetFrameSize().x-frame->m_owner->GetTheoraFrameSize().x;
// 			}
			out-=slice;
			ySrc2+=yuv[0].stride;
			if (y%2 == 1) { uSrc2+=yuv[1].stride;; vSrc2+=yuv[2].stride; }
		}
	}

TheoraFrame::TheoraFrame(TheoraVideo* owner){
	m_owner=owner;
	ready=inUse=false;
	timeToDisplay=0;

	m_buffer=MRAY_new uchar[m_owner->GetFrameSize().x*m_owner->GetTheoraFrameSize().y*4];
}
TheoraFrame::~TheoraFrame(){
	if(m_buffer)
		delete [] m_buffer;
}

//! do not call directly, this function is used to reset back color after video clip output mode change
void TheoraFrame::fillBackColor(uint c){
	uint* p=(uint*)m_buffer;
	int x=0;
	for(int y=0;y<m_owner->GetTheoraFrameSize().y;++y){
		//p+=m_owner->getFrameSize().x;
		for(x=0;x<m_owner->GetFrameSize().x;++x){
			*p=c;
			p++;
		}
	}
}

uchar* TheoraFrame::getBuffer(){
	return m_buffer;
}
void TheoraFrame::decodeFrame(void* data)
{
	TheoraFrameHelper::decodeFrame(this,*(th_ycbcr_buffer*)data);
}


}
}


