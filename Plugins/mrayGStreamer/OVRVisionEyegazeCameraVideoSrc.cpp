

#include "stdafx.h"
#include "OVRVisionEyegazeCameraVideoSrc.h"



namespace mray
{
namespace video
{

OVRVisionEyegazeCameraVideoSrc::OVRVisionEyegazeCameraVideoSrc()
{
}

OVRVisionEyegazeCameraVideoSrc::~OVRVisionEyegazeCameraVideoSrc()
{
}

void OVRVisionEyegazeCameraVideoSrc::_setFoveatedRectsCount(int count){
	EyegazeCameraVideoSrc::_setFoveatedRectsCount(4);
}

std::string OVRVisionEyegazeCameraVideoSrc::_generateFullString()
{
	std::string videoStr = "";

	videoStr += GetCameraStr(0) + " ! mylistener name=precodec  ! tee name=t_src ";

	math::vector2di frameSize = GetFrameSize(0);
	math::vector2di cropSize = GetEyeCropSize();
	frameSize.y /= 2;

	//camera eyes (left,right)
	videoStr += " t_src. ! queue ! videocrop left=" + core::StringConverter::toString(frameSize.x) + " bottom=" + core::StringConverter::toString(frameSize.y ) + " ! tee name=t_0 ";
	videoStr += " t_src. ! queue ! videocrop right=" + core::StringConverter::toString(frameSize.x) + " bottom=" + core::StringConverter::toString(frameSize.y ) + " ! tee name=t_1 ";
	videoStr += " t_src. ! queue ! videocrop left=" + core::StringConverter::toString(frameSize.x) + " top=" + core::StringConverter::toString(frameSize.y ) + " ! tee name=t_2 ";
	videoStr += " t_src. ! queue ! videocrop right=" + core::StringConverter::toString(frameSize.x) + " top=" + core::StringConverter::toString(frameSize.y ) + " ! tee name=t_3 ";

	//master mixer
	videoStr +=
		"videomixer name=mix_eyes  "
		"sink_0::xpos=0 sink_0::ypos=0  sink_0::zorder=0 sink_0::alpha=1  "
		"sink_1::xpos=0 sink_1::ypos=" + core::StringConverter::toString(cropSize.y) + "  sink_1::zorder=0 sink_1::alpha=1 "
		"sink_2::xpos=0 sink_2::ypos=" + core::StringConverter::toString(cropSize.y * 2) + "  sink_2::zorder=0 sink_2::alpha=1 "
		"sink_3::xpos=0 sink_3::ypos=" + core::StringConverter::toString(cropSize.y * 3) + "  sink_3::zorder=0 sink_3::alpha=1 ";


	for (int i = 0; i < 4; ++i)
	{
		std::string idx = core::StringConverter::toString(i);
		std::string fovStr = _generateFoveatedString(i, frameSize,math::vector2di(1, 2));
		videoStr += fovStr;
		videoStr += " ! mix_eyes.sink_" + idx + " ";
	}

	videoStr += " mix_eyes. ";

	return videoStr;
}






}
}