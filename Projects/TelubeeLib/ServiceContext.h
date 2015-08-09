

#ifndef __ServiceContext__
#define __ServiceContext__

namespace mray
{
namespace TBee
{
	
class ServiceContext
{
protected:

public:
	ServiceContext(){}
	virtual ~ServiceContext(){}

};

class ServiceRenderContext
{
public:
	ServiceRenderContext(){}
	virtual ~ServiceRenderContext(){}
	virtual void RenderText(const core::string &txt, int x, int y, const video::SColor& clr=1){}
	virtual void Reset(){};
};

}
}


#endif
