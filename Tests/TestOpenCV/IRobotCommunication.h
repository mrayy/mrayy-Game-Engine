


/********************************************************************
	created:	2012/05/22
	created:	22:5:2012   12:58
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\IRobotCommunication.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	IRobotCommunication
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IRobotCommunication__
#define __IRobotCommunication__


namespace mray
{

	// user side : From PC to robot communication layer
class IRobotCommunication
{
private:
protected:
public:
	IRobotCommunication(){}
	virtual~IRobotCommunication(){}

	virtual void OnHeadMovement(const math::vector3d& angles)=0;
	virtual void OnSpeedApply(const math::vector2d& angles)=0;
};


}

#endif
