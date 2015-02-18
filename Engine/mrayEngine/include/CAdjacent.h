
/********************************************************************
	created:	2009/07/27
	created:	27:7:2009   23:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CAdjacent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CAdjacent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CAdjacent___
#define ___CAdjacent___

#include <set>



#include "CompileConfig.h"
#include "mTypes.h"

namespace mray{
namespace scene{


typedef std::set<ushort> shortSet;
typedef std::map<ushort,shortSet> shortSetMap;


class MRAY_DLL CAdjacent
{
	struct Index3
	{
		Index3(ushort i1,ushort i2,ushort i3){
			this->i1=i1;
			this->i2=i2;
			this->i3=i3;
		}
		ushort i1;
		ushort i2;
		ushort i3;
	};

	struct Edge{
		ushort i1;
		ushort i2;

		bool operator==(const Edge&o)const
		{
			return (i1==o.i1 && i2==o.i2)||
				(i1==o.i2 && i2==o.i1);
		}
		bool operator<(const Edge&o)const
		{
			return (i1<o.i1 && i2<o.i2);
		}
	};
	struct Triangle{
		ushort e1;
		ushort e2;
		ushort e3;
	};
	typedef std::vector<Edge> EdgeSet;
	typedef std::vector<Triangle> TriList;

public:
	CAdjacent();
	virtual~CAdjacent();

	void calcAdjacent(ushort*idx,uint count);

	void reset();

	const shortSet& getEdgeAdjacent(ushort i);
	const shortSet& getTriAdjacent(ushort i);

private:

	void getEdgesIncr(const Index3&i,ushort xyz[3]);

	inline uint HashEdge(ushort e1,ushort e2) {
		if(e2<e1)
			return e1 | e2<<16;
		else return e2 | e1<<16;
	}

	void generateEdges(ushort*idx,uint count);
	void generateTriangles(ushort*idx,uint count);

	typedef std::map<uint,uint> EdgeIDMap;

	EdgeIDMap m_edgeIDMap;

	shortSetMap m_edgeMap;
	shortSetMap m_TriMap;

	EdgeSet m_edges;
	TriList m_triangles;

};


}
}

#endif //__CAdjacent_h
