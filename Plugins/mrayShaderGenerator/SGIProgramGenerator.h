

/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   11:38
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGIProgramGenerator.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGIProgramGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGIProgramGenerator__
#define __SGIProgramGenerator__


namespace mray
{
namespace OS
{
	class StreamWriter;
}
namespace shader
{
	class SGProgram;

class SGIProgramGenerator
{
private:
protected:
public:
	SGIProgramGenerator(){}
	virtual~SGIProgramGenerator(){}

	virtual const core::string& GenerateShader(SGProgram*program)=0;

	virtual const core::string& GetType()const=0;

};

class SGIProgramGeneratorFactory
{
public:
	virtual const core::string& GetType()const=0;
	virtual SGIProgramGenerator* CreateGenerator()const=0;
};

}
}

#endif
