
/********************************************************************
	created:	2009/07/17
	created:	17:7:2009   14:59
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IGoal.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IGoal
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGoal___
#define ___IGoal___


#include "CompileConfig.h"
#include "AITypeDef.h"
#include "mString.h"

#include "XMLElement.h"

#include <IValue.h>
#include <map>

namespace mray{
namespace AI{

	class IRenderDevice;

	enum EGoalStatus
	{
		EGS_Active,
		EGS_Inactive,
		EGS_Completed,
		EGS_Paused,
		EGS_Failed
	};

class MRAY_AI_DLL IGoal//:public AIMessageReciver
{
private:
protected:
	bool m_isBlocking;
	mray::ValuesMap m_parameters;
	void AddAttribute(IValue*v);
public:

	IGoal():m_isBlocking(true){}
	virtual~IGoal(){}

	virtual const ValuesMap& GetAttributes();
	virtual IValue* GetAttribute(const core::string&name);

	//type of the goal,important to assign unique types for each goal type
	virtual int GetGoalType()=0;

	virtual void pause()=0;
	virtual void resume()=0;

	virtual bool isBlocking(){return m_isBlocking;}

	virtual void activate()=0;
	virtual EGoalStatus process()=0;
	virtual void terminate()=0;

	virtual bool isActive(){return getStatus()==EGS_Active;}
	virtual bool isInactive(){return getStatus()==EGS_Inactive;}
	virtual bool isComplete(){return getStatus()==EGS_Completed;}
	virtual bool isFailed(){return getStatus()==EGS_Failed;}

	virtual EGoalStatus getStatus()=0;

	virtual bool isPrimitive()=0;

	virtual core::string getType()=0;

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg);

	virtual void Visualize(IRenderDevice*dev,const core::string&prefix);

	virtual void loadXMLSettings(xml::XMLElement*elem);

};

}
}


#endif //___IGoal___
