
#include "TreeEntity.h"

namespace mray{
namespace gameMod{

TreeEntity::TreeEntity(GCPtr<scene::ISceneNode> shape)
	:StaticEntity(shape)
{
	m_exploded=false;
}
TreeEntity::~TreeEntity(){
}

void TreeEntity::hasCollide(){
	if(m_exploded)
		return;
	m_exploded=true;
	m_passable=true;
	m_shape->Visible=false;
}

}
}

