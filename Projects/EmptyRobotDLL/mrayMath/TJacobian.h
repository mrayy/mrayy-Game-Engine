
/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   12:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\TJacobian.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	TJacobian
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this methods calcs Jacobian Inverse as the transpose
*********************************************************************/

#ifndef ___TJacobian___
#define ___TJacobian___

#include "IJacobian.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL TJacobian:public IJacobian
{
private:
protected:
public:
	TJacobian();
	virtual~TJacobian();
	virtual void calcInverse(IJacobian&j);
};

}
}


#endif //___TJacobian___
