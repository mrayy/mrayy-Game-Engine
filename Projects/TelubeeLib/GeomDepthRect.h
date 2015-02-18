

/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   1:03
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\GeomDepthRect.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	GeomDepthRect
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GeomDepthRect__
#define __GeomDepthRect__


#include "DepthFrame.h"

#include "IStream.h"

namespace mray
{
namespace TBee
{

class GeomDepthRect
{
protected:

	DepthFrame m_frame;

	math::rectf m_rect;

public:
	GeomDepthRect();
	virtual~GeomDepthRect();

	const DepthFrame* GetFrame()const { return &m_frame; }

	const math::rectf& GetRect()const { return m_rect; }
	void SetFrame(DepthFrame* src, math::rectf& rc);

	int WriteToStream(OS::IStream*stream);
	int ReadFromStream(OS::IStream*stream);
};

}
}


#endif
