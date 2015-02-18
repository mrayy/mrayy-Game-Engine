


/********************************************************************
	created:	2011/02/23
	created:	23:2:2011   14:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IMeshFileCreator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IMeshFileCreator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IMeshFileCreator_h__
#define IMeshFileCreator_h__

#include "CompileConfig.h"
#include "mstring.h"

namespace mray
{
namespace scene
{

class MRAY_DLL IMeshFileCreator
{
private:
protected:
	core::string m_type;
public:
	IMeshFileCreator(const core::string& type):m_type(type)
	{}
	virtual ~IMeshFileCreator(){}

	const core::string& GetType()const{return m_type;}

	virtual MeshRenderableNode* LoadFromFile(const core::string&str,bool instanced=true)=0;
	
};

}
}


#endif // MeshFileCreator_h__
