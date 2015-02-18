
/********************************************************************
	created:	2010/02/11
	created:	11:2:2010   11:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\IJacobian.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	IJacobian
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IJacobian___
#define ___IJacobian___


#include "compileConfig.h"
#include "GenericMatrix.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL IJacobian:public GenericMatrix
{
private:
protected:
public:
	IJacobian();
	virtual~IJacobian();


	virtual void calcInverse(IJacobian&){};
};

}
}


#endif //___IJacobian___
