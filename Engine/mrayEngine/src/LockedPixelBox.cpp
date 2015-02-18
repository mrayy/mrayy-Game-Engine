#include "stdafx.h"

#include "LockedPixelBox.h"
#include "PixelUtil.h"



namespace mray{
namespace video{

LockedPixelBox LockedPixelBox::getSubBox(const math::box3d&b)const
{
	uint eSize=PixelUtil::getPixelDescription(format).elemSizeB;

	LockedPixelBox res(b,format,0);
	res.data=((uchar*)data)+(uint)(
		((b.MinP.x - box.MinP.x)*eSize)+
		((b.MinP.y - box.MinP.y)*eSize*rowPitch)+
		((b.MinP.z - box.MinP.z)*eSize*slicePitch));
	res.rowPitch=rowPitch;
	res.slicePitch=slicePitch;
	res.format=format;

	return res;
}

}
}
