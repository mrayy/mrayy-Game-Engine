

/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   11:04
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGShaderManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGShaderManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGShaderManager__
#define __SGShaderManager__


namespace mray
{
namespace shader
{

class SGShaderLibrary;

class SGShaderManager
{
public:
	typedef std::list<SGShaderLibrary*> LibrayList;
private:
protected:
	LibrayList m_libraries;
public:
	SGShaderManager();
	virtual~SGShaderManager();

	bool LoadShaderLibrary(const core::string& path);
	void LoadShaderLibraries(const core::string& directory);

};

}
}

#endif
