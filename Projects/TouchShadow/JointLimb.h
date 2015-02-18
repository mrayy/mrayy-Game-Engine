

/********************************************************************
	created:	2013/06/19
	created:	19:6:2013   22:28
	filename: 	C:\Development\IVRC\User Tracking\JointLimb.h
	file path:	C:\Development\IVRC\User Tracking
	file base:	JointLimb
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __JointLimb__
#define __JointLimb__




namespace mray
{

struct JointLimb
{
protected:
public:

	static JointLimb instance;
	JointLimb(){Init();}
	JointLimb(nite::JointType ja,nite::JointType jb):a(ja),b(jb)
	{
	}
	nite::JointType a;
	nite::JointType b;

	static std::vector<JointLimb> Limbs;
	static void Init();

};



}


#endif
