


/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   17:24
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\BallCreator.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	BallCreator
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___BallCreator___
#define ___BallCreator___


namespace mray
{
namespace VT
{

class BallCreator
{
protected:
	float m_time;
public:
	BallCreator():m_time(0){}
	virtual~BallCreator(){}

	game::GameEntity* GenerateBall(game::GameEntityManager* mngr,float radius,const math::vector3d& pos);

	void AutoGenerate(float dt,game::GameEntityManager* mngr,float radius,const math::vector3d& pos);
};

}
}

#endif
