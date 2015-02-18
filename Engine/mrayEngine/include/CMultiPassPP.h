


#ifndef ___CMultiPassPP___
#define ___CMultiPassPP___

#include "IPostProcessing.h"

#include "GCPtr.h"
#include "IRenderTarget.h"

namespace mray{
namespace video{

	class IVideoDevice;


class MRAY_DLL CMultiPassPP:public IPostProcessing
{
protected:
	std::list<IPostProcessingPtr>m_postProcessors;
	IRenderArea* m_output;
	video::IVideoDevice*device;
public:
	CMultiPassPP(video::IVideoDevice*dev);
	virtual ~CMultiPassPP();

	virtual void Setup(const math::rectf& targetVP);
	virtual IRenderArea* render(IRenderArea* input);
	virtual IRenderArea* getOutput();

	void addPostProcessor(const IPostProcessingPtr&pp);
	bool removePostProcessor(const IPostProcessingPtr&pp);
	int getPostProcessorCount();
	void clear();
};

}
}




#endif



