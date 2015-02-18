

/********************************************************************
	created:	2011/06/03
	created:	3:6:2011   22:10
	filename: 	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall\IGameState.h
	file path:	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall
	file base:	IGameState
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGameState_h__
#define IGameState_h__


namespace mray
{
	class Event;
namespace Ball
{

class IGameState
{
private:
protected:
	core::string m_name;
public:
	IGameState(const core::string& str):m_name(str){}
	virtual ~IGameState(){}

	const core::string& GetName(){return m_name;}
	
	virtual void OnEvent(Event*e)=0;

	virtual void Enter(IGameState*prev)=0;
	virtual void Update(float dt)=0;
	virtual void Exit()=0;
};

}
}


#endif // IGameState_h__
