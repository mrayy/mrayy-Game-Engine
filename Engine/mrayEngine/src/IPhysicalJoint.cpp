


#include "stdafx.h"
#include "IPhysicalJoint.h"


namespace mray{
namespace physics{

	IPhysicalJoint3DDesc* IPhysicalJoint::ConvertToDesc()
	{
		IPhysicalJoint3DDesc*d=IPhysicalJoint3DDesc::CreateJoint(getType());
		if(d)
		{
			if(!SaveToDesc(d))
			{
				delete d;
				return 0;
			}
		}
		return d;
	}
	

}
}

