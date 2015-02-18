#include "stdafx.h"


#include "GLPickBuffer.h"

#include "IRenderable.h"
#include "MeshBufferData.h"
#include "GLDev.h"

#include "ISceneNode.h"


namespace mray
{
namespace video
{

GLPickBuffer::GLPickBuffer(mrayDev*device)
{
	m_device=device;
}
GLPickBuffer::~GLPickBuffer()
{
	Clear();
}

void GLPickBuffer::Clear()
{
	while(!m_objects.empty())
	{
		m_objects.pop();
	}
}

void GLPickBuffer::SetViewMatrix(const math::matrix4x4& m)
{
	m_viewMatrix=m;
}
void GLPickBuffer::SetProjectionMatrix(const math::matrix4x4& m)
{
	m_projMatrix=m;
}


void GLPickBuffer::PushPickID(uint id)
{
	m_objects.push(IDRenderObject(id));
}

void GLPickBuffer::AddRenderObject(scene::MeshBufferData*bd,scene::IRenderable*obj)
{
	if(m_objects.empty())
		return;
	m_objects.top().objs.push_back(IDRenderObject::ROperation(obj,bd));
}

void GLPickBuffer::PopPickID()
{
	if(m_objects.empty()==0)
		return;
	m_objects.pop();
}

void GLPickBuffer::Render()
{
	math::matrix4x4 oldProj,oldView;


	glSelectBuffer(SELECT_BUFFER_SIZE,m_selectBuffer);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0) ;
	
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
	video::RenderMaterial* mtrl=0;
	video::IShaderConstantsCallback c;
	m_device->useRenderPass(0);

	IDObjectsMap::container_type::const_iterator oit=m_objects._Get_container().begin();
	for(;oit!=m_objects._Get_container().end();++oit)
	{
		std::vector<IDRenderObject::ROperation>::const_iterator it=(*oit).objs.begin();
		glLoadName((*oit).id);
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
				m_device->useMaterial(mtrl);
				c.setConstants(mtrl->shader,video::IShaderConstantsCallback::ESUT_Global);
			}
			bd->getShaderCallback()->setConstants(mtrl->shader,video::IShaderConstantsCallback::ESUT_Object);
			*/
			

			m_device->setTransformationState(video::TS_WORLD,node->getOwner()->getAbsoluteTransformation());

			m_device->drawElements(mb->getIndexData(),mb->getRenderType());
		}
	}
	m_device->enableStream(EMST_Color,colorState);
	m_device->enableStream(EMST_Normal,normalState);
	m_device->enableStream(EMST_Texcoord,texState);

	m_device->setTransformationState(TS_PROJECTION,oldProj);
	m_device->setTransformationState(TS_VIEW,oldView);

	m_hitsCount=glRenderMode(GL_RENDER);
	//return ProcessHits();
}



uint GLPickBuffer::ProcessHits()
{
	uint id=0;
	float oldZ=math::Infinity;
	uint*ptr=m_selectBuffer;
	for (int i=0;i<m_hitsCount;++i)
	{
		//each hit records 3 values+array (# of names for the hit) -minZ-maxZ- array of hit Name
		uint names=*ptr;++ptr;
		uint z1=*ptr;++ptr;
		uint z2=*ptr;++ptr;
		if(oldZ>z1)
			id=*ptr;//++ptr;
		ptr+=names;
		/*
		for(uint j=0;j<names;++j){
			uint nameID=*ptr;++ptr;
		}*/
	}
	return id;
}

uint GLPickBuffer::GetID(int x, int y)
{
	return 0;
}

}
}
