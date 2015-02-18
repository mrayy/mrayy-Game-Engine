
/********************************************************************
	created:	2008/11/18
	created:	18:11:2008   0:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine\ShaderCallback.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine
	file base:	ShaderCallback
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ShaderCallback___
#define ___ShaderCallback___

#include <matrix4x4.h>
#include <IVideoDevice.h>
#include <SLightNode.h>
#include <SMaterial.h>
#include <ITexture.h>
#include <SCameraNode.h>
#include <ShaderSemanticTable.h>
#include <GenericValues.h>
#include <VideoValues.h>


namespace mray{

class PostprocessingShaderConstants:public video::IShaderConstantsCallback
{
public:
// 	typedef std::map<core::string,GCPtr<IValue>> ValueList;
// 	ValueList m_values;
	GCPtr<video::TextureUnit> tex0;
	GCPtr<video::TextureUnit> tex1;
	GCPtr<video::TextureUnit> tex2;
	GCPtr<video::TextureUnit> tex3;

	math::vector3d cornerFrustrum;
protected:
	virtual void setUniform(video::GPUShader*shader,video::GPUUniform* u,bool VertexU)
	{
		if(u->name=="cornerFrustrum")
		{
			shader->setVertexShaderConstant(u,&cornerFrustrum.x,3);
		}else
			video::IShaderConstantsCallback::setUniform(shader,u,VertexU);
	}
public:
	PostprocessingShaderConstants(){
		tex0=new video::TextureUnit();
		tex1=new video::TextureUnit();
		tex2=new video::TextureUnit();
		tex3=new video::TextureUnit();
	}
/*
		GCPtr<IValue> getValue(const core::string&name){
			return m_values[name];
		}
		void setValue(GCPtr<IValue>v){
			if(!v)
				return ;
			m_values[v->getName()]=v;
		}*/
	

	virtual void setConstants(GCPtr<video::GPUShader>shader)
	{
		video::IShaderConstantsCallback::setConstants(shader);
// 		math::vector2d sz=video::ShaderSemanticTable::getInstance().getViewport().getSize();
// 		shader->setVertexShaderConstant("viewPortSize",&sz.x,2);
		/*
		core::IteratorPair<ValueList> p(m_values);
		for (;!p.done();p++)
		{
			switch ((*p)->getType())
			{
			case EVT_FLOAT:
				{
					FloatValue*v=*p;
					shader->setPixelShaderConstant(v->getName(),)
				}
			}
		}*/
		math::vector3d cr=video::ShaderSemanticTable::getInstance().getViewNode()->getViewFrustrum()->getFarLUp();
		shader->setVertexShaderConstant("farCorner",&cr.x,3);
		shader->setTexture("tex0",tex0->texture);
		shader->setTexture("tex1",tex1->texture);
		shader->setTexture("tex2",tex2->texture);
		shader->setTexture("tex3",tex3->texture);

	}
};

class ShaderSetConstants:public video::IShaderConstantsCallback
{
protected:
	GCPtr<video::IVideoDevice> m_dev;
	video::SMaterial*m_mat;
public:
	ShaderSetConstants(GCPtr<video::IVideoDevice> dev,video::SMaterial*mat){
		m_dev=dev;
		m_mat=mat;
	}

	virtual void setConstants(GCPtr<video::GPUShader>shader)
	{
		math::matrix4x4 matWorldViewProj;
		math::matrix4x4 world;
		math::matrix4x4 worldView;
		math::matrix4x4 proj;
		math::matrix4x4 view;
		math::matrix4x4 mat;

		m_dev->getTransformationState(video::TS_VIEW,view);
		m_dev->getTransformationState(video::TS_WORLD,world);
		m_dev->getTransformationState(video::TS_PROJECTION,proj);
		matWorldViewProj=proj;
		worldView=view*world;
		matWorldViewProj*=worldView;

		matWorldViewProj.getTransposeMatrix(mat);
		shader->setVertexShaderConstant("modelViewProj",mat.getMatPointer(),16);
		shader->setVertexShaderConstant("view_matrix",view.getMatPointer(),16);
		shader->setVertexShaderConstant("modelView",worldView.getMatPointer(),16);
		view.getInverse(mat);
		shader->setVertexShaderConstant("inv_view_matrix",mat.getMatPointer(),16);

	}
};

class phongShaderConstants:public ShaderSetConstants
{
	GCPtr<scene::SLightNode> m_light;
	GCPtr<scene::SCameraNode> m_cam;
public:
	phongShaderConstants(GCPtr<video::IVideoDevice> dev,video::SMaterial*mat,GCPtr<scene::SLightNode> light,
		GCPtr<scene::SCameraNode> cam)
		:ShaderSetConstants(dev,mat){
		m_light=light;
		m_cam=cam;
	}

	virtual void setConstants(GCPtr<video::GPUShader>shader)
	{
		ShaderSetConstants::setConstants(shader);
		math::vector3d lp=m_light->position();
		shader->setVertexShaderConstant("lightPos",&lp.x,3);
		shader->setVertexShaderConstant("camPos",&m_cam->position().x,3);

		shader->setTexture("texture",m_mat->getTexture(0));
	}
};
class bumpShaderConstants:public ShaderSetConstants
{
	GCPtr<scene::SLightNode> m_light;
	GCPtr<scene::SCameraNode> m_cam;
public:
	bumpShaderConstants(GCPtr<video::IVideoDevice> dev,video::SMaterial*mat,GCPtr<scene::SLightNode> light,GCPtr<scene::SCameraNode> cam)
		:ShaderSetConstants(dev,mat){
			m_light=light;
			m_cam=cam;
	}

	virtual void setConstants(GCPtr<video::GPUShader>shader)
	{
		ShaderSetConstants::setConstants(shader);
		math::vector3d lp=m_light->position();
		shader->setVertexShaderConstant("lightPos",&lp.x,3);
		lp=m_cam->position();
		shader->setVertexShaderConstant("viewPos",&lp.x,3);

		shader->setTexture("BaseMap",m_mat->getTexture(0));
		shader->setTexture("BumpMap",m_mat->getTexture(1));
		shader->setTexture("specularMap",m_mat->getTexture(2));
	}
};

class terrainShaderConstants:public ShaderSetConstants
{
	GCPtr<scene::SLightNode> m_light;
	GCPtr<scene::SCameraNode> m_cam;
public:
	float fallOff;

	terrainShaderConstants(GCPtr<video::IVideoDevice> dev,video::SMaterial*mat,GCPtr<scene::SCameraNode> cam)
		:ShaderSetConstants(dev,mat){
			fallOff=400;
			m_cam=cam;
	}

	virtual void setConstants(GCPtr<video::GPUShader>shader)
	{
		ShaderSetConstants::setConstants(shader);
		shader->setTexture("texture",m_mat->getTexture(0));
		shader->setTexture("detail",m_mat->getTexture(1));

		shader->setPixelShaderConstant("fallOff",&fallOff,1);
		math::vector3d pos=m_cam->position();
		shader->setVertexShaderConstant("camreaEye",&pos.x,1);
	}
};
}


#endif //___ShaderCallback___

