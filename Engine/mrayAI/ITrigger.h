

/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   16:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ITrigger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ITrigger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	like an if(condition)then Trigger something
*********************************************************************/

#ifndef ___ITrigger___
#define ___ITrigger___


namespace mray{
namespace AI{

class TriggerSystem;

class ITrigger
{
private:
protected:
	TriggerSystem* m_system;
	bool m_shouldRemove;
	bool m_isActive;

	void RemoveTrigger(){m_shouldRemove=true;}
public:
	ITrigger():m_system(0),m_shouldRemove(0),m_isActive(true)
	{}
	virtual~ITrigger()
	{}

	bool ShouldBeRemoved(){return m_shouldRemove;}
	bool IsActive(){return m_isActive;}
	void SetActive(bool a){m_isActive=a;}

	TriggerSystem* getTriggerSystem(){return m_system;}
	void setTriggerSystem(TriggerSystem*s){m_system=s;}

	virtual void TestTrigger()=0;	//test the trigger condition

};

}
}


#endif //___ITrigger___
