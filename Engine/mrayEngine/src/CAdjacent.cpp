
#include "stdafx.h"
#include "CAdjacent.h"


namespace mray{
namespace scene{

CAdjacent::CAdjacent(){
}
CAdjacent::~CAdjacent(){
}

void CAdjacent::calcAdjacent(ushort*idx,uint count){
	reset();
	generateEdges(idx,count);
	generateTriangles(idx,count);
}

void CAdjacent::reset(){
	m_edgeMap.clear();
	m_TriMap.clear();
	m_edges.clear();
	m_triangles.clear();
}

const shortSet& CAdjacent::getEdgeAdjacent(ushort i){
	return m_edgeMap[i];
}
const shortSet& CAdjacent::getTriAdjacent(ushort i){
	return m_TriMap[i];
}

void CAdjacent::generateEdges(ushort*idx,uint count){
	for (short i=0;i<count;i+=3)
	{
		Index3 ix(idx[i],idx[i+1],idx[i+2]);
		ushort xyz[3]={0};
		getEdgesIncr(ix,xyz);
		Edge e;
		e.i1=xyz[0];
		e.i2=xyz[1];
		m_edgeMap[e.i1].insert(e.i2);
		m_edgeMap[e.i2].insert(e.i1);
		e.i2=xyz[2];
		m_edgeMap[e.i1].insert(e.i2);
		m_edgeMap[e.i2].insert(e.i1);
		e.i1=xyz[1];
		m_edgeMap[e.i1].insert(e.i2);
		m_edgeMap[e.i2].insert(e.i1);
	}

	shortSetMap::iterator it=m_edgeMap.begin();
	Edge e;
	for (;it!=m_edgeMap.end();++it)
	{
		e.i1=it->first;
		shortSet&s=it->second;
		shortSet::iterator it2=s.begin();
		for (;it2!=s.end();it2++)
		{
			e.i2=*it2;
			if(e.i1<e.i2){
				m_edgeIDMap[HashEdge(e.i1,e.i2)]=m_edges.size();
				m_edges.push_back(e);
			}
		}
	}
}

void CAdjacent::generateTriangles(ushort*idx,uint count){

	Triangle t;
	for (short i=0;i<count;i+=3)
	{
		Index3 ix(idx[i],idx[i+1],idx[i+2]);
		ushort xyz[3]={0};
		getEdgesIncr(ix,xyz);
		uint h=HashEdge(xyz[0],xyz[1]); 
		t.e1=m_edgeIDMap[h];
		h=HashEdge(xyz[0],xyz[2]); 
		t.e2=m_edgeIDMap[h];
		h=HashEdge(xyz[1],xyz[2]); 
		t.e3=m_edgeIDMap[h];

		m_triangles.push_back(t);
	}
	int ent=0;
	for (int i=0;i<m_triangles.size();i++)
	{
		Triangle &t=m_triangles[i];
		for (int j=i+1;j<m_triangles.size();j++)
		{
			Triangle &t2=m_triangles[j];
			if( t.e1==t2.e1 || 
				t.e1==t2.e2 || 
				t.e1==t2.e3 || 

				t.e2==t2.e1 || 
				t.e2==t2.e2 || 
				t.e2==t2.e3 ||

				t.e3==t2.e1 || 
				t.e3==t2.e2 || 
				t.e3==t2.e3  )
			{
				m_TriMap[i].insert(j);
				m_TriMap[j].insert(i);
				ent++;
			}
		}
	}
}


void CAdjacent::getEdgesIncr(const CAdjacent::Index3&ix,ushort xyz[3]){
	if(ix.i1<ix.i2){
		if(ix.i1<ix.i3){
			xyz[0]=ix.i1;
			if(ix.i2<ix.i3){
				xyz[1]=ix.i2;
				xyz[2]=ix.i3;
			}else{
				xyz[1]=ix.i3;
				xyz[2]=ix.i2;
			}
		}else{
			xyz[0]=ix.i3;
			xyz[1]=ix.i1;
			xyz[2]=ix.i2;
		}
	}else //(ix.i2<ix.i1)
	{
		if(ix.i2<ix.i3){
			xyz[0]=ix.i2;
			if(ix.i1<ix.i3){
				xyz[1]=ix.i1;
				xyz[2]=ix.i3;
			}else{
				xyz[1]=ix.i3;
				xyz[2]=ix.i1;
			}
		}else{
			xyz[0]=ix.i3;
			xyz[1]=ix.i2;
			xyz[2]=ix.i1;
		}
	}
}

}
}