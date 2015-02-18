/********************************************************************
	created:	2009/11/13
	created:	13:11:2009   10:51
	filename: 	d:\Project\9_11\Project\PhysicalJointDoubleLimitSoftDesc.h
	file path:	d:\Project\9_11\Project
	file base:	PhysicalJointDoubleLimitSoftDesc
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef __PhysicalJointDoubleLimitSoftDesc__
#define __PhysicalJointDoubleLimitSoftDesc__

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class PhysicalJointDoubleLimitSoftDesc
		{
		public:
			PhysicalJointDoubleLimitSoftDesc(){
				setToDefault();
			}
			virtual void setToDefault(){
				low = 0;
				high = 0;
				restitution = 0;
				spring = 0;
				damping = 0;
			}
		
			float low;
			float high;
			float restitution;
			float spring;
			float damping;
		};
	}
}
#endif // __PhysicalJointDoubleLimitSoftDesc__