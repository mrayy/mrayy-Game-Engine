

/********************************************************************
	created:	2010/04/19
	created:	19:4:2010   23:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIIntializing.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIIntializing
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	setup AI static items
*********************************************************************/

#ifndef AIIntializing_h__
#define AIIntializing_h__

#include "compileConfig.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL AIIntializing
{
private:
protected:
	static bool m_isInitiated;
public:
	static void InitAI();

	static void ShutdownAI();
	
};

}
}

#endif // AIIntializing_h__
