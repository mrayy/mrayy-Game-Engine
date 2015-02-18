
#ifndef ___IPostProcessing___
#define ___IPostProcessing___

#include "GCPtr.h"

namespace mray{
namespace video{

	class IRenderArea;
	class IRenderTarget;

class IPostProcessing
{
protected:
	bool m_enabled;
public:
	IPostProcessing():m_enabled(1){}
	virtual ~IPostProcessing(){}

	void setEnable(bool e){
		m_enabled=e;
	}
	bool isEnabled(){
		return m_enabled;
	}

	virtual void Setup(const math::rectf& targetVP)=0;
	virtual IRenderArea* render(IRenderArea* input) = 0;
	virtual IRenderArea* getOutput() = 0;
};

MakeSharedPtrType(IPostProcessing);


}
}


#endif




