

#include "Sun.h"



namespace mray{
namespace gameMod{

Sun::Sun(){
}
Sun::~Sun(){
}

void Sun::setColor(const video::SColor &clr){
	m_color=clr;
}
const video::SColor& Sun::getColor(){
	return m_color;
}

void Sun::attachLight(GCPtr<scene::SLightNode>light){
	m_light=light;
}

GCPtr<scene::SLightNode> Sun::getAttachedLight(){
	return m_light;
}


void Sun::update(float dt){

}

}
}

