
/********************************************************************
	created:	2009/04/24
	created:	24:4:2009   0:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRenderGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRenderGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRenderGroup___
#define ___IRenderGroup___

#include "RenderOperation.h"

namespace mray{
	class BenchmarkItem;
namespace scene{

	class IRenderPass;

class MRAY_DLL IRenderGroup
{
private:
protected:
public:
	enum ESortType{
		Sort_None,
		Sort_ASC,
		Sort_DES
	};

	IRenderGroup(){}
	virtual~IRenderGroup(){}

	virtual void addRenderOperation(const RenderOperation& op)=0;

	virtual void sort()=0;

	virtual void setSortType(ESortType s)=0;
	virtual ESortType getSortType()=0;

	virtual void performRender(IRenderPass*pass)=0;

	virtual void clear()=0;

	virtual BenchmarkItem* getBenchmarkItem()=0;

};

}
}


#endif //___IRenderGroup___
