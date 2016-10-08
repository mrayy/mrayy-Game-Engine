
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   22:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\FractalTypes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	FractalTypes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FractalTypes___
#define ___FractalTypes___

#include "Fractal.h"
#include "Noise2D.h"
#include "Noise3D.h"
#include "SeamlessNoise.h"

namespace mray{
namespace math{

	typedef Fractal<Noise2D,vector2d> Fractal2d;
	typedef Fractal<Noise3D,vector3d> Fractal3d;
	typedef Fractal<SeamlessNoise,vector3d> SeamlessFractal;

}
}


#endif //___FractalTypes___
