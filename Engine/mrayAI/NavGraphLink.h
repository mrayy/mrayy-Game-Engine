
/********************************************************************
	created:	2010/03/08
	created:	8:3:2010   20:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavGraphLink.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavGraphLink
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavGraphLink___
#define ___NavGraphLink___

#include "IGraphLink.h"
#include "NavGraphNode.h"


namespace mray{
namespace AI{

class NavMeshPolygon;

class MRAY_AI_DLL NavGraphLink:public IGraphLink
{
private:
protected:
	float m_weight;
	NavMeshPolygon* m_ownerNode;
public:
	NavGraphLink();
	NavGraphLink(float weight,NavMeshPolygon*owner,NavGraphNode* begin,NavGraphNode* end,int id
		/*,const core::string &name,*/);

	virtual~NavGraphLink();

	void setOwnerNode(NavMeshPolygon*node);
	NavMeshPolygon* getOwnerNode();
	virtual bool isBidirectional();

	virtual float getWeightBeginEnd(){return m_weight;}
	virtual float getWeightEndBegin(){return m_weight;}
	IGraphNode* GetNearestNode(const math::vector3d&p);
};

}
}


#endif //___NavGraphLink___
