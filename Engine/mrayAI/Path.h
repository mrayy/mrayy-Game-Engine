
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   18:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\Path.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	Path
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Path___
#define ___Path___

#include "compileConfig.h"
#include "PathEdge.h"

namespace mray{
namespace AI{

class MRAY_AI_DLL Path
{
private:
protected:
	PathEdgeList m_path;
public:
	Path();
	Path(const PathEdgeList& path);
	Path(const Path& o);
	virtual~Path();

	void SetPath(const PathEdgeList& path);
	void Clear();

	float CalculateLength();

	PathEdgeList& GetPath();
	const PathEdgeList& GetPath()const;
	
};

}
}


#endif //___Path___
