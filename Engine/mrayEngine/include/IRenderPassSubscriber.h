


/********************************************************************
	created:	2012/09/03
	created:	3:9:2012   1:04
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IRenderPassSubscriber.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IRenderPassSubscriber
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IRenderPassSubscriber___
#define ___IRenderPassSubscriber___



namespace mray
{
namespace scene
{
	class IRenderPass;

class IRenderPassSubscriber
{
protected:

public:
	IRenderPassSubscriber(){}
	virtual~IRenderPassSubscriber(){}
	
	virtual void GetRenderPasses(std::list<IRenderPass*>& plst)=0;
};

}
}

#endif
