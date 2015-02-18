#include "stdafx.h"

#include "ITerrainBrush.h"


namespace mray
{
namespace scene
{

float ITerrainBrush::ApplyAtVertex(float height,float value,ETerrainModificationMode mode)
{
	float res;
	switch (mode)
	{
	case ETMM_Sub:
		value*=-1;
	case ETMM_Add:
		res=height+value;
		break;
	case ETMM_SetValue:
		res=value;
		break;
	case ETMM_Smooth:
		//requires higher processing
		res=value;
		break;
	case ETMM_Noise:
		res=height+(math::Randomizer::rand01()-0.5)*value;
		break;
	}
	return res;
}


}
}