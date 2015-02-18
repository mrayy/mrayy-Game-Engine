#include "stdafx.h"

#include "TheoraCommon.h"


namespace mray{
namespace video{


	TheoraCommon TheoraCommon::instance;

TheoraCommon::TheoraCommon(){
	initalizeTables();
}
TheoraCommon::~TheoraCommon(){
}


void TheoraCommon::initalizeTables(){
	//used to bring the table into the high side (scale up) so we
	//can maintain high precision and not use floats (FIXED POINT)
	int scale = 1L << 13,temp;

	for ( unsigned int i = 0; i < 256; i++ )
	{
		temp = i - 128;

		YTable[i]  = (unsigned int)((1.164 * scale + 0.5) * (i - 16));	//Calc Y component

		RVTable[i] = (unsigned int)((1.596 * scale + 0.5) * temp);		//Calc R component

		GUTable[i] = (unsigned int)((0.391 * scale + 0.5) * temp);		//Calc G u & v components
		GVTable[i] = (unsigned int)((0.813 * scale + 0.5) * temp);

		BUTable[i] = (unsigned int)((2.018 * scale + 0.5) * temp);		//Calc B component
	}
}

}
}


