#include "stdafx.h"



#include "CMouseController.h"
#include "mraySystem.h"

namespace mray{
	
namespace controllers{

CMouseController::CMouseController(){
	mraySystem::memSet(m_pressed,0,sizeof(m_pressed));
}

CMouseController::~CMouseController(){
}


EMouseButton CMouseController::getButton(){
	return m_button;
}
bool CMouseController::isPressed(EMouseButton button){
	return m_pressed[button];
}

void CMouseController::setPos(math::vector3d p){
	m_dpos=p-m_pos;
	m_pos=p;
}
void CMouseController::setDPos(math::vector3d p){
	m_dpos=p;
}

math::vector3d CMouseController::getPos(){
	return m_pos;
}
math::vector3d CMouseController::getDPos(){
	return m_dpos;
}

int CMouseController::getX(){
	return m_pos.x;
}
int CMouseController::getY(){
	return m_pos.y;
}
int CMouseController::getZ(){
	return m_pos.z;
}

float CMouseController::getDX(){
	return m_dpos.x;
}
float CMouseController::getDY(){
	return m_dpos.y;
}
float CMouseController::getDZ(){
	return m_dpos.z;
}


void CMouseController::setButton(EMouseButton b){
	m_button=b;
}
void CMouseController::setPressed(EMouseButton button,bool p){
	m_button=button;
	m_pressed[button]=p;
}

void CMouseController::setX(int v){
	m_dpos.x=m_pos.x-v;
	m_pos.x=v;
}
void CMouseController::setY(int v){
	m_dpos.y=m_pos.y-v;
	m_pos.y=v;
}
void CMouseController::setZ(int v){
	m_dpos.z=m_pos.z-v;
	m_pos.z=v;
}

void CMouseController::setDX(float v){
	m_dpos.x=v;
}
void CMouseController::setDY(float v){
	m_dpos.y=v;
}
void CMouseController::setDZ(float v){
	m_dpos.z=v;
}

}
}


