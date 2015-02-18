
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   18:42
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\Sun.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	Sun
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Sun___
#define ___Sun___

#include <SColor.h>
#include <SLightNode.h>

namespace mray{
namespace gameMod{

class Sun
{
private:
protected:
	video::SColor m_color;
	GCPtr<scene::SLightNode> m_light;
public:
	Sun();
	virtual~Sun();

	void setColor(const video::SColor &clr);
	const video::SColor& getColor();

	void attachLight(GCPtr<scene::SLightNode>light);
	GCPtr<scene::SLightNode> getAttachedLight();

	void update(float dt);
};

}
}


#endif //___Sun___
