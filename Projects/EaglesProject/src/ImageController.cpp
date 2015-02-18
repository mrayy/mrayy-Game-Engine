

#include "ImageController.h"

#include <Randomizer.h>


namespace mray{
namespace gameMod{

ImageController::ImageController(){
	m_currentImage=-1;

	m_effector=new GUI::GUIFadeEffector(1,1,80,2);
	m_anchor=new GUI::GUIPercentAnchorEff(GUI::EPAnchor_UL,0);

}

ImageController::~ImageController(){
}

void ImageController::addImage(GCPtr<GUI::GUIRotImage> image){
	m_images.push_back(image);
	image->Visible=0;
}

void ImageController::update(float dt){
	if(m_images.size()==0)
		return;
	if(m_currentImage==-1){
		m_currentImage=0;
		m_images[0]->addEffector(m_effector);
		m_images[0]->addEffector(m_anchor);
		m_images[m_currentImage]->Visible=1;
		m_effector->reset();
		m_anchor->PosPercent().x=math::Randomizer::rand01();
		m_anchor->PosPercent().y=math::Randomizer::rand01();

		m_images[0]->Angle=math::Randomizer::randRange(-40,40);
	}
	if(m_effector->isDone()){
		m_images[m_currentImage]->removeEffector(m_effector);
		m_images[m_currentImage]->removeEffector(m_anchor);
		m_images[m_currentImage]->Visible=0;
		m_currentImage++;
		if(m_currentImage>=m_images.size()){
			m_currentImage=0;
		}
		m_images[m_currentImage]->addEffector(m_effector);
		m_images[m_currentImage]->addEffector(m_anchor);
		m_images[m_currentImage]->Visible=1;
		m_effector->reset();
		m_anchor->PosPercent().x=math::Randomizer::rand01()*0.8+0.1;
		m_anchor->PosPercent().y=math::Randomizer::rand01()*0.8+0.1;

		m_images[m_currentImage]->Angle=math::Randomizer::randRange(-40,40);
	}
}

void ImageController::shuffle(){
	m_currentImage=-1;
	for (int i=0;i<m_images.size()-1;i++)
	{
		int id=math::Randomizer::rand(m_images.size()-i-1);
		math::Swap(m_images[i],m_images[id]);
		m_images[i]->Visible=false;
	}
}

}
}


