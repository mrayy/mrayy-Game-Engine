

#ifndef __ISCENE__
#define __ISCENE__

namespace mray
{
	
class IScene
{
protected:

public:
	IScene(){}
	virtual ~IScene(){}

	virtual void OnInit() = 0;
	virtual void OnRender(const math::rectf& rc) = 0;
	virtual void OnUpdate(float dt) = 0;

};

}


#endif