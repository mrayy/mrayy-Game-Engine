


/********************************************************************
	created:	2012/06/27
	created:	27:6:2012   16:05
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\XMLShapeSerializer.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	XMLShapeSerializer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __XMLShapeSerializer__
#define __XMLShapeSerializer__


namespace mray
{
namespace physics
{

class MRAY_DLL XMLShapeSerializer
{
private:
protected:
public:
	XMLShapeSerializer(){}
	virtual~XMLShapeSerializer(){}

	void LoadXML(const core::string& path);

};

}
}

#endif
