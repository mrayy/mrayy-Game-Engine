#include "stdafx.h"

#include "IMovable.h"
#include "IAnimationMixer.h"


namespace mray{
namespace animation{

	
	IAnimationMixer::~IAnimationMixer()
	{
	}
	uint IAnimationMixer::calcSizeInternal(){
		return sizeof(IAnimationMixer);
	}

}
}