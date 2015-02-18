#include "stdafx.h"



#include "STerrain.h"
#include "IVideoDevice.h"

#include "ITexture.h"
#include "ISceneManager.h"
#include "ILogManager.h"
#include "TextureResourceManager.h"
#include "MeshGenerator.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{


STerrain::STerrain():terrMesh(0)
{
}

STerrain::~STerrain()
{
}

bool STerrain::loadTerrain(const  mchar*fileName,TERR_PATCH_SIZE patchSz)
{
	
	video::ITexturePtr  heightMap=gTextureResourceManager.loadTexture2D(fileName);

	if(!heightMap)
	{
		gLogManager.log(mT("unable to load Terrain heightmap"),fileName,ELL_WARNING);
		return 0;
	}

	if(heightMap->getColorDepth()!=8) //Not Grayscale
	{
		gLogManager.log(mT("Terrain heightmap should be Grayscale"),fileName,ELL_WARNING);
		
		return 0;
	}

	patchSize=patchSz;

	MeshGenerator::getInstance().generateTerrain(heightMap,(int)patchSize,terrMesh);
	return 1;
}

void STerrain::setTexture(const  mchar* texName,int layer)
{
	getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(gTextureResourceManager.loadTexture2D(texName),layer);
}

void STerrain::setTexture(video::ITexturePtr   tex,int layer)
{
	getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,layer);
	
}


float STerrain::getHeight(float x,float z,math::vector3d *outNormal)
{

	float height=0;
	//calc the real position

	math::vector2d pos(x,z);

	pos.x-=m_owner->getPosition().x;
	pos.y-=m_owner->getPosition().z;
	pos.x/=m_owner->getScale().x;
	pos.y/=m_owner->getScale().z;

// 	pos.x-=terrMesh->getBuffer(0)->getPosition().x;
// 	pos.y-=terrMesh->getBuffer(0)->getPosition().z;

// 	pos.x/=terrMesh->getBuffer(0)->getScale().x;
// 	pos.y/=terrMesh->getBuffer(0)->getScale().z;

	int px=pos.x;
	int pz=pos.y;

	if(px<0 || px>=vertPerEdge-1 ||
		pz<0 || pz>=vertPerEdge-1 )
		return 0 ;// out of bounds


	IMeshBuffer*buffer=terrMesh->getBuffer(0);
	video::IHardwareStreamBuffer* vStream=buffer->getStream(0,video::EMST_Position);
	math::vector3d* vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	float v1=vert[(px)*vertPerEdge+pz].y;
	float v2=vert[((px+1))*vertPerEdge+pz].y;
	float v3=vert[(px)*vertPerEdge+(pz+1)].y;
	float v4=vert[((px+1))*vertPerEdge+(pz+1)].y;

	float dx=pos.x-px;
	float dz=pos.y-pz;

	float AlphaX=1-dx;
	float AlphaZ=1-dz;

	height=v1*(AlphaX)*(AlphaZ)+v2*(dx)*(AlphaZ)+
		v3*(AlphaX)*(dz)+v4*(dx)*(dz);

//	height=v1;
	height*=m_owner->getScale().y;
//	height*=terrMesh->getBuffer(0)->getScale().y;

	height+=m_owner->getPosition().y;
//	height+=terrMesh->getBuffer(0)->getPosition().y;

	return height;
}


void STerrain::setVertexPos(int vertex,math::vector3d pos)
{
	IMeshBuffer*buffer=terrMesh->getBuffer(0);
	video::IHardwareStreamBuffer* vStream=buffer->getStream(0,video::EMST_Position);
	if(vertex>=vStream->length())return;
	math::vector3d* vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	pos-=m_owner->getPosition();
	pos/=m_owner->getScale();

//	pos-=buffer->getPosition();
//	pos/=buffer->getScale();
	vert[vertex]=pos;
}

int STerrain::getVertexFromPos(math::vector3d Pos)
{
	math::vector3d pos(Pos);

	pos-=m_owner->getPosition();
	pos/=m_owner->getScale();

// 	pos-=terrMesh->getBuffer(0)->getPosition();
//	pos/=terrMesh->getBuffer(0)->getScale();

	int px=pos.x;
	int pz=pos.z;

	if(px<0 || px>=vertPerEdge ||
		pz<0 || pz>=vertPerEdge )
		return 0 ;// out of bounds

	return px*vertPerEdge+pz;
}

void STerrain::setPosition(const math::vector3d&  v){
}
void STerrain::setRotation(const math::vector3d&  v){
}
void STerrain::setScale(const math::vector3d&  v){
}




void STerrain::setMaterial(const video::RenderMaterialPtr& mtrl,int i){
	terrMesh->getBufferData(0)->setMaterial(mtrl);
}
const video::RenderMaterialPtr&  STerrain::getMaterial(int i){
	return terrMesh->getBufferData(0)->getMaterial();
}

}
}
