

#include "StdAfx.h"
#include "TBAppGlobals.h"
#include "Application.h"
#include "XMLTree.h"
#include "XMLWriter.h"


#include <winsock2.h>		// for struct in_addr



namespace mray
{
namespace TBee
{



math::vector2d TBAppGlobals::GetStereoScaleRatio()
{
	switch(stereoMode)
	{
	case ERenderStereoMode::None:
	case ERenderStereoMode::StereoTV:
		return math::vector2d(1,1);
	case ERenderStereoMode::TopDown:
		return math::vector2d(1,0.5);
	case ERenderStereoMode::SideBySide:
		return math::vector2d(0.5,1);
	}
	return math::vector2d(1,1);
}
void TBAppGlobals::Load(const core::string& path)
{

	AppData::Load(path);

}
void TBAppGlobals::Save(const core::string& path)
{
	AppData::Save(path);


}
}
}

