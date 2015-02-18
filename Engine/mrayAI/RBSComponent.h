

/********************************************************************
	created:	2010/04/24
	created:	24:4:2010   20:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\RBSComponent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	RBSComponent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RBSComponent_h__
#define RBSComponent_h__

#include "CompileConfig.h"
#include "IAIComponent.h"

#include <RuleBasedSystem.h>

namespace mray
{
namespace AI
{

class MRAY_AI_DLL RBSComponent:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	RuleBasedSystem* m_RBS;

public:
	RBSComponent();
	virtual ~RBSComponent();

	RuleBasedSystem* GetRBS();

	void Tick();

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // RBSComponent_h__
