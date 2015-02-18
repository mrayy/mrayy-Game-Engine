
/********************************************************************
	created:	2012/08/01
	created:	1:8:2012   15:20
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\AttachmentComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	AttachmentComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___AttachmentComponent___
#define ___AttachmentComponent___



namespace mray
{
namespace VT
{

class AttachmentComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:

	core::string m_targetNodeName;
	math::vector3d m_localAxis;
	math::vector3d m_localNormal;
	math::vector3d m_localPoint;

public:
	AttachmentComponent();
	virtual~AttachmentComponent();	
	
};

}
}

#endif
