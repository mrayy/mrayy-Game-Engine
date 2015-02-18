


/********************************************************************
	created:	2011/06/03
	created:	3:6:2011   22:04
	filename: 	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall\GameStateManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall
	file base:	GameStateManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameStateManager_h__
#define GameStateManager_h__


namespace mray
{
class StateMachine;

namespace Ball
{

class GameStateManager
{
private:
protected:
	StateMachine* m_stateMachine;
public:
	GameStateManager();
	virtual ~GameStateManager();

	void OnEvent(Event*e);
	void Update(float dt);
	
};

}
}


#endif // GameStateManager_h__
