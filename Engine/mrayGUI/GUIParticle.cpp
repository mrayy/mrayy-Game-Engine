

#include "GUIParticle.h"
#include "GUIParticleSystem.h"
#include "GUIManager.h"

namespace mray{
namespace GUI{


GUIParticle::GUIParticle(GUIParticleSystem*system)
:GUIElement(system->getManager()),pSystem(system),bAlive(0),lifeSpawn(0)
{
#ifdef ___DEBUG___
	setDebugName("GUIParticle");
#endif
	velocity=0;
	acceleration=0;
	force=0;
	m_lastRot=0;
}


GUIParticle::~GUIParticle()
{
}

void GUIParticle::draw(float dt)
{
	if(!bAlive)return;
	GCPtr<GUIRotImage> nShape=0;
	nShape=pSystem->getShapeNode();


	if(nShape){
		nShape->Angle=m_lastRot;
		nShape->MainPos=getPos();
		nShape->MainSize=getSize();
		nShape->Color=color;
		nShape->draw(dt);
	}
	else//just a point
	{
		//TODO
	}
	lifeSpawn-=dt;
	if(lifeSpawn<=0)
	{
		lifeSpawn=0;
		bAlive=0;
		pSystem->addDeadParticle(this);
		return;
	}
	R+=colorDT.x*dt;
	G+=colorDT.y*dt;
	B+=colorDT.z*dt;
	A+=AlphaDT*dt;
	color=video::SColor(R,G,B,A);
	if(color.A<=4){
		int i=0;
	}

	MainPos=MainPos()+velocity*dt;
	velocity+=(acceleration+force)*dt;

	m_lastRot+=rotSpeed*dt;

	MainSize=MainSize()+sizeDT*dt;

	GUIElement::draw(dt);

}








}
}
