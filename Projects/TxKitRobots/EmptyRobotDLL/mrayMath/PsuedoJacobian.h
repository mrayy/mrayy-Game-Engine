

/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   15:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\PsuedoJacobian.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	PsuedoJacobian
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PsuedoJacobian___
#define ___PsuedoJacobian___


#include "IJacobian.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL PsuedoJacobian:public IJacobian
{
private:
	IJacobian transJaco;
	IJacobian JacJJT;//this will hold : J.J^T
	IJacobian JacInverse;
	
	//tmp vectors
	IJacobian nrCopy;
	int *indxc;
	int *indxr;
	int *ipiv;


	static void gaussj(float **a, int n, float **b, int m,int *indxc, int *indxr, int *ipiv) ;

	void _calcInverse();
protected:
public:
	PsuedoJacobian();
	virtual~PsuedoJacobian();
	virtual void calcInverse(IJacobian&j);
};

}
}

#endif //___PsuedoJacobian___
