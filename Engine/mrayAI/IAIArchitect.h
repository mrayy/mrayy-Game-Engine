
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   15:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IAIArchitect.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IAIArchitect
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	declare an abstract interface to control an actor
				the controller can be simply a player controller (keyboard + mouse)
				to complex AI controllers
*********************************************************************/

#ifndef ___IAIArchitect___
#define ___IAIArchitect___

#include "CompileConfig.h"
#include "AITypeDef.h"
#include "mstring.h"
#include "IVisualizable.h"
#include "IObjectComponent.h"
#include "RTTI.h"
#include <map>

namespace mray{
namespace AI{

	class AgentObstacle;
	class IAIComponent;

class MRAY_AI_DLL IAIArchitect:public IObjectComponent,public IVisualizable
{
	DECLARE_RTTI;
private:
protected:
	bool m_isVisible;

public:
	IAIArchitect();
	virtual~IAIArchitect();

	void EnableVisualize(bool e);
	bool IsEnableVisualize();

	//if the architect contains an obstacle definition,then we will need it for dynamic obstacles system
	virtual AgentObstacle* GetObstacle()=0;

	virtual bool IsAIControlled()=0;

	virtual void Update(float dt);

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg)=0;

	virtual void Visualize(IRenderDevice*dev);

};

}
}


#endif //___IAIArchitect___
