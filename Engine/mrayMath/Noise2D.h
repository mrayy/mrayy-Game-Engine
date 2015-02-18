
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   21:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Noise2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Noise2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Noise2D___
#define ___Noise2D___

//#include "mTypes.h"
#include "CompileConfig.h"
#include "Randomizer.h"
#include "Point2d.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL Noise2D
{
public:
	Noise2D(unsigned int seedVal=1023058);
	virtual~Noise2D();

	void seed(unsigned int seedVal);
	double Noise(const vector2d&v);

private:
	void buildTable();
	void buildCoefs(const vector2d&v);
	int fold(const vector2d&v);
	double interp(double t);


	static const unsigned int TableSize=256;
	vector2d m_Table[TableSize];
	vector2d m_vCoef[4];
	int m_iCoef[4];

	int m_perm[2*TableSize];


	Randomizer m_rand;
};

}
}


#endif //___Noise2D___
