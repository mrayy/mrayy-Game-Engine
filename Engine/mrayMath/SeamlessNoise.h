
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   21:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\SeamlessNoise.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	SeamlessNoise
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SeamlessNoise___
#define ___SeamlessNoise___

//#include "mTypes.h"
#include "CompileConfig.h"
#include "Randomizer.h"
#include "Point3d.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL SeamlessNoise
{
public:
	SeamlessNoise();
	SeamlessNoise(unsigned int seedVal);
	virtual~SeamlessNoise();

	void seed(unsigned int seedVal);
	float Noise(const vector3d&v);
	float Noise(const vector3d&v,int rep);

	void setRepeat(int r){m_repeat=r;}

private:
	void buildTable();
	float fade(float t);
	float grad(int h,float x,float y,float z);
	int m_repeat;

	Randomizer m_rand;


	static bool s_init;
	static int s_p[512];
	static int s_perm[256];
};

}
}


#endif //___SeamlessNoise___
