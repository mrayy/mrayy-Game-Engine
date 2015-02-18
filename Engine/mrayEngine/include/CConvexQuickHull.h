

#ifndef ___CConvexQuickHull___
#define ___CConvexQuickHull___

#include "IConvexHull.h"

namespace mray{
namespace scene{

class CConvexQuickHull:public IConvexHull
{
	std::vector<int>vtxCopy;

	void copyVertices();

	void addVerticesToFace(std::vector<int>&vertices,IConvexHull::SFace &face);

	bool getExtreme(IConvexHull::SFace &face,int &bestV);
	bool getMinMax(int &p1,int &p2,int &p3);
public:
	CConvexQuickHull(GCPtr<scene::SMesh>mesh);
	virtual ~CConvexQuickHull();

	virtual void createConvexHull(GCPtr<scene::SMesh>mesh);
};

}
}

#endif




