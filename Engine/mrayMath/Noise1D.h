
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   21:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Noise1D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Noise1D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Noise1D___
#define ___Noise1D___

//#include "mTypes.h"
#include "CompileConfig.h"
#include "Randomizer.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL Noise1D
{
public:
	Noise1D(unsigned int seedVal=1023058);
	virtual~Noise1D();

	void seed(unsigned int seedVal);
	float Noise(float v);

private:
	void buildTable();
	void buildCoefs(float v);
	int fold(float v);
	double interp(double v);


	static const unsigned int TableSize=256;
	float m_Table[TableSize];
	float m_vCoef[2];
	int m_iCoef[2];

	int m_perm[2*TableSize];

	Randomizer m_rand;
};

}
}


#endif //___Noise1D___
