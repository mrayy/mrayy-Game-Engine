

/********************************************************************
	created:	2013/06/19
	created:	19:6:2013   21:37
	filename: 	C:\Development\IVRC\User Tracking\MathHelp.h
	file path:	C:\Development\IVRC\User Tracking
	file base:	MathHelp
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __MathHelp__
#define __MathHelp__




namespace mray
{

	const core::string JointNames[]=
	{
		"HEAD",
		"NECK",

		"LEFT_SHOULDER",
		"RIGHT_SHOULDER",
		"LEFT_ELBOW",
		"RIGHT_ELBOW",
		"LEFT_HAND",
		"RIGHT_HAND",

		"TORSO",

		"LEFT_HIP",
		"RIGHT_HIP",
		"LEFT_KNEE",
		"RIGHT_KNEE",
		"LEFT_FOOT",
		"RIGHT_FOOT"
	};
#define COLORS_COUNT 6
	class AppCommon
	{
	public:
		static video::SColor Colors[COLORS_COUNT];

	};
	inline math::vector3d ToVectro3d(const NitePoint3f&pt)
	{
		return math::vector3d(pt.x,pt.y,pt.z);
	}

}


#endif
