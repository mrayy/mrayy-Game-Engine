#include "stdafx.h"

#include "IGeometryBatch.h"




namespace mray{
namespace scene{

IGeometryBatch::IGeometryBatch(){

}
IGeometryBatch::~IGeometryBatch(){

}

bool IGeometryBatch::addNode(SMesh* meshBuffer,IMovable* node){
	InstanceData inst;
	inst.meshBuffer=meshBuffer;
	inst.node=node;
	m_instances.push_back(inst);
	return true;
}

void IGeometryBatch::clearAll(){
	m_instances.clear();
}
/**/

uint IGeometryBatch::build(){
	return 0;
}

void IGeometryBatch::update(){

}


}
}
