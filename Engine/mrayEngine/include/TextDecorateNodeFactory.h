

/********************************************************************
	created:	2010/09/01
	created:	1:9:2010   23:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TextDecorateNodeFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TextDecorateNodeFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TextDecorateNodeFactory_h__
#define TextDecorateNodeFactory_h__


#include "ISingleton.h"
#include "CompileConfig.h"
#include "ITextDecorateNode.h"


namespace mray
{
namespace GUI
{

class MRAY_DLL TextDecorateNodeFactory:public ISingleton<TextDecorateNodeFactory>
{
private:
protected:
	typedef std::map<uint, ITextDecorateCreator*> CreatorMap;
	CreatorMap m_creators;
public:
	TextDecorateNodeFactory();
	virtual ~TextDecorateNodeFactory();

	void RegisterCreator(ITextDecorateCreator*c);
	void UnRegisterCreator(const core::string&type);
	void Clear();
	
	ITextDecorateNode* CreateNode(const core::string&type);
};

}
}


#endif // TextDecorateNodeFactory_h__
