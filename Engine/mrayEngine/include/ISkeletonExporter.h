
/********************************************************************
	created:	2009/06/23
	created:	23:6:2009   23:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ISkeletonExporter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ISkeletonExporter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISkeletonExporter___
#define ___ISkeletonExporter___


#include "IFileWriter.h"
#include "Skeleton.h"

namespace mray{
namespace loaders{

class ISkeletonExporter:public IFileWriter
{
private:
protected:
public:
	ISkeletonExporter(){}
	virtual~ISkeletonExporter(){}

	virtual void write(scene::Skeleton* s,OS::IStream* file)=0;

	virtual bool canWrite(const core::string& fileName)=0;
	virtual const core::string& getExt()=0;

	virtual const core::string& getDescription()=0;
};

}
}


#endif //___ISkeletonExporter___
