

/********************************************************************
	created:	2010/07/17
	created:	17:7:2010   16:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationTypes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AnimationTypes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AnimationTypes_h__
#define AnimationTypes_h__


namespace mray
{
namespace animation
{

	enum EAnimationMode{
		EAM_Cycle,
		EAM_PingPong,
		EAM_Clamp
	};

	enum EInterpolationType{
		EIT_Linear,
		EIT_Spline
	};


}
}


#endif // AnimationTypes_h__
