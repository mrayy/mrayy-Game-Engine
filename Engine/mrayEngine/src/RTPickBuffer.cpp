#include "stdafx.h"

#include "RTPickBuffer.h"
#include "IVideoDevice.h"

#include "MeshBufferData.h"
#include "IRenderable.h"
#include "ISceneNode.h"

namespace mray
{
namespace video
{

RTPickBuffer::RTPickBuffer(IVideoDevice* device,const math::vector2di& rtSize,const math::vector2di& vpSize):m_mtrl(0)
{
	m_device=device;
	SetViewportSize(vpSize);
	SetRenderTargetSize(rtSize);

	m_mtrl.setRenderState(RS_Lighting,ES_DontUse);
	m_mtrl.setRenderState(RS_Wireframe,ES_DontUse);
	m_mtrl.setRenderState(RS_Smooth,ES_DontUse);
	m_mtrl.setRenderState(RS_CullFace,ES_DontUse);
	m_mtrl.setRenderState(RS_Blend,ES_DontUse);

}
RTPickBuffer::~RTPickBuffer()
{
	Clear();
	m_rtTex=0;
	m_rt=0;
}


void RTPickBuffer::SetViewportSize(const math::vector2di& vpSize)
{
	m_vpSize=vpSize;
}

void RTPickBuffer::SetRenderTargetSize(const math::vector2di& rtSize)
{
	m_rtTex=m_device->createEmptyTexture2D(true);
	m_rtTex->setMipmapsFilter(false);
	m_rtTex->setBilinearFilter(false);
	m_rtTex->setTrilinearFilter(false);
	m_rtTex->setAnisotropicFilter(false);
	m_rtTex->createTexture(math::vector3d(rtSize.x,rtSize.y,1),EPixel_R8G8B8A8);
	m_rt=m_device->createRenderTarget(mT(""),m_rtTex,0,0,0);
	m_vpRatio.x=((float)rtSize.x)/((float)m_vpSize.x);
	m_vpRatio.y=((float)rtSize.y)/((float)m_vpSize.y);
}

void RTPickBuffer::Clear()
{
	while(!m_objects.empty())
	{
		m_objects.pop();
	}
}

void RTPickBuffer::SetViewMatrix(const math::matrix4x4& m)
{
	m_viewMatrix=m;
}
void RTPickBuffer::SetProjectionMatrix(const math::matrix4x4& m)
{
	m_projMatrix=m;
}

void RTPickBuffer::PushPickID(uint id)
{
	m_objects.push(IDRenderObject(id));
}

void RTPickBuffer::AddRenderObject(scene::MeshBufferData*bd,scene::IRenderable*obj)
{
	if(m_objects.empty())
		return;
	m_objects.top().objs.push_back(IDRenderObject::ROperation(obj,bd));
}

void RTPickBuffer::PopPickID()
{
	if(m_objects.empty()==0)
		return;
	m_objects.pop();
}

void RTPickBuffer::Render()
{
	math::matrix4x4 oldProj,oldView;
	IHardwarePixelBuffer* pixels= m_rtTex->getSurface(0);
	if(pixels->isLocked())
		pixels->unlock();

	m_device->set3DMode();

	m_device->getTransformationState(TS_PROJECTION,oldProj);
	m_device->getTransformationState(TS_VIEW,oldView);

	m_device->setTransformationState(TS_PROJECTION,m_projMatrix);
	m_device->setTransformationState(TS_VIEW,m_viewMatrix);

	bool colorState=m_device->isStreamEnabled(EMST_Color);
	bool normalState=m_device->isStreamEnabled(EMST_Normal);
	bool texState=m_device->isStreamEnabled(EMST_Texcoord);
	m_device->enableStream(EMST_Color,false);
	m_device->enableStream(EMST_Normal,false);
	m_device->enableStream(EMST_Texcoord,false);

	video::StreamsData*sdata=0;
	m_device->useShader(0);

	m_device->setRenderTarget(m_rt,true,true,video::SColor(0,0,0,0));

	video::SColor clr;
	IDObjectsMap::container_type::const_iterator oit=m_objects._Get_container().begin();
	for(;oit!=m_objects._Get_container().end();++oit)
	{
		std::vector<IDRenderObject::ROperation>::const_iterator it=(*oit).objs.begin();

		clr.setColorARGB((*oit).id);
		m_mtrl.SetDiffuse(clr);
		m_mtrl.SetAmbient(clr);
		m_mtrl.SetEmissive(clr);
		m_mtrl.SetAlpha(clr.A);
		//m_mtrl.SetDiffuse(video::SColor(1,1,1,1));//
		m_device->useRenderPass(&m_mtrl);

		for(;it!=(*oit).objs.end();++it){
			scene::MeshBufferData*bd=(*it).bd;
			scene::IRenderable*node=(*it).node;
			scene::IMeshBuffer* mb=bd->getMeshBuffer();

			if(mb->getStreamsData().pointer()!=sdata){
				sdata=mb->getStreamsData();
				m_device->setStreamMap(sdata);
			}
	/*
	
			if(bd->getMaterial()!=mtrl){
				mtrl=bd->getMaterial();
				c.setConstants(mtrl->shader,video::IShaderConstantsCallback::ESUT_Global);
			}
			bd->getShaderCallback()->setConstants(mtrl->shader,video::IShaderConstantsCallback::ESUT_Object);
			*/



			m_device->setTransformationState(video::TS_WORLD,node->getOwner()->getAbsoluteTransformation());

			m_device->drawElements(mb->getIndexData(),mb->getRenderType());
		}
	}
	m_device->setRenderTarget(0);
	m_device->enableStream(EMST_Color,colorState);
	m_device->enableStream(EMST_Normal,normalState);
	m_device->enableStream(EMST_Texcoord,texState);

	m_device->setTransformationState(TS_PROJECTION,oldProj);
	m_device->setTransformationState(TS_VIEW,oldView);

	math::box3d lockBox(0,m_rtTex->getSize());
	m_lockedBox= pixels->lock(lockBox,IHardwarePixelBuffer::ELO_ReadOnly);

	return;
}

uint RTPickBuffer::GetID(int x,int y)
{
	if(x<0 || y<0 )
		return 0;
	int pX=x*m_vpRatio.x;
	int pY=y*m_vpRatio.y;
	struct RGBPtr
	{
		byte r,g,b;
	};
	RGBA* ptr=(RGBA*)m_lockedBox.data;

	if(pX>=m_lockedBox.box.getWidth() || pY>=m_lockedBox.box.getHeight() )
		return 0;

	pY=m_lockedBox.box.getHeight()-pY-1;

	int offset=pY*m_rtTex->getSize().x+pX;
	ptr+=offset;

	IDObjectsMap::container_type::const_iterator oit=m_objects._Get_container().begin();
	for(;oit!=m_objects._Get_container().end();++oit)
	{
		if(*ptr==(*oit).id)
			return *ptr;
	}
	return 0;
	
	//const SColor&c= (*ptr);

//	uint v=ptr->b<<16+ptr->g<<8+ptr->r;
//	return v;
	return (*ptr);//c.getColorRGBA(); 
}

}
}
