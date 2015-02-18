

/********************************************************************
	created:	2010/04/18
	created:	18:4:2010   23:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IAIComponent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IAIComponent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IAIComponent_h__
#define IAIComponent_h__


#include "CompileConfig.h"
#include "IObjectComponent.h"
#include "TypedProperty.h"
#include "IVisualizable.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL IAIComponent:public IObjectComponent,public IVisualizable
{
	DECLARE_RTTI
private:
	bool m_enableVisualize;
protected:
	float m_componentTriggerPeriod;
	float m_componentUpdateTime;

	DECLARE_PROPERTY_TYPE(Type,core::string,MRAY_AI_DLL);
public:
	IAIComponent();
	virtual ~IAIComponent();

	void EnableVisualize(bool e);
	bool IsEnableVisualize();

	virtual void Update(float dt){};
	void UpdateComponent(float dt);

	virtual void Visualize(IRenderDevice*dev){}
	void VisualizeComponent(IRenderDevice*dev);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	
};

}
}

#endif // IAIComponent_h__
