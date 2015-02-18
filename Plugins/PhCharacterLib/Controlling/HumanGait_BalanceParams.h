/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   10:37
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\HumanGait_BalanceParams.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	HumanGait_BalanceParams
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef HumanGait_BalanceParams_h__
#define HumanGait_BalanceParams_h__
#include "BalanceParams.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class HumanGait_BalanceParams:public BalanceParams
		{
		public:
			float cd,cv;
			bool leftStance;
		};
	}
}
#endif // HumanGait_BalanceParams_h__