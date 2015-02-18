

/********************************************************************
	created:	2010/06/27
	created:	27:6:2010   17:07
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\AIGameArch.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	AIGameArch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIGameArch_h__
#define AIGameArch_h__

#include "CompileConfig.h"
#include <IAIArchitect.h>
#include "EAgentType.h"

namespace mray
{
namespace GameMod
{


class CHARACTERDEMOGM_API AIGameArch:public AI::IAIArchitect
{
private:
protected:
	EAgentType m_type;
	AI::AIActor* m_owner;
public:
	AIGameArch(AI::AIActor*owner,EAgentType type):m_owner(owner),m_type(type)
	{}
	virtual ~AIGameArch(){}

	void SetOwner(AI::AIActor*owner){m_owner=owner;}
	AI::AIActor* GetOwner(){return m_owner;}

	EAgentType GetAgentType(){return m_type;}

	virtual bool IsAIControlled(){return true;}
	
};

}
}
#endif // AIGameArch_h__
