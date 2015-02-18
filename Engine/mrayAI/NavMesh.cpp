#include "stdafx.h"

#include "NavMesh.h"
#include "Plane.h"
#include "NavMeshVisualizer.h"
#include "NavAStarSearch.h"
#include "IRenderDevice.h"
#include "NavGraphLink.h"
#include "NavGraphNode.h"
#include "NavMeshHeuristic.h"
#include "AISystem.h"
#include <mraySystem.h>
#include <ILogManager.h>
#include <IStream.h>
#include <IFileSystem.h>
#include <XMLTree.h>

namespace mray{
namespace AI{

#define HEADER_MAGIC_NUMBER 0x60

NavMesh::NavMesh(AISystem* system):m_isVisible(false),m_system(system)
{
	m_slope=0.9;
	m_maxStepHeight=1.1;
	m_visualizer=new NavMeshVisualizer(this);
}
NavMesh::~NavMesh()
{ 
	Clear();

	delete m_visualizer;
}


void NavMesh::SetGridCellSize(float sz)
{
	m_grid.SetCellSize(sz);
}
void NavMesh::SetMaxSlope(float slope)
{
	m_slope=slope;
}
float NavMesh::GetMaxSlope()
{
	return m_slope;
}

void NavMesh::SetMaxStepSize(float step)
{
	m_maxStepHeight=step;
}
float NavMesh::GetMaxStepSize()
{
	return m_maxStepHeight;
}
void NavMesh::Generate(const math::vector3d*vert,int vCount,const void*ind,int indCount,int *attrs,bool is16bitIdx,const math::matrix4x4*transMat)
{
	Clear();
	_copyData(vert,vCount,ind,indCount,attrs,is16bitIdx,transMat);
	_optimize();
	GenerateGraph();
	GenerateWalls();
	//create the grid
	m_grid.SetGrindBounds(m_boundingBox);
	m_grid.CreateGrid();
	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	for(;it!=m_polygons.end();++it){
		m_grid.InsertPoly((NavMeshPolygon*)*it);
	}

	if(m_system)
		m_system->OnNavMeshLoaded();
}


bool NavMesh::getAdjEdge(NavMeshPolygon*p1,NavMeshPolygon*p2,uint &oi1,uint &oi2)
{
	return getAdjEdge(p1,p2,0,p1->indicies.size(),oi1,oi2);
}


bool NavMesh::getAdjEdge(NavMeshPolygon*p1,NavMeshPolygon*p2,int s,int cnt,uint &oi1,uint &oi2)
{
	uint i11,i12;
	uint i21,i22;
	int indCnt=p1->indicies.size();
	NavWorldNodeList::iterator it;
	for(int i=s;i<s+cnt;++i){
		i11=math::Min(p1->indicies[i],p1->indicies[(i+1)%indCnt]);
		i12=math::Max(p1->indicies[i],p1->indicies[(i+1)%indCnt]);
		int ind2Cnt=p2->indicies.size();
		for(int i2=0;i2<ind2Cnt;++i2){
			i21=math::Min(p2->indicies[i2],p2->indicies[(i2+1)%ind2Cnt]);
			i22=math::Max(p2->indicies[i2],p2->indicies[(i2+1)%ind2Cnt]);
			if(i21==i11 && i22==i12){
				oi1=i;
				oi2=i2;
				return true;
			}
		}
	}
	return false;
}
void NavMesh::_onAddPolygon(NavMeshPolygon*p)
{
	if(p->indicies.size()<3)
		return;

	uint i11,i12;
	uint i21,i22;
	int indCnt=p->indicies.size();
	NavWorldNodeListPtr::iterator it,end;
	NavMeshPolygon::NavMeshPolyAdj* tmp;
	for(int i=0;i<indCnt;++i){
		if(p->adjacent[i])
			continue;
		i11=math::Min(p->indicies[i],p->indicies[(i+1)%indCnt]);
		i12=math::Max(p->indicies[i],p->indicies[(i+1)%indCnt]);
		it=m_polygons.begin();
		end=m_polygons.end();
		bool found=false;
		for(;!found && it!=end;++it){
			if(*it==p)continue;
			NavMeshPolygon* p2=(NavMeshPolygon*)*it;

			int ind2Cnt=p2->indicies.size();
			for(int i2=0;i2<ind2Cnt;++i2){
				if( p2->adjacent[i2])
					continue;
				i21=math::Min(p2->indicies[i2],p2->indicies[(i2+1)%ind2Cnt]);
				i22=math::Max(p2->indicies[i2],p2->indicies[(i2+1)%ind2Cnt]);
				if(i21==i11 && i22==i12){
					//math::vector3d midPoint=(m_verticies[i21]+m_verticies[i22])*0.5;
					tmp=new NavMeshPolygon::NavMeshPolyAdj(p2);
					p->adjacent[i]=tmp;

					tmp=new NavMeshPolygon::NavMeshPolyAdj(p);
					p2->adjacent[i2]=tmp;
					found=true;
					break;
				}
			}
		}
	}
}

void NavMesh::_onRemovePolygon(NavMeshPolygon*p)
{/*
	NavMeshPolygon::NavMeshPolyAdjList::iterator it;
	for(int i=0;i<p->adjacent.size();++i){
		it=p->adjacent[i].begin();
		for(;it!=p->adjacent[i].end();++it)
	}*/
}

bool NavMesh::_MergePolygons(NavMeshPolygon*p1,NavMeshPolygon*p2,int e,NavMeshPolygon*&resPoly){
	resPoly=0;
	//should have the same attribute to be able to merge
	if(p1->attribute!=p2->attribute)
		return 0;
	//test if both polygons are don't have the same dir
	if(p1->plane.Normal.dotProduct(p2->plane.Normal)<0.9)
	{
		return 0;
	}
	math::vector3d normal=(p1->plane.Normal+p2->plane.Normal)*0.5;
	float dp=normal.dotProduct(math::vector3d::YAxis);
	math::box3d bbox;
	//trivial test..
	bbox=p1->boundingbox;
	bbox.AddBox(p2->boundingbox);
	if(bbox.getHeight()> m_maxStepHeight && dp<m_slope ){
		return true;
	}

	NavMeshPolygon*outPoly=new NavMeshPolygon();

	int sz1=p1->indicies.size();
	int sz2=p2->indicies.size();
	uint i;
	int e2;

	int e2Index;
	outPoly->plane.Normal=normal;
	outPoly->boundingbox=bbox;
	outPoly->attribute=p1->attribute;

	//result poly's normal is the same as the p1,p2 normals

	//result poly's edges count is the sum of p1,p2 edges - 2(shared edges)
	outPoly->setNumberOfEdges(sz1+sz2-2);
	outPoly->indicies.resize(0);
	//outPoly->adjacent.resize(0);

	//create the poly by adding indicies starting from the 2nd vert of A poly,then add B verticies
	for(i=1;i<sz1;++i){
		int idx=(i+e)%sz1;
		outPoly->indicies.push_back(p1->indicies[idx]);
	}
	//find the vertex of B
	uint edgeP2= p1->indicies[e];
	for(i=0;i<sz2;++i){
		if(p2->indicies[i]==edgeP2){
			e2=i;
			break;
		}
	}
	e2Index=outPoly->indicies.size();

	for(i=0;i<sz2-1;++i){
		int idx=(i+e2)%sz2;
		outPoly->indicies.push_back(p2->indicies[idx]);
	}


	//make sure it's a convex poly
	if(!isConvex(outPoly)){
		delete outPoly;
		return true;
	}
	outPoly->adjacent.resize(outPoly->indicies.size());
	{
		int adjID=0;
		NavMeshPolygon::NavMeshPolyAdj*adPoly;
		for(int i=1;i<sz1;++i,++adjID){
			int idx=(i+e)%sz1;
			if(p1->adjacent[idx]){
				adPoly=p1->adjacent[idx];
				outPoly->adjacent[adjID]=
					new AI::NavMeshPolygon::NavMeshPolyAdj(adPoly->poly);
				int edge=adPoly->poly->getEdgeIndex(outPoly->indicies[adjID],outPoly->indicies[adjID+1]);
				delete adPoly->poly->adjacent[edge];
				adPoly->poly->adjacent[edge]=
					new AI::NavMeshPolygon::NavMeshPolyAdj(outPoly);
			}
		}
		for(int i=0;i<sz2-1;++i,++adjID){
			int idx=(i+e2)%sz2;
			if(p2->adjacent[idx]){
				adPoly=p2->adjacent[idx];
				outPoly->adjacent[adjID]=
					new AI::NavMeshPolygon::NavMeshPolyAdj(adPoly->poly);
				int edge=adPoly->poly->getEdgeIndex(outPoly->indicies[adjID],
					outPoly->indicies[(adjID+1)%outPoly->indicies.size()]);
				delete adPoly->poly->adjacent[edge];
				adPoly->poly->adjacent[edge]=
					new AI::NavMeshPolygon::NavMeshPolyAdj(outPoly);
			}
		}
	}


/*
	//find adjacents from both polys and add them to the result poly
	NavMeshPolygon::NavMeshPolyAdjList::iterator nAdjIt;

	uint oi1,oi2;
	math::vector3d midPoint;
	for(int adIt=0;adIt<p1->adjacent.size();++adIt)
	{
		NavMeshPolygon::NavMeshPolyAdj*adPoly=p1->adjacent[adIt];
		if(!adPoly)
			continue;
		if(adPoly->poly!=p2){
			bool r=getAdjEdge(outPoly,adPoly->poly,0,sz1-1,oi2,oi1);
			if(r){
				uint idx1=outPoly->indicies[oi2];
				uint idx2=outPoly->indicies[(oi2+1)%outPoly->indicies.size()];
				//midPoint=(m_verticies[idx1]+m_verticies[idx2])*0.5;
			}else{
				//this is wrong to get here!
				gLogManager.log(mT("Found 2 adjacent polys but cann't find adjacent edge!!"),ELL_WARNING);
				continue;
			}
			NavMeshPolygon::NavMeshPolyAdj*adj1,*adj2;
			adj1=new NavMeshPolygon::NavMeshPolyAdj(adPoly->poly);
			adj2=new NavMeshPolygon::NavMeshPolyAdj(outPoly);

			outPoly->adjacent[oi2]=adj1;
			if(adPoly->poly->adjacent[oi1]){
				adPoly->poly->removeAdjacent(adPoly->poly->adjacent[oi1]->poly);
			}
			adPoly->poly->adjacent[oi1]=adj2;

		}
	}
	for(int adIt=0;adIt<p2->adjacent.size();++adIt)
	{
		NavMeshPolygon::NavMeshPolyAdj*adPoly=p2->adjacent[adIt];
		if(!adPoly)
			continue;
		if(adPoly->poly!=p1){
			bool r=getAdjEdge(outPoly,adPoly->poly,sz1-1,sz2-1,oi2,oi1);
			if(r){
				uint idx1=outPoly->indicies[oi2];
				uint idx2=outPoly->indicies[(oi2+1)%outPoly->indicies.size()];
				//midPoint=(m_verticies[idx1]+m_verticies[idx2])*0.5;
			}else{
				//this is wrong to get here!
				gLogManager.log(mT("Found 2 adjacent polys but cann't find adjacent edge!!"),ELL_WARNING);
				continue;
			}
			NavMeshPolygon::NavMeshPolyAdj*adj1,*adj2;
			adj1=new NavMeshPolygon::NavMeshPolyAdj(adPoly->poly);
			adj2=new NavMeshPolygon::NavMeshPolyAdj(outPoly);

			outPoly->adjacent[oi2]=adj1;
			if(adPoly->poly->adjacent[oi1]){
				adPoly->poly->removeAdjacent(adPoly->poly->adjacent[oi1]->poly);
			}
			adPoly->poly->adjacent[oi1]=adj2;

		}
	}*/
	

	//clean step,remove unwanted verticies which are on the same line
	if(outPoly->canRemoveEdge(0,&m_verticies[0])){
		//before we remove the edge,we have to check for the adjacents!
		//check if we can remove the edge from the adjacent
		bool canRemove=true;
		int idx=(1+e)%sz1;
		if(p1->adjacent[idx]!=0){
			if(p1->adjacent[idx]->poly!=p2){
				int p1Edge=p1->adjacent[idx]->poly->getEdgeIndex(outPoly->indicies[0]);
				if(p1Edge!=-1)//shouldn't get -1
				{
					//same adjacent
					if(p1->adjacent[idx]->poly->canRemoveEdge(p1Edge,&m_verticies[0]))
					{
						p1->adjacent[idx]->poly->removeEdge(p1Edge);
						canRemove=true;
					}else{
						canRemove=false;
					}
				}
			}
		}else if(p2->adjacent[(sz2+e2-2)%sz2]!=0){
			canRemove=false;
		}
		if(canRemove){
			outPoly->removeEdge(0);
			--e2Index;
		}
	}
	if(outPoly->canRemoveEdge(e2Index,&m_verticies[0])){
		int idx=e2;
		bool canRemove=true;
		if(p2->adjacent[idx]!=0){
			if(p2->adjacent[idx]->poly!=p1){
				int p1Edge=p2->adjacent[idx]->poly->getEdgeIndex(outPoly->indicies[e2Index]);
				if(p1Edge!=-1)//shouldn't get -1
				{
					//same adjacent
					if(p2->adjacent[idx]->poly->canRemoveEdge(p1Edge,&m_verticies[0]))
					{
						p2->adjacent[idx]->poly->removeEdge(p1Edge);
						canRemove=true;
					}else{
						canRemove=false;
					}
				}
			}
		}else if(p1->adjacent[(sz1+e-1)%sz1]!=0){
			canRemove=false;
		}
		if(canRemove)
			outPoly->removeEdge(e2Index);
	}
	//calc center point
	outPoly->center=0;
	for (int i=0;i<outPoly->indicies.size();++i)
	{
		outPoly->center+=m_verticies[outPoly->indicies[i]];
	}
	outPoly->center/=outPoly->indicies.size();

	m_polygons.push_back(outPoly);
	resPoly=outPoly;
	return true;
}

bool NavMesh::isConvex(const NavMeshPolygon*p)
{
	uint i1,i2,i3;
	int sz=p->indicies.size();

	math::vector3d*vPtr=&m_verticies[0];
	math::vector3d v;
	for(int i=1;i<sz+1;++i){
		i1=p->indicies[i-1];
		i2=p->indicies[i%sz];
		i3=p->indicies[(i+1)%sz];
		v=(vPtr[i2]-vPtr[i1]).crossProduct(vPtr[i3]-vPtr[i2]);
		v.Normalize();
		if(v.dotProduct(p->plane.Normal)<-math::Epsilon)
			return false;
	}
	return true;
}

NavWorldNodeListPtr::iterator NavMesh::_findPoly(NavMeshPolygon*p){
	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	for(;it!=m_polygons.end();++it){
		if(*it==p){
			break;
		}
	}
	return it;
}


void NavMesh::_copyData(const math::vector3d*vert,int vCount,const void*ind,int indCount,int *attrs,bool is16bitIdx,const math::matrix4x4*transMat)
{
	m_polygons.clear();
	m_verticies.resize(vCount);
	if(!transMat)
		mraySystem::memCopy(&m_verticies[0],vert,vCount*sizeof(math::vector3d));
	else
	{
		math::vector3d *ptr=&m_verticies[0];
		for (int i=0;i<vCount;++i)
		{
			ptr[i]=(*transMat)*vert[i];
		}
	}
	math::vector3d *pVert=&m_verticies[0];

	NavMeshPolygon* p=0;

	m_boundingBox.reset(pVert[0]);

	int triangle=0;
	for(int i=0;i<indCount;i+=3,++triangle)
	{
		if(!p)
			p=new NavMeshPolygon();
		p->setNumberOfEdges(3);
		if(is16bitIdx){
			p->indicies[0]=((ushort*)ind)[i+0];
			p->indicies[1]=((ushort*)ind)[i+1];
			p->indicies[2]=((ushort*)ind)[i+2];
		}else{
			p->indicies[0]=((uint*)ind)[i+0];
			p->indicies[1]=((uint*)ind)[i+1];
			p->indicies[2]=((uint*)ind)[i+2];
		}
		p->boundingbox.reset(pVert[p->indicies[0]]);
		p->boundingbox.AddPoint(pVert[p->indicies[1]]);
		p->boundingbox.AddPoint(pVert[p->indicies[2]]);
		p->center=(vert[p->indicies[0]]+pVert[p->indicies[1]]+pVert[p->indicies[2]])/3;
		p->plane.Normal=(math::Plane(pVert[p->indicies[0]],pVert[p->indicies[1]],pVert[p->indicies[2]])).Normal;

		if(attrs)
			p->attribute=attrs[triangle];

		float dp=p->plane.Normal.dotProduct(math::vector3d::YAxis);
		//exclude polygons which we cann't walk on
		if(dp<0 || p->boundingbox.getHeight()> m_maxStepHeight && dp<m_slope){
			continue;
		}
		m_polygons.push_back(p);
		_onAddPolygon(p);
		m_boundingBox.AddBox(p->boundingbox);
		p=0;
	}
}



void NavMesh::_optimize()
{
	NavMeshPolygon *resPoly;
	bool foundPoly=false;
	math::vector3d midPoint;
	//find 2 polygons share the same edge
	//try to remove this edge and test for convex
	NavWorldNodeListPtr::iterator end=m_polygons.end();
	while(true){
		NavWorldNodeListPtr::iterator it=m_polygons.begin();
		//NavWorldNodeListPtr::iterator it2=it+1;
		//NavMeshPolygon*pp2=*it2;

		foundPoly=false;
		for(;it!=end;++it){
			NavMeshPolygon*p1=(NavMeshPolygon*)*it;

			for(int it2=0;it2<p1->adjacent.size();++it2){
				NavMeshPolygon::NavMeshPolyAdj*p2=p1->adjacent[it2];
				if(!p2)continue;
				if(_MergePolygons(p1,p2->poly,it2,resPoly)==0)continue;
				if(resPoly){

					//remove p1,p2 and add the new poly
					NavMeshPolygon*tmp=p2->poly;
					_onRemovePolygon(tmp);
					_onRemovePolygon(p1);
					m_polygons.erase(it);
					m_polygons.erase(_findPoly(tmp));
					delete p1;
					delete tmp;

					foundPoly=true;
					break;
				}
				
				if(foundPoly)
					break;
			}
			if(foundPoly)
				break;
		}
		if(!foundPoly){
			break;
		}
	}

	//number polys,prepare for clean verticies step,and calculate mid points
	int lastID=-1;

	m_vertPoly.resize(m_verticies.size());
	math::vector3d*vptr=&m_verticies[0];
	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	std::vector<math::vector3d> finalVertices;
	for(;it!=end;++it){
		NavMeshPolygon*p=(NavMeshPolygon*)(*it);
		p->polyID=++lastID;
		p->calcAdjMidPoints(vptr);
		p->plane.recalcD(vptr[p->indicies[0]]);
		for(int i=0;i<p->indicies.size();++i){
			m_vertPoly[p->indicies[i]].push_back(p);
		}
	}
	//calc used vertices
	int usedVertices=0;
	for(int i=0;i<m_vertPoly.size();++i){
		if(m_vertPoly[i].size()!=0)
			++usedVertices;
	}
	//create vertex pool
	finalVertices.resize(usedVertices);
//	finalVertices.set_free_when_destroyed(false);
	for(int i=0,j=0;i<m_vertPoly.size();++i){
		if(m_vertPoly[i].size()!=0){
			finalVertices[j]=m_verticies[i];
			NavWorldNodeListPtr::iterator it=m_vertPoly[i].begin();
			for(;it!=m_vertPoly[i].end();++it){
				NavMeshPolygon*p=(NavMeshPolygon*)*it;
				for(int e=0;e<p->indicies.size();++e){
					if(p->indicies[e]==i){
						p->indicies[e]=j;
						break;
					}
				}
			}
			++j;
		}
	}
	m_verticies=finalVertices;//.set_pointer(&finalVertices[0],finalVertices.size());
	m_vertPoly.clear();
}

void NavMesh::GenerateGraph()
{
	math::vector3d*vtx=&m_verticies[0];
	uint nodeID=0;
	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	for(;it!=m_polygons.end();++it){
		NavMeshPolygon*p=(NavMeshPolygon*)(*it);
		p->createAdjList();

		//create node for center
		p->nodeCenter=new NavGraphNode(nodeID++);
		p->nodeCenter->position=p->center;
		p->nodeCenter->owner=p;
		m_nodes.push_back(p->nodeCenter);
		int adjCnt=0;
		//count the number of adjacents
		for (int i=0;i<p->adjacent.size();++i)
		{
			if(p->adjacent[i]){
				++adjCnt;
			}
		}
		//create nodes for adjacents
		for (int i=0;i<p->adjacent.size();++i)
		{
			if(p->adjacent[i]){
				NavGraphNode*node=0;
				NavMeshPolygon::NavMeshPolyAdj*adj=p->adjacent[i]->poly->getAdjacent(p);
				if(adj->nodeEdge)
					node=adj->nodeEdge;
				else{
					node=adj->nodeEdge=new NavGraphNode(nodeID++);
					node->position=adj->edgeMidpoint;
					m_nodes.push_back(node);
				}

				p->adjacent[i]->nodeEdge=node;
				
				if(adjCnt==1){
					//create edge between edge,center
					NavGraphLink*link=new NavGraphLink(1,p,p->nodeCenter,node,0);
					node->links.push_back(link);
					p->nodeCenter->links.push_back(link);
					m_links.push_back(link);
				}
				
			}
		}
		//uint i11=0,i12=0,i21=0,i22=0;
		//create connection between edges (lattice)
		
		for (int i=0;i<p->adjacent.size();++i)
		{
			NavMeshPolygon::NavMeshPolyAdj*adj1=p->adjacent[i];
			if(!adj1)
				continue;
			
// 			p->getEdge(i,i11,i12);
// 			math::vector3d v1=(vtx[i11]-vtx[i12]);
// 			v1.y=0;
// 			v1.Normalize();
			for (int j=i+1;j<p->adjacent.size();++j)
			{
				NavMeshPolygon::NavMeshPolyAdj*adj2=p->adjacent[j];
				if(!adj2)
					continue;

// 				p->getEdge(j,i21,i22);
// 				math::vector3d v2=(vtx[i21]-vtx[i12]);
// 				v2.y=0;
// 				v2.Normalize();
// 				math::vector3d v3=(vtx[i22]-vtx[i12]);
// 				v3.y=0;
// 				v3.Normalize();
// 
// 				if(abs(v1.dotProduct(v2))>0.8
// 					&& abs(v1.dotProduct(v3))>0.8)
// 					continue;

				//TODO: check if both adjacents lies in the same line,
				//		don't make connection between them in this case..


				NavGraphLink*link=new NavGraphLink(1,p,adj1->nodeEdge,adj2->nodeEdge,0);
				m_links.push_back(link);
				adj1->nodeEdge->links.push_back(link);
				adj2->nodeEdge->links.push_back(link);
			}
		}
	}
	m_nodesArray.resize(m_nodes.size());

	{
		GraphNodesList::iterator it=m_nodes.begin();
		for(;it!=m_nodes.end();++it)
		{
			m_nodesArray[(*it)->getID()]=*it;
		}
	}
}



void NavMesh::GenerateWalls()
{
	//search for nodes which have more than one wall
	m_walls.clear();
	WorldWall wall;
	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	for(;it!=m_polygons.end();++it){
		NavMeshPolygon*p=(NavMeshPolygon*)(*it);
		int adjCnt=p->adjacent.size();
		wall.owner=p;
		for (int i=0;i<adjCnt;++i)
		{
			if(p->adjacent[i]==0)
			{
				wall.wallLine.pStart=m_verticies[p->indicies[i]];
				wall.wallLine.pEnd=m_verticies[p->indicies[(i+1)%adjCnt]];
				m_walls.push_back(wall);
			}
		}
	}
}

void NavMesh::Clear()
{
	{
		NavWorldNodeListPtr::iterator it=m_polygons.begin();
		for(;it!=m_polygons.end();++it)
			delete *it;
		m_polygons.clear();
	}

	{
		GraphLinksList::iterator it=m_links.begin();
		for(;it!=m_links.end();++it)
			delete *it;
		m_links.clear();
	}
	{
		GraphNodesList::iterator it=m_nodes.begin();
		for(;it!=m_nodes.end();++it)
			delete *it;
		m_nodes.clear();
	}

	m_walls.clear();
	m_verticies.clear();
	m_vertPoly.clear();
	m_nodesArray.clear();
	m_grid.Clear();
	m_attributeTranslateMap.clear();
	m_attributeNameIDMap.clear();
}


void NavMesh::LoadAttributeDescFile(const core::string&name)
{
	m_attributeTranslateMap.clear();
	m_attributeNameIDMap.clear();
	m_AttrDescFileName=name;
	OS::IStream*stream=gFileSystem.openFile(name,OS::TXT_READ);
	if(!stream)
		return;
	xml::XMLTree tree;
	if(!tree.load(stream))
		return;
	xml::XMLElement* elem= tree.getSubElement(mT("NavMeshAttrs"));
	if(!elem)
		return;
	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		xml::XMLAttribute*attr;
		attr=e->getAttribute(mT("Value"));
		if(!attr)
			continue;
		int id=core::StringConverter::toInt(attr->value);
		AttributeDesc desc;

		attr=e->getAttribute(mT("ID"));
		if(attr)
			desc.name=attr->value;
		attr=e->getAttribute(mT("Weight"));
		if(attr)
			desc.defaultWeight=core::StringConverter::toFloat(attr->value);
		m_attributeTranslateMap[id]=desc;
		m_attributeNameIDMap[desc.name]=id;
	}
}
core::string NavMesh::GetAttributeName(int id)
{
	std::map<int,AttributeDesc>::iterator it=m_attributeTranslateMap.find(id);
	if(it==m_attributeTranslateMap.end())
		return mT("");
	return it->second.name;
}
float NavMesh::GetAttributeDefaultWeight(int id)
{
	std::map<int,AttributeDesc>::iterator it=m_attributeTranslateMap.find(id);
	if(it==m_attributeTranslateMap.end())
		return 1;
	return it->second.defaultWeight;
}
int NavMesh::GetAttributeID(const core::string& attr)
{
	
	std::map<core::string,int>::iterator it=m_attributeNameIDMap.find(attr);
	if(it==m_attributeNameIDMap.end())
		return 0;
	return it->second;
}


const math::vector3d* NavMesh::GetVerticies(){
	return &m_verticies[0];
}
const NavWorldNodeListPtr& NavMesh::GetWorldNodes(){
	return m_polygons;
}


const GraphNodesList& NavMesh::GetGraphNodes()
{
	return m_nodes;
}
const GraphLinksList& NavMesh::GetGraphLinks()
{
	return m_links;
}

IGraphNode* NavMesh::GetGraphNode(int id)
{
	if(id>=m_nodesArray.size())
		return 0;
	return m_nodesArray[id];
}

const math::box3d& NavMesh::GetBoundingBox()
{
	return m_boundingBox;
}


void NavMesh::OptimizePath(const math::vector3d&start,INavWorldNode*startPoly,
						   const math::vector3d&end,INavWorldNode*endPoly,
				  GraphLinksList&path,PathEdgeList&list)
{
	if(!startPoly || !endPoly)
		return;
	//this will try to smooth out the path by removing edge connections if it can
	list.clear();
	PathEdge currEdge;
	GraphLinksList::iterator it=path.begin();
	NavGraphNode*lastNode=0;
	currEdge.SetAttribute(startPoly->GetAttribute());

	if(it==path.end())
	{
		currEdge.SetEdge(start,end);
		list.push_back(currEdge);
		return;
	}else
	{
		lastNode=(NavGraphNode*)((NavGraphLink*)(*it))->GetNearestNode(start);
		currEdge.SetEnd(lastNode->position);
	}
	bool added=false;
	math::vector3d endP;
	NavGraphNode*currNode;
	NavGraphLink*link;
	NavMeshPolygon*ownerPoly;
	NavMeshPolygon*currPoly=(NavMeshPolygon*)startPoly;
	ownerPoly=currPoly;
	while(true)
	{
		if(it!=path.end()){
			link=(NavGraphLink*)*it;
			currNode=((NavGraphNode*)link->getOtherNode(lastNode));
			endP=currNode->position;
			ownerPoly=link->getOwnerNode();
		}else{
			endP=end;
			ownerPoly=(NavMeshPolygon*)endPoly;
		}

		if(/*ownerPoly->GetAttribute()!=currEdge.attributes ||*/  !lineOfSightTest(currEdge.GetBegin(),currPoly,endP,ownerPoly,0))
		{
			added=true;
		}else
		{
			added=false;
		}
		if(added)
		{
			list.push_back(currEdge);
			currPoly=ownerPoly;
			currEdge.SetBegin(currEdge.GetEnd());
			currEdge.SetAttribute(ownerPoly->GetAttribute());
		}
		currEdge.SetEnd(endP);
		if(it==path.end())
			break;
		lastNode=currNode;
		++it;
	}
	if(!added)
		list.push_back(currEdge);
}

bool NavMesh::lineOfSightTest(const math::vector3d&start,INavWorldNode*startPoly,const math::vector3d&target,
					 INavWorldNode*targetPoly,math::vector3d*intersection)
{
	math::vector2d pstart2D(start.x,start.z);
	math::line2d path(pstart2D.x,pstart2D.y,target.x,target.z);
	NavMeshPolygon* node=(NavMeshPolygon*)startPoly;
	if(!startPoly)
		return false;
	math::vector2d intPt2D;
	math::vector2d targetPt2D(target.x,target.z);
	int e=0;
	math::vector2d dir=path.getVector().Normalize();
	while(node!=targetPoly)
	{
		node=node->classifyPathToPoly(path,&m_verticies[0],e,&intPt2D);
		if(!node || targetPt2D.getDistSQ(intPt2D)<=math::Epsilon){
			break;
		}
		path.pStart=intPt2D+dir*0.01;
	}

	if(intersection){
		//math::vector2d v1,v2;
		math::vector2d t=((intPt2D-pstart2D)/(path.pEnd-pstart2D).Normalize());
		//math::vector2d pp=pstart2D+(path.pEnd-pstart2D).Normalize()*t;
		*intersection=start+(target-start).Normalize()*t.x;
		//(*intersection).set(intPt2D.x,start.y,intPt2D.y);
	}
	if(node)
		return true;
	return false;
}


INavWorldNode* NavMesh::QueryPolys(const math::vector3d&p,NavWorldNodeListPtr&outPoly)
{
	//query polys by there bounding box
	m_grid.QueryPolys(p,outPoly);
//	return;
	//next, more precise search
	//select polys which the point exactly inside them "(X,Z) plane"
	//and choose nearest poly
	NavWorldNodeListPtr::iterator it=outPoly.begin();
	NavWorldNodeListPtr::iterator it2;
	NavWorldNodeListPtr::iterator best=outPoly.end();
	float dist=math::Infinity;
	math::line3d line(p,math::vector3d(p.x,-math::Infinity,p.z));

	math::vector3d inter;
	for(;it!=outPoly.end();){
		it2=it;
		NavMeshPolygon*poly=(NavMeshPolygon*)*it;
		++it2;
		if(!poly->isLineIntersect(line,&m_verticies[0],&inter))
		{
			outPoly.erase(it);
		}else
		{
			float len=(inter-p).LengthSQ();
			if(dist>len){
				best=it;
				dist=len;
			}
		}
		it=it2;
	}
	if(best==outPoly.end())return 0;
	return *best;
}


void NavMesh::Deserialize(const core::string& file)
{
	//File Format
	//|-Header
	//|-Vertices
	//|-Polys
	//	|-indicies
	//	|-adjacents
	//|-Grid

	GCPtr<OS::IStream> stream=gFileSystem.openFile(file,OS::BIN_READ);
	if(!stream)
	{
		gLogManager.log(mT("Couldn't open Navigation mesh file to write: ")+file,ELL_WARNING);
		return;
	}
	m_navMeshFile=file;
	Clear();
	


	NavMeshHeader header;
	PolyDesc pDesc;
	std::vector<AdjDesc> adjArray;
	std::vector<NavMeshPolygon*> PolygonArray;

	stream->read(&header,sizeof(header));
	if(header.magic!=HEADER_MAGIC_NUMBER){
		gLogManager.log(mT("NavMesh::Serialize()-unkown file format"),ELL_WARNING);
		return;
	}
	m_boundingBox=header.bbox;
	m_slope=header.slope;
	m_maxStepHeight=header.maxStepHeight;
	m_grid.SetCellSize(header.gridSize);
	m_grid.SetGrindBounds(m_boundingBox);
	m_grid.CreateGrid();


	m_verticies.resize(header.numberOfVertices);
	stream->read(&m_verticies[0],header.numberOfVertices*sizeof(math::vector3d));

	//create polys
	PolygonArray.resize(header.numberOfPolygons);
	for(int i=0;i<header.numberOfPolygons;++i){
		PolygonArray[i]=new NavMeshPolygon();
		m_polygons.push_back(PolygonArray[i]);
	}

	for(int i=0;i<header.numberOfPolygons;++i){
		stream->read(&pDesc,sizeof(pDesc));
		PolygonArray[pDesc.id]->polyID=pDesc.id;
		PolygonArray[pDesc.id]->boundingbox=pDesc.bbox;
		PolygonArray[pDesc.id]->center=pDesc.center;
		PolygonArray[pDesc.id]->plane=pDesc.plane;
		PolygonArray[pDesc.id]->attribute=pDesc.attribute;
		PolygonArray[pDesc.id]->setNumberOfEdges(pDesc.indexCount);
		stream->read(&PolygonArray[pDesc.id]->indicies[0],
			pDesc.indexCount*sizeof(uint));
		//read adjacent.
		adjArray.resize(pDesc.adjCount);
		stream->read(&adjArray[0],
			pDesc.adjCount*sizeof(AdjDesc));
		for (int j=0;j<pDesc.adjCount;++j)
		{
			NavMeshPolygon::NavMeshPolyAdj *adj=new NavMeshPolygon::NavMeshPolyAdj();
			adj->poly=PolygonArray[adjArray[j].pId];
			adj->edgeMidpoint=adjArray[j].edgeMidpoint;
			PolygonArray[pDesc.id]->adjacent[adjArray[j].edgeId]=adj;
		}
	}
	std::vector<int> polyIDArray;
	for(int i=0;i<header.cellsCount;++i){
		GridCellDesc cellDesc;
		stream->read(&cellDesc,sizeof(GridCellDesc));
		polyIDArray.resize(cellDesc.polyCount);
		if(cellDesc.polyCount)
			stream->read(&polyIDArray[0],polyIDArray.size()*sizeof(int));
		for(int j=0;j<cellDesc.polyCount;++j){
			m_grid.addToCell(i,PolygonArray[polyIDArray[j]]);
		}
	}
	GenerateGraph();
	GenerateWalls();
	stream->close();
}
void NavMesh::Serialize(OS::IStream* stream)
{

	NavMeshHeader header;
	header.magic=HEADER_MAGIC_NUMBER;
	header.bbox=m_boundingBox;
	header.cellsCount=m_grid.getCellsCount();
	header.gridSize=m_grid.GetCellSize();
	header.maxStepHeight=m_maxStepHeight;
	header.slope=m_slope;
	header.numberOfVertices=m_verticies.size();
	header.numberOfPolygons=m_polygons.size();

	PolyDesc pDesc;
	AdjDesc adjDesc;

	stream->write(&header,sizeof(header));
	if(header.numberOfVertices)
		stream->write(&m_verticies[0],header.numberOfVertices*sizeof(math::vector3d));

	NavWorldNodeListPtr::iterator it=m_polygons.begin();
	for(;it!=m_polygons.end();++it){
		NavMeshPolygon*p=(NavMeshPolygon*)*it;
		pDesc.id=p->polyID;
		pDesc.bbox=p->boundingbox;
		pDesc.center=p->center;
		pDesc.plane=p->plane;
		pDesc.indexCount=p->indicies.size();
		pDesc.attribute=p->attribute;

		pDesc.adjCount=0;
		for (int i=0;i<p->adjacent.size();++i)
		{
			if(p->adjacent[i])
				++pDesc.adjCount;
		}
		stream->write(&pDesc,sizeof(pDesc));
		if(pDesc.indexCount)
			stream->write(&p->indicies[0],pDesc.indexCount*sizeof(uint));
		for (int i=0;i<p->adjacent.size();++i)
		{
			if(p->adjacent[i]){
				adjDesc.edgeId=i;
				adjDesc.edgeMidpoint=p->adjacent[i]->edgeMidpoint;
				adjDesc.pId=p->adjacent[i]->poly->polyID;
				stream->write(&adjDesc,sizeof(AdjDesc));
			}
		}
	}
	std::vector<int> polyIDArray;
	for(int i=0;i<header.cellsCount;++i){
		GridCellDesc cellDesc;
		NavWorldNodeListPtr &pList= m_grid.getPolygonsInCell(i);
		cellDesc.polyCount=pList.size();
		stream->write(&cellDesc,sizeof(GridCellDesc));
		NavWorldNodeListPtr::iterator it=pList.begin();
		for(;it!=pList.end();++it){
			NavMeshPolygon*p=(NavMeshPolygon*)*it;
			stream->write(&p->polyID,sizeof(int));
		}
	}
}

const std::vector<WorldWall>& NavMesh::GetWorldWalls()
{
	return m_walls;
}

IWorldVisualizer* NavMesh::GetVisualizer()
{
	return m_visualizer;
}

IGraphSearch* NavMesh::CreatePathFinder()
{
	return new NavAStarSearch(this);
}


ICostFunction* NavMesh::CreateHeuristicFunction()
{
	return new NavMeshHeuristic();
}

void NavMesh::Visualize(IRenderDevice*dev)
{
	//m_visualizer->DrawWorld(dev);
	m_visualizer->DrawNodes(dev,m_polygons,false);
	m_visualizer->DrawLinks(dev,m_links);
	/*
	for (int i=0;i<m_walls.size();++i)
		{
			dev->drawLine(m_walls[i].wallLine+math::vector3d(0,4,0),video::SColor(255,255,0,255));
		}*/
	
}


void NavMesh::loadXMLSettings(xml::XMLElement* e)
{
	xml::XMLAttribute*attr;
	attr=e->getAttribute(mT("File"));
	if(attr)
		Deserialize(attr->value);
	attr=e->getAttribute(mT("DescFile"));
	if(attr)
		LoadAttributeDescFile(attr->value);
}
xml::XMLElement*  NavMesh::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("NavigationMesh"));
	elem->addSubElement(e);

	e->addAttribute(mT("File"),m_navMeshFile);
	if(m_AttrDescFileName!=mT(""))
		e->addAttribute(mT("DescFile"),m_AttrDescFileName);

	return e;
}

}
}

