
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   21:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Noise3D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Noise3D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Noise3D___
#define ___Noise3D___

//#include "mTypes.h"
#include "CompileConfig.h"
#include "Randomizer.h"
#include "Point3d.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL Noise3D
{
public:
	Noise3D(unsigned int seedVal=1023058);
	virtual~Noise3D();

	void seed(unsigned int seedVal);
	float Noise(const vector3d&v);

private:
	void buildTable();
	void buildCoefs(const vector3d&v);
	int fold(const vector3d&v);
	double interp(double t);


	static const unsigned int TableSize=256;
	vector3d m_Table[TableSize];
	vector3d m_vCoef[8];
	int m_iCoef[8];

	int m_perm[2*TableSize];

	Randomizer m_rand;
};

}
}


#endif //___Noise3D___
