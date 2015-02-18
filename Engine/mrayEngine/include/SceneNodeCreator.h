


/********************************************************************
	created:	2010/10/15
	created:	15:10:2010   15:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SceneNodeCreator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SceneNodeCreator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SceneNodeCreator_h__
#define SceneNodeCreator_h__

#include "Gcptr.h"

namespace mray
{
namespace scene
{
	class SLODAnimatedMesh;

class MRAY_DLL SceneNodeCreator
{
private:
protected:
public:

	/*!
	Add an animated mesh (mdl,3ds,smesh,...)
	*/
	static SLODAnimatedMesh* addAnimatedMesh(const  core::string&meshFileName,bool instance);


};

}
}
#endif // SceneNodeCreator_h__
