
/********************************************************************
	created:	2009/08/12
	created:	12:8:2009   8:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LockedPixelBox.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LockedPixelBox
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LockedPixelBox___
#define ___LockedPixelBox___


#include "videoCommon.h"


namespace mray{
namespace video{

class MRAY_DLL LockedPixelBox
{
private:
protected:
public:

	void* data;
	math::box3d box;
	EPixelFormat format;

	uint rowPitch;
	uint slicePitch;


	LockedPixelBox(){

	}

	LockedPixelBox(const math::box3d&lockBox,EPixelFormat pFormat,void*pdata):
	box(lockBox),format(pFormat),data(pdata)
	{
		setConsecutive();
	}
	LockedPixelBox(uint w,uint h,uint d,EPixelFormat pFormat,void*pdata):
	box(0,0,0,w,h,d),format(pFormat),data(pdata)
	{
		setConsecutive();
	}

	virtual~LockedPixelBox()
	{
	}

	void setConsecutive()
	{
		rowPitch=box.getWidth();
		slicePitch=rowPitch*box.getHeight();
	}

	bool isConsecutive()const
	{
		return rowPitch==box.getWidth() && slicePitch==box.getHeight()*box.getHeight();
	}

	uint getRowPitchSkip()const
	{
		return rowPitch-box.getWidth();
	}
	uint getSlicePitchSkip()const
	{
		return slicePitch-rowPitch*box.getHeight();
	}

	LockedPixelBox getSubBox(const math::box3d&b)const;
};

}
}

#endif //___LockedPixelBox___
