


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   14:48
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFIController.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFIController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFIController__
#define __JFIController__


namespace mray
{
namespace game
{

	class JFSpaceShip;

class JFIController
{
private:
protected:
	JFSpaceShip* m_controlledShip;
public:
	JFIController():m_controlledShip(0){}
	virtual~JFIController(){}

	virtual void SetShip(JFSpaceShip*s){m_controlledShip=s;}
	virtual JFSpaceShip* GetShip(){return m_controlledShip;}

};


}
}

#endif

