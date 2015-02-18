

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   0:11
	filename: 	C:\Development\mrayEngine\Tests\TestOpenCV\GMMapTypeControlOptions.h
	file path:	C:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GMMapTypeControlOptions
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GMMapTypeControlOptions__
#define __GMMapTypeControlOptions__




namespace mray
{
namespace google
{

enum MapTypeControlStyle
{
	Default,
	DropDownMenu,
	HorizontalBar
};

enum MapTypeID
{
	Hybrid,
	Roadmap,
	Satellite,

};

class GMMapTypeControlOptions
{
protected:
public:
	GMMapTypeControlOptions(){}
	virtual~GMMapTypeControlOptions(){}

	MapTypeID type;
	core::string control;
	MapTypeControlStyle style;
};

}
}

#endif
