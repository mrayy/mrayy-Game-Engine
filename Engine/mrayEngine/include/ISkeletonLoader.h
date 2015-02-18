
/********************************************************************
	created:	2009/06/23
	created:	23:6:2009   23:15
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISkeletonLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISkeletonLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISkeletonLoader___
#define ___ISkeletonLoader___

#include "IFileLoader.h"
#include "Skeleton.h"

namespace mray{
	namespace scene{
		class Skeleton;
	}
namespace loaders{

class ISkeletonLoader:public IFileLoader
{
private:
protected:
public:
	ISkeletonLoader(){}
	virtual~ISkeletonLoader(){}

	virtual scene::Skeleton* load(const  core::string& name)=0;
	virtual scene::Skeleton* load(OS::IStream* file)=0;

	virtual bool canLoad(const core::string& fileName)=0;
	virtual bool canLoad(OS::IStream* file)=0;

	virtual const core::string& getDescription()=0;
};

}
}


#endif //___ISkeletonLoader___
