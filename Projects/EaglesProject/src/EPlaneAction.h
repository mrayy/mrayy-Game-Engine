
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   0:08
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\EPlaneAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	EPlaneAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EPlaneAction___
#define ___EPlaneAction___


namespace mray{
namespace gameMod{

	enum EPlaneAction{
		EPA_PitchUp,
		EPA_PitchDown,
		EPA_YawLeft,
		EPA_YawRight,
		EPA_RollLeft,
		EPA_RollRight,

		EPA_Turbo,
		EPA_Brake,

		EPA_Missile,

		EPA_NONE,
		EPA_ActionsCount
	};
}
}


#endif //___EPlaneAction___