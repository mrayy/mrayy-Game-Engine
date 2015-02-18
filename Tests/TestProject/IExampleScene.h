
/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   19:56
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\IExampleScene.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	IExampleScene
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IExampleScene___
#define ___IExampleScene___


namespace mray{

class IExampleScene
{
private:
protected:
public:
	IExampleScene(){}
	virtual~IExampleScene(){}

	virtual void onEvent(GCPtr<Event> event){};

	virtual const mchar* getName()=0;
	virtual const mchar* getDescription()=0;

	virtual void createScene()=0;
	virtual void destroyScene()=0;

	virtual void update(float dt)=0;
	
};

}


#endif //___IExampleScene___
