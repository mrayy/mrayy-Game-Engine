


/********************************************************************
	created:	2013/01/26
	created:	26:1:2013   23:35
	filename: 	C:\Development\mrayEngine\Tests\TestOpenCV\GoogleMapHTMLGenerator.h
	file path:	C:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GoogleMapHTMLGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GoogleMapHTMLGenerator__
#define __GoogleMapHTMLGenerator__




namespace mray
{
namespace google
{
	class GMMapData;

class GoogleMapHTMLGenerator
{
protected:
public:
	GoogleMapHTMLGenerator(){}
	virtual~GoogleMapHTMLGenerator(){}

	void Generate(GMMapData* map);
};

}
}


#endif
