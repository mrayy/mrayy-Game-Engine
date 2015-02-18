

/********************************************************************
	created:	2011/01/30
	created:	30:1:2011   22:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RoadNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RoadNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef RoadNode_h__
#define RoadNode_h__

#include "compileConfig.h"


namespace mray
{
namespace scene
{
	class SPatchTerrain;
	class IMeshBuffer;

class MRAY_DLL RoadNode
{
private:
protected:

	struct RoadPoint
	{
		math::vector3d pos;
		math::vector3d up;
		math::vector3d side;
	};

	std::vector<RoadPoint> m_points;

	float m_width;

public:
	RoadNode();
	virtual ~RoadNode();

	void SetRoadCVs(const std::vector<math::vector3d>& cvs);
	void Build(float w,IMeshBuffer* buffer);

	void Build(SPatchTerrain*terrain);

};

}
}


#endif // RoadNode_h__

