
/********************************************************************
	created:	2009/04/12
	created:	12:4:2009   14:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIEffectorFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIEffectorFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIEffectorFactory___
#define ___GUIEffectorFactory___


#include "IGUIEffectorCreator.h"
#include "ISingleton.h"

#include <map>


namespace mray{
namespace GUI{

class GUIEffectorFactory:public ISingleton<GUIEffectorFactory>
{
private:
protected:
	typedef std::map<core::string,GCPtr<IGUIEffectorCreator>> EffectorCreators;
	EffectorCreators m_creators;
public:
	GUIEffectorFactory();
	virtual~GUIEffectorFactory();

	void addEffectorCreator(GCPtr<IGUIEffectorCreator> c);
	void removeEffectorCreator(const core::string&type);

	GCPtr<GUI::GUIEffector> createEffector(const core::string&type);
};

}
}

#endif //___GUIEffectorFactory___
