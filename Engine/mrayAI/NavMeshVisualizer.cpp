#include "stdafx.h"

#include "NavMeshVisualizer.h"
#include "DefaultColors.h"
#include "NavGraphLink.h"
#include "IRenderDevice.h"


namespace mray{
namespace AI{

NavMeshVisualizer::NavMeshVisualizer(NavMesh*world)
{
	m_navMesh=world;
}
NavMeshVisualizer::~NavMeshVisualizer()
{

}

void NavMeshVisualizer::DrawNode(IRenderDevice*dev,AI::INavWorldNode*poly){
	NavMeshPolygon&p=*(NavMeshPolygon*)poly;

	const math::vector3d* vptr=m_navMesh->GetVerticies();


	int vCnt=p.indicies.size();
	int iCnt=p.indicies.size();

	math::vector3d*v=&m_points[0];
	ushort*ind=&m_indicies[0];
	for(ushort i=0;i<p.indicies.size()-1;++i){
		dev->drawLine(vptr[p.indicies[i]],vptr[p.indicies[i+1]],video::DefaultColors::DimGray);
	}

}
void NavMeshVisualizer::DrawWorld(IRenderDevice*dev)
{
	GraphLinksList::const_iterator it=m_navMesh->GetGraphLinks().begin();
	GraphLinksList::const_iterator end=m_navMesh->GetGraphLinks().end();
	for(;it!=end;++it)
	{
		NavGraphLink&link=*(NavGraphLink*)*it;
		dev->drawLine(((NavGraphNode*)link.m_begin)->position,((NavGraphNode*)link.m_end)->position,video::DefaultColors::DimGray);
	}
}
void NavMeshVisualizer::DrawNodes(IRenderDevice*dev,const NavWorldNodeListPtr&nodes,bool drawConnections)
{

	NavWorldNodeListPtr::const_iterator it=nodes.begin();
	const math::vector3d* vptr=m_navMesh->GetVerticies();
	it=nodes.begin();
	for(;it!=nodes.end();++it){
		NavMeshPolygon&p=*(NavMeshPolygon*)*it;
		for(int i=1;i<p.indicies.size()+1;++i){
			dev->drawLine(vptr[p.indicies[i-1]],vptr[p.indicies[i%p.indicies.size()]],video::DefaultColors::DimGray);
		}

	}
//	dev->renderLines(v,vCnt,ind,iCnt,video::DefaultColors::DimGray);
}

void NavMeshVisualizer::DrawLinks(IRenderDevice*dev,GraphLinksList&links)
{


	GraphLinksList::iterator it=links.begin();

	for(;it!=links.end();++it){
		IGraphLink*link=*it;
		dev->drawLine(((NavGraphNode*)link->m_begin)->position,((NavGraphNode*)link->m_end)->position,video::DefaultColors::Red);
	}
}


}
}
