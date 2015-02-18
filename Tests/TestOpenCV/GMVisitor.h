

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   0:00
	filename: 	C:\Development\mrayEngine\Tests\TestOpenCV\GMVisitor.h
	file path:	C:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GMVisitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GMVisitor__
#define __GMVisitor__




namespace mray
{
namespace google
{


class GMVisitor
{
protected:
public:
	GMVisitor(){}
	virtual~GMVisitor(){}

	virtual void OnVisit(GMStyleInfo* s){};
};

	}
}


#endif
