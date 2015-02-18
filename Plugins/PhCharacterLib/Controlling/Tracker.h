/********************************************************************
	created:	2010/07/15
	created:	15:7:2010   8:03
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\Tracker.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	Tracker
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Tracker_h__
#define Tracker_h__
#include "../PhysicalModel/Segment.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class Tracker
		{
		public:
			Tracker():segment(0)
			{}
			Segment* segment;
			float ks,kd;
		};
	}
}
#endif // Tracker_h__