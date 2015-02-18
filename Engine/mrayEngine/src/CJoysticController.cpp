#include "stdafx.h"
#include "CJoysticController.h"



namespace mray{



namespace controllers{


CJoysticController::CJoysticController(){
}
CJoysticController::~CJoysticController(){
	m_buttons.clear();
	m_buttons.clear();
	m_Axis.clear();
}

void CJoysticController::reset(){
	for(int i=0;i<m_buttons.size();++i){
		m_buttons[i]=0;
	}
	for(int i=0;i<m_Axis.size();++i){
		m_Axis[i]=JoysticAxis();
	}
	for(int i=0;i<m_Vectors.size();++i){
		m_Vectors[i]=0;
	}
	for(int i=0;i<4;++i){
		m_Pov[i]=JoysticPov();
		m_sliders[i]=JoysticSlider();
	}
}

bool CJoysticController::getButtonState(int index){
	if(index<0 || index>=m_buttons.size())return 0;
	return m_buttons[index];
}
void CJoysticController::setButtonState(int index,bool v){
	if(index<0 || index>=m_buttons.size())return;
	m_buttons[index]=v;
}

void CJoysticController::copyButtonState(bool*v,int cnt){
	for(int i=0;i<cnt;++i)
		m_buttons[i]=v[i];
}

JoysticAxis CJoysticController::getAxisState(int index){
	if(index<0 || index>=m_Axis.size())return JoysticAxis();
	return m_Axis[index];
}
void CJoysticController::setAxisState(int index,JoysticAxis state){
	if(index<0 || index>=m_Axis.size())return;
	m_Axis[index]=state;
}

math::vector3d CJoysticController::getVectorState(int index){
	if(index<0 || index>=m_Vectors.size())return 0;
	return m_Vectors[index];
}
void CJoysticController::setVectorState(int index,math::vector3d state){
	if(index<0 || index>=m_Vectors.size())return;
	m_Vectors[index]=state;
}

JoysticPov CJoysticController::getPovState(int index){
	if(index<0 || index>=4)return JoysticPov();
	return m_Pov[index];
}
void CJoysticController::setPovState(int index,JoysticPov state){
	if(index<0 || index>=4)return;
	m_Pov[index]=state;
}

JoysticSlider CJoysticController::getSliderState(int index){
	if(index<0 || index>=4)return JoysticSlider();
	return m_sliders[index];
}
void CJoysticController::setSliderState(int index,JoysticSlider state){
	if(index<0 || index>=4)return ;
	m_sliders[index]=state;
}

}
}