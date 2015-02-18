#include "stdafx.h"

#include "SPatchTerrain.h"
#include "ShaderResourceManager.h"

#include "FunctionProfiler.h"


#include "IViewNode.h"
#include "IVideoDevice.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ITexture.h"
#include "IsceneManager.h"

#include "ILogManager.h"
#include "ITexture.h"

#include "IHardwareBufferManager.h"
#include "TextureResourceManager.h"
#include "MaterialResourceManager.h"
#include "RenderManager.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"
#include "ShaderSemanticTable.h"

#include "videoValues.h"
#include "ISceneManager.h"
#include "IDebugDrawManager.h"

#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{


TerrainTextureLayer::TerrainTextureLayer()
{
	m_BaseTilt=1;
	m_DetailTilt=1;
	m_alpha=1;

	m_maskTexture=new video::TextureUnit();
	m_baseTexture=new video::TextureUnit();
	m_detailTexture=new video::TextureUnit();
}
TerrainTextureLayer::~TerrainTextureLayer()
{
	delete m_maskTexture;
	delete m_baseTexture;
	delete m_detailTexture;
}

void TerrainTextureLayer::SetMaskTexture(video::ITexture* tex)const
{
	m_maskTexture->SetTexture(tex);
}
video::ITexture* TerrainTextureLayer::GetMaskTexture()
{
	return m_maskTexture->GetTexture();
}

void TerrainTextureLayer::SetBaseTexture(video::ITexture* tex)const
{
	m_baseTexture->SetTexture(tex);
}
video::ITexture* TerrainTextureLayer::GetBaseTexture()
{
	return m_baseTexture->GetTexture();
}

void TerrainTextureLayer::SetDetailTexture(video::ITexture* tex)const
{
	m_detailTexture->SetTexture(tex);
}
video::ITexture* TerrainTextureLayer::GetDetailTexture()const
{
	return m_detailTexture->GetTexture();
}

	//////////////////////////////////////////////////////////////////////////
SPatchTerrain::SPatchTerrain(const math::vector3d&  pos,const math::vector3d&  scale):
tRepeat(1),cameraPosDelta(1),cameraRotateDelta(5),terrPivot(0),m_position(pos),m_scale(scale)
{
	video::RenderPass* pass;
	m_material=gMaterialResourceManager.getMaterial(mT("Terrain"));
	if(!m_material){
		m_material=new video::RenderMaterial();
		pass= m_material->CreateTechnique(mT("Default"))->CreatePass(mT(""));
		//m_material->shader=gShaderResourceManager.loadShader("terrain.cg","terrain.cg",mT("cg"));
		pass->setRenderState(video::RS_Lighting,video::ES_DontUse);
		pass->setRenderState(video::RS_Smooth,video::ES_DontUse);
	}
	m_layerMaterial=gMaterialResourceManager.getMaterial(mT("TerrainLayer"));

	if(!m_layerMaterial){
		m_layerMaterial=new video::RenderMaterial();
		pass= m_layerMaterial->CreateTechnique(mT("Default"))->CreatePass(mT(""));
		pass->setRenderState(video::RS_Lighting,video::ES_DontUse);
		pass->setRenderState(video::RS_Smooth,video::ES_DontUse);

		video::GPUShaderPtr shader=new video::GPUShader();
		shader->SetVertexShader(gShaderResourceManager.loadShaderFromFile(mT("TerrainLayer.cg"), video::EShader_VertexProgram, "main_vp", video::ShaderPredefList(), mT("cg")));
		shader->SetFragmentShader(gShaderResourceManager.loadShaderFromFile(mT("TerrainLayer.cg"), video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg")));

		pass->setRenderShader(shader);
	}
	pass=m_layerMaterial->GetActiveTechnique()->GetPassAt(0);
	pass->setRenderState(video::RS_Blend,video::ES_Use);
	pass->setRenderState(video::RS_ZWrite,video::ES_DontUse);

	pass->SetSourceBlendFactor(video::EBF_SRC_ALPHA);
	pass->SetDestinationBlendFactor(video::EBF_ONE_MINUS_SRC_ALPHA);

	m_skip=2;
}
SPatchTerrain::~SPatchTerrain()
{
	for(int i=0;i<m_layers.size();++i)
		delete m_layers[i];
	m_layers.clear();
}

void SPatchTerrain::initialize(const GCPtr<loaders::IHeightMapLoader>& heightMap,bool software){
	m_firstUpdate=true;
	m_heightMap=heightMap;


	m_heightMapName=m_heightMap->getFileName();
	{
		int i=m_heightMapName.findlast('/');
		if(i!=-1)
			m_heightMapName=m_heightMapName.substr(i+1,m_heightMapName.length()-i);
	}

	double*data=heightMap->getHeightData();

	int width=heightMap->getWidth();
	segments = width/m_skip;

	int count=segments*segments;
	m_basePos.resize(count);

	math::vector3d *vert=&m_basePos[0];

	int ver=0; 
	//creating Vertex Array 

	tRepeat=1;

	float y;
	math::Point2df uvCrd=0;
	float maxY=data[ver];
	float minY=data[ver];

	math::box3d b;
	bool flag=0;

	float mid=segments*0.5f;


	m_streams.start=0;

	video::MeshStreamKey key;
	video::IHardwareStreamBuffer* stream;
	key.index=0;
	key.type=video::EMST_Position;
	if(software)
		stream=video::IHardwareBufferManager::getInstance().createSoftwareStreamBuffer(video::ESDT_Point3f,
			count,video::IHardwareBuffer::EUT_Static);
	else
		stream=video::IHardwareBufferManager::getInstance().createHardwareStreamBuffer(video::ESDT_Point3f,
			count,video::IHardwareBuffer::EUT_Static,false);
	m_streams.streams.insert(video::StreamsMap::value_type(key,stream));
	
	key.index=0;
	key.type=video::EMST_Normal;
	stream=video::IHardwareBufferManager::getInstance().createHardwareStreamBuffer(video::ESDT_Point3f,
			count,video::IHardwareBuffer::EUT_Static,false);
	m_streams.streams.insert(video::StreamsMap::value_type(key,stream));

/**/
	key.index=0;
	key.type=video::EMST_Texcoord;
	stream=video::IHardwareBufferManager::getInstance().createHardwareStreamBuffer(video::ESDT_Point2f,
		count,video::IHardwareBuffer::EUT_Static,false);
	m_streams.streams.insert(video::StreamsMap::value_type(key,stream));

	video::IHardwareStreamBuffer* uvStream=stream;

	math::vector2d *tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	
	double iSegments=1.0f/(double)segments;
	float factor=1;//width*iSegments;


	for(int i=0;i<segments;++i){
		for(int j=0;j<segments;j++)
		{
			int index=(i*width+j)*factor*m_skip;
			ver=(i*segments+j)*factor;
			y=data[index];
			if(y>maxY)
				maxY=y;
			if(y<minY)
				minY=y;

			vert[ver].x=i*iSegments;
			vert[ver].z=j*iSegments;
			vert[ver].y=y;


			if(!flag){
				b.reset(vert[ver]);
				flag=1;
			}else
				b.AddPoint(vert[ver]);

			tc[ver].x= (float)j*iSegments;
			tc[ver].y= 1-(float)i*iSegments;

		}
	}


	uvStream->unlock();
	terrPivot=b.getCenter();
	terrPivot.y=0.5;


	applySmoothing();
	applyTransformation();


	calcNormals();
	//m_heightMap=0;	

}


void SPatchTerrain::AddTextureLayer(TerrainTextureLayer* layer)
{
	m_layers.push_back(layer);
}
const std::vector<TerrainTextureLayer*>& SPatchTerrain::GetTextureLayers()const
{
	return m_layers;
}
bool SPatchTerrain::loadTerrain(const GCPtr<loaders::IHeightMapLoader>& heightMap,int lod,float lodDist,bool software)
{
	m_lodDist=lodDist;

	if(!heightMap)return 0;



	m_skip=1<<0;
	int width=heightMap->getWidth();
	segments = width/m_skip;


	int po2=(1<<lod);
	if(po2<1){
		lod=1;
		po2 = 1;
	}
	if(po2 > segments){
		lod=log((float)segments)/log(2.0f);
		po2=segments;
	}
	maxLOD=lod;
	patchCnt=segments / po2;

	patchSize = segments / patchCnt;

	initialize(heightMap,software);

	return 1;
}

bool SPatchTerrain::loadTerrain(const GCPtr<loaders::IHeightMapLoader>& heightMap,int patchPerSide,int lod,float lodDist,bool software)
{
	m_lodDist=lodDist;

	if(!heightMap)return 0;

	m_skip=1<<0;
	int width=heightMap->getWidth();
	segments = width/m_skip;

	patchSize =math::Max(1,segments / patchPerSide);


	maxLOD=math::Min<int>(lod,log((float)patchSize)/log(2.0f));

	patchCnt=segments/(1<<maxLOD);

	patchSize = segments / patchCnt;

	segments=patchCnt*patchSize;

	initialize(heightMap,software);

	return 1;
}

const GCPtr<loaders::IHeightMapLoader>& SPatchTerrain::getHeightMap()
{
	return m_heightMap;
}
int SPatchTerrain::GetLODCount()
{
	return maxLOD;
}
float SPatchTerrain::GetLODDistance()
{
	return m_lodDist;
}
int SPatchTerrain::GetPatchCount()
{
	return patchCnt;
}
int SPatchTerrain::GetSegementsCount()
{
	return segments;
}
bool SPatchTerrain::preRender(IRenderPass*pass){
	if(!pass->isPrepareationPass() || m_firstUpdate)
	{
		recalcLOD(pass->getViewNode() ,m_firstUpdate);
		m_firstUpdate=false;
		recalcPatchesToRender();
	}

	RenderOperation op;
	op.node=this;
	op.customRender=true;
	m_owner->getSceneManager()->getRenderManager()->addOperation(RGH_Solid,op);
	//getSceneManager()->addNodeToRender(this,NRT_SOLID);
	return true;
}

void SPatchTerrain::_DrawLayer(TerrainTextureLayer* layer,video::RenderPass* mtrl)
{
	video::IVideoDevice*dev=m_owner->getSceneManager()->getDevice();
	dev->useRenderPass(mtrl);
	if(layer)
	{
		video::IGPUShaderProgram* shader;
		shader=mtrl->getRenderShader()->GetVertexShader();
		if(shader)
		{
			shader->setConstant(mT("baseTilt"),&layer->m_BaseTilt,1);
			shader->setConstant(mT("detailTilt"),&layer->m_DetailTilt,1);
			shader->setConstant(mT("alpha"),&layer->m_alpha,1);
		}
		shader=mtrl->getRenderShader()->GetFragmentShader();
		if(shader)
		{
			shader->setTexture(mT("baseTex"),layer->GetBaseTextureUnit());
			shader->setTexture(mT("detailTex"),layer->GetDetailTextureUnit());
			shader->setTexture(mT("maskTex"),layer->GetMaskTextureUnit());
		}

	}

	for(int i=0;i<patchs.size();++i){
		SPatch*patch=&patchs[i];
		if(patch->currentLod>=0){
// 			if(layer && patch->currentLod>4)
// 				continue;
			for (int p=0;p<patch->m_indicies.size();p++)
			{
				dev->drawElementsUP(&patch->m_indicies[p][0],
					video::IHardwareIndexBuffer::EIT_32Bit,0,patch->m_indicies[p].size(),video::MR_TRIANGLE_STRIP);
			}
		}
	}	
}

bool SPatchTerrain::render(IRenderPass*pass)
{
	if(!pass->canRenderObject(this))
		return true;
	video::IVideoDevice*dev=m_owner->getSceneManager()->getDevice();
	
	dev->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	if(m_owner->getDrawBoundingBox()){
		dev->useRenderPass(0);
		for(int i=0;i<patchs.size();++i){
			if(patchs[i].currentLod>=0)
			{
				if(m_owner->getSceneManager()->GetDebugDrawManager())
				{
					m_owner->getSceneManager()->GetDebugDrawManager()->AddAABB(patchs[i].BBox,1);
				}
			//	dev->drawBox(patchs[i].BBox,math::matrix4x4::Identity,video::MR_LINES);
			}
		}
	}

	video::ShaderSemanticTable::getInstance().setPrevWorldMatrix(getOwner()->getPrevAbsoluteTransformation());
	video::IShaderConstantsCallback callback;
	video::RenderPass* p=m_material->GetActiveTechnique()->GetPassAt(0);
	if(pass->useMaterial(p))
	{
		callback.setConstants(p->getRenderShader());
		dev->setStreamMap(&m_streams);
		_DrawLayer(0,p);//draw base layer
		p=m_layerMaterial->GetActiveTechnique()->GetPassAt(0);
		if(pass->useMaterial(p))
		{
			dev->setDepthBias(0.1,0.01);
			callback.setConstants(p->getRenderShader());
			for(int i=0;i<m_layers.size();++i)
			{
				_DrawLayer(m_layers[i],p);
			}
			dev->setDepthBias(0,0);
		}
		dev->endStreams();
		dev->unuseShader();
	}
	return true;
}

void SPatchTerrain::update(float dt)
{
}

void SPatchTerrain::calcPatchs(){
	 
	patchs.resize(patchCnt*patchCnt);

	video::IHardwareStreamBuffer* vStream=getStream(0,video::EMST_Position);
	math::vector3d* vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	BoundingBox=math::box3d(math::vector3d(math::Infinity),math::vector3d(-math::Infinity));
	for(int x=0;x<patchCnt;x++){
	for(int z=0;z<patchCnt;z++){
		int idx=x*patchCnt + z;
		SPatch*p=&patchs[idx];
		p->currentLod=-1;
		p->BBox.reset(vert[x*patchSize*segments + z*patchSize]);
/*		if(p->m_indexBuffer==0){
			p->m_indexBuffer=video::IHardwareBufferManager::getInstance().createHardwareIndexBuffer(video::IHardwareIndexBuffer::EIT_32Bit,
				0,video::IHardwareBuffer::EUT_DynamicWriteOnly,false);
		}*/
		for(int i=x*(patchSize-1);i<=(x+1)*(patchSize-1);++i)
		for(int j=z*(patchSize-1);j<=(z+1)*(patchSize-1);j++)
			p->BBox.AddPoint(vert[i*segments + j]);

		p->center = p->BBox.getCenter();

		BoundingBox.AddBox(p->BBox);

		if(x>0)
			p->top=(x-1)*patchCnt + z;
		else p->top=-1;

		if(x<patchCnt-1)
			p->bottom=(x+1)*patchCnt + z;
		else p->bottom=-1;

		if(z>0)
			p->left=x*patchCnt + z-1;
		else p->left=-1;

		if(z<patchCnt-1)
			p->right=x*patchCnt + z+1;
		else p->right=-1;

	}
	}

	center=BoundingBox.getCenter();
	vStream->unlock();

}

void SPatchTerrain::calcDistThreshold(){

	LodDistThreshold.resize(maxLOD);

	//float iSegements=1.0f/segments;
	for(int i=0;i<maxLOD;++i){
//		float sc=patchSize*(i+1+i*i);
// 		LodDistThreshold[i].x=terrScale.x*math::sqr<float>(sc*lodDist.x);
// 		LodDistThreshold[i].y=terrScale.y*math::sqr<float>(sc*lodDist.y);
// 		LodDistThreshold[i].z=terrScale.z*math::sqr<float>(sc*lodDist.z);
		//LodDistThreshold[i]*=LodDistThreshold[i]; // get As Square
		LodDistThreshold[i]=m_scale*patchSize*(i*i+i*0.5+ 1)*m_lodDist;
		//LodDistThreshold[i]*=LodDistThreshold[i];
	}
}


void SPatchTerrain::recalcLOD(IViewNode*vn,bool forceCalc){

	ISceneManager* smngr=m_owner->getSceneManager();

	math::vector3d camPos=0;
	math::vector3d camRot=0;

	if(!vn)return;

	math::line3d camLine;
	camLine.pStart = vn->getViewFrustrum()->viewPos;
	camLine.pEnd = vn->getViewMatrix()*math::vector3d::ZAxis;

	camPos = camLine.pStart;
	camRot = camLine.getVector().getHorizontalAngle();

	//camPos=math::vector3d(0,0,0);

	if(!forceCalc && fabs(oldCamPos.x - camPos.x) < cameraPosDelta&&
	   fabs(oldCamPos.y - camPos.y) < cameraPosDelta&&
	   fabs(oldCamPos.z - camPos.z) < cameraPosDelta&&
	   fabs(oldCamRot.x - camRot.x) < cameraRotateDelta&&
	   fabs(oldCamRot.y - camRot.y) < cameraRotateDelta)
	{
		return;
	}

	oldCamPos=camPos;
	oldCamRot=camRot;

	float lookDist=3;

	const math::Frustum*f=vn->getViewFrustrum();
	//f->recalcBoundingBox();

	for(int p=0;p<patchCnt*patchCnt;p++){
		int lod=0;
		if(f->boundingBox.intersectWithBox(patchs[p].BBox)){
		//if(f->pointInFrustrum(patchs[p].center)){
			patchs[p].distToCam=(camPos - patchs[p].center).LengthSQ();
			math::vector3d dist = patchs[p].distToCam;
			//dist-=terrScale.x*patchSize*lookDist;
			dist.x=abs(dist.x);
			dist.y=abs(dist.y);
			dist.z=abs(dist.z);
			for(int i=maxLOD-1; i>=0 ;i--){
				lod = i;
				if( dist.x>=LodDistThreshold[i].x ||
					//dist.y>=LodDistThreshold[i].y ||
					dist.z>=LodDistThreshold[i].z)
				{
					break;
				}
			}
		}else{
			lod = -1;
		}
		patchs[p].isChanged=true;
		if(patchs[p].currentLod != lod){
			patchs[p].currentLod = lod;
			patchs[p].isChanged=true;
			if(patchs[p].top!=-1)patchs[patchs[p].top].isChanged=true;
			if(patchs[p].bottom!=-1)patchs[patchs[p].bottom].isChanged=true;
			if(patchs[p].left!=-1)patchs[patchs[p].left].isChanged=true;
			if(patchs[p].right!=-1)patchs[patchs[p].right].isChanged=true;
		}
	}


}

bool SPatchTerrain::calcPatchIndicies(int pX,int pZ,SPatch*patch){
	if(patch->currentLod<0)return false;

	int step = 1<<patch->currentLod;
	int vCnt=patchSize/step;

	patch->m_indicies.resize(vCnt);
	//.resize(vCnt*vCnt*2*3);

	int z=0;
	for (int i=0;i<vCnt;++i)
	{
		patch->m_indicies[i].resize((vCnt+1)*2);
		uint*idx=&patch->m_indicies[i][0];

		int n=0;

		int x=0;
		for(int j=0;j<vCnt+1;j++){

			//creating Indices Array
			/*

			1--2
			|\ |
			| \|
			3--4
			t1=(1,3,4)
			t2=(1,4,2)
			...and so on

			*/
			idx[n++]=getIndex(pX,pZ,patch,x,z);
			idx[n++]=getIndex(pX,pZ,patch,x,z+step);
			//i2=getIndex(pX,pZ,patch,x+step,z);
			//i4=getIndex(pX,pZ,patch,x+step,z+step);
/*
			idx[n++]=i1;
			idx[n++]=i3;
			idx[n++]=i4;

			idx[n++]=i1;
			idx[n++]=i4;
			idx[n++]=i2;*/


			x+=step;
		}
		z+=step;
	}
	/*
	patch->m_indexBuffer->resize(patch->m_indicies.size()*sizeof(uint));
	patch->m_indexBuffer->writeData(0,patch->m_indexBuffer->getSizeInBytes(),patch->m_indicies.pointer());
*/
	patch->isChanged=0;
	return true;
}

void SPatchTerrain::recalcPatchesToRender(){

	for(int i=0;i<patchCnt;++i){
		for(int j=0;j<patchCnt;j++){
			int index=i*patchCnt+j;
			SPatch*patch=&patchs[index];
			if(patch->isChanged)
				calcPatchIndicies(i,j,patch);
		}
	}
/*
	GCPtr<video::IndexData> idata= terrRenderMesh->getBuffer(0)->getIndexData();

	GCPtr<video::IHardwareIndexBuffer> indexBuff=idata->indexBuffer;
	indexBuff->resize(numberOfIndices);

	indexBuff->writeData(0,indexBuff->getSizeInBytes(),m_tempIndicies.pointer());
*/
}

int SPatchTerrain::getIndex(int pX,int pZ,SPatchTerrain::SPatch*patch,int vX,int vZ){
	//PROFILE_FUNCTION();

	if(vX==0){//top
		if(patch->top!=-1 ){
			int lod=patchs[patch->top].currentLod;
			if(lod!=-1 && patch->currentLod<lod){
				if(vZ%(1<<lod)!=0){
					vZ -= vZ%(1<<lod);
				}
			}
		}
	}else if(vX==(patchSize)){//bottom
		if(patch->bottom!=-1){
			int lod=patchs[patch->bottom].currentLod;
			 if(lod!=-1 && patch->currentLod<lod){
				 if(vZ%(1<<lod)!=0){
					vZ -= vZ%(1<<lod);
				}
			}
		}
	}
	if(vZ==0){//left
		if(patch->left!=-1){
			int lod=patchs[patch->left].currentLod;
			if(lod!=-1 && patch->currentLod<lod){
				vX -= vX%(1<<lod);
			}
		}
	}else if(vZ==(patchSize)){//right
		if(patch->right!=-1){
			int lod=patchs[patch->right].currentLod;
			if(lod!=-1 && patch->currentLod<lod){
				vX -= vX%(1<<lod);
			}
		}
	}

	if(vZ >= patchSize) vZ=patchSize-1;
	if(vX >= patchSize) vX=patchSize-1;
	return (vX + (patchSize-1) * pX)*segments + 
			vZ + (patchSize-1) * pZ;
}

const core::string& SPatchTerrain::getHeightMapName()
{
	
	return m_heightMapName;
}
void SPatchTerrain::setTexture(const video::ITexturePtr&   tex,int layer){
	if(!m_material)return;
	m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,layer);
}
void SPatchTerrain::setBaseTexture(const video::ITexturePtr&   tex){
	if(!m_material)return;
	 m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);
	 return;
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("base"));
	if(!v){
		video::TextureUnit* u=new video::TextureUnit();
		u->SetTexture(tex);
		v=new TextureValue(mT("base"),u);
		m_material->GetActiveTechnique()->GetPassAt(0)->addValue(v);
	}else{
		((TextureValue*)v)->value->SetTexture(tex);
	}
}
void SPatchTerrain::setBaseNormal(const video::ITexturePtr&   tex)
{
	if(!m_material)return;
	m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,1);
	return;
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("normalMap"));
	if(!v){
		video::TextureUnit* u=new video::TextureUnit();
		u->SetTexture(tex);
		v=new TextureValue(mT("normalMap"),u);
		m_material->GetActiveTechnique()->GetPassAt(0)->addValue(v);
	}else{
		((TextureValue*)v)->value->SetTexture(tex);
	}
}
void SPatchTerrain::setDetailTexture(const video::ITexturePtr&   tex){
	if(!m_material)return;
	m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,2);
	return;
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("detail"));
	if(!v){
		video::TextureUnit* u=new video::TextureUnit();
		u->SetTexture(tex);
		v=new TextureValue(mT("detail"),u);
		m_material->GetActiveTechnique()->GetPassAt(0)->addValue(v);
	}else{
		((TextureValue*)v)->value->SetTexture(tex);
	}
}
void SPatchTerrain::setDetailNormal(const video::ITexturePtr&   tex){
	if(!m_material)return;
	m_material->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,3);
	return;
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("detailNormal"));
	if(!v){
		video::TextureUnit* u=new video::TextureUnit();
		u->SetTexture(tex);
		v=new TextureValue(mT("detailNormal"),u);
		m_material->GetActiveTechnique()->GetPassAt(0)->addValue(v);
	}else{
		((TextureValue*)v)->value->SetTexture(tex);
	}
}
const video::ITexturePtr&   SPatchTerrain::getBaseTexture()
{
	if(!m_material)return video::ITexturePtr::Null;
	return m_material->GetActiveTechnique()->GetPassAt(0)->getTexture(0);
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("base"));
	if(v)
		return ((TextureValue*)v)->value->GetTexture();
	return video::ITexturePtr::Null;
}
const video::ITexturePtr&   SPatchTerrain::getBaseNormal()
{
	if(!m_material)return video::ITexturePtr::Null;
	return m_material->GetActiveTechnique()->GetPassAt(0)->getTexture(1);
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("normalMap"));
	if(v)
		return ((TextureValue*)v)->value->GetTexture();
	return video::ITexturePtr::Null;
}
const video::ITexturePtr&   SPatchTerrain::getDetailTexture()
{
	if(!m_material)return video::ITexturePtr::Null;
	return m_material->GetActiveTechnique()->GetPassAt(0)->getTexture(2);
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("detail"));
	if(v)
		return ((TextureValue*)v)->value->GetTexture();
	return video::ITexturePtr::Null;
}
const video::ITexturePtr&   SPatchTerrain::getDetailNormal()
{
	if(!m_material)return video::ITexturePtr::Null;
	return m_material->GetActiveTechnique()->GetPassAt(0)->getTexture(3);
	IValue*v=m_material->GetActiveTechnique()->GetPassAt(0)->getValue(mT("detailNormal"));
	if(v)
		return ((TextureValue*)v)->value->GetTexture();
	return video::ITexturePtr::Null;
}

void SPatchTerrain::setMaterial(const video::RenderMaterialPtr& mtrl,int i){
	m_material=mtrl;
}
const video::RenderMaterialPtr&  SPatchTerrain::getMaterial(int i){
	return m_material;
}
int SPatchTerrain::getMaterialCount(){
	return 1;
}

void SPatchTerrain::setShader(int mesh,const video::GPUShaderPtr& shader)
{
	m_material->GetActiveTechnique()->GetPassAt(0)->setRenderShader(shader);
}
void SPatchTerrain::setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback)
{
}

const video::GPUShaderPtr& SPatchTerrain::getShader(int i)
{
	return m_material->GetActiveTechnique()->GetPassAt(0)->getRenderShader();
}

void SPatchTerrain::setPosition(const math::vector3d& v){
	m_position=v;
	applyTransformation();
}
/*
void SPatchTerrain::setRotation(const math::vector3d& v){
	setOrintation(v);
	applyTransformation();
}*/

void SPatchTerrain::setScale(const math::vector3d& v){
	m_scale=v;
	applyTransformation();
}
void SPatchTerrain::setTransofrmation(const math::vector3d& pos/*,math::quaternion orintation*/,const math::vector3d& scale){
	m_position=pos;
	//setOrintation(orintation);
	m_scale=scale;
	applyTransformation();
}

void SPatchTerrain::applyTransformation(){

	video::IHardwareStreamBuffer* vertexBuffer=getStream(0,video::EMST_Position);
	math::vector3d* vert=(math::vector3d*)vertexBuffer->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
/*	getOrintation().toMatrix(transMatrix);
	transMatrix.setTranslation(terrPos);
	if(terrScale!=1){
		matrix4x4 m;
		m.setScale(terrScale);
		transMatrix=transMatrix*m;
	}
*/


	math::vector3d s=m_scale;
	math::vector3d p=m_position;

	transMatrix.setScale(s);
	transMatrix.setTranslation(p);

	int vCount=m_basePos.size();
	math::vector3d tpv=terrPivot*s;
	for(int i=0;i<vCount;++i){

		vert[i]=m_basePos[i]*s+p;
		vert[i]-=tpv;

		//vert[i]=transMatrix.inverseTransformVectPre(vert[i]);
		//v2->Pos=m.inverseTransformVectPre(v2->Pos);
	//	v2->Pos+=tpv;

	//	v2->Pos+=tpv;


	}
	vertexBuffer->unlock();
	
	calcPatchs();
	calcDistThreshold();
//	calcNormals();

}

void SPatchTerrain::OnHeightChanged(int vtx,float h)
{
	float height=((h+terrPivot.y*m_scale.y-m_position.y))/m_scale.y;
	m_basePos[vtx].y=height;
}


float SPatchTerrain::getHeight(float x,float z,math::vector3d *outNormal)
{


	float height=-math::Infinity;
	
	math::vector3d pos(x,0,z);
//	pos-=terrPivot;
	//pos=transMatrix.inverseTransformVectPre(pos);
	/*pos+=terrPivot*getScale();

	pos-=getPosition();

	pos/=getScale();

	pos*=segments;*/

	pos=TransformVectorToLocal(pos);

	int iX=floorf(pos.x);
	int iZ=floorf(pos.z);


	math::vector3d terrScale=m_scale;

	//math::Swap<int>(iX,iZ);

	if(iX>=0 && iX<segments-1 && iZ>=0 && iZ<segments-1 ){
		math::vector3d &a=m_basePos[iX*segments+iZ];
		math::vector3d &b=m_basePos[(iX+1)*segments+iZ];
		math::vector3d &c=m_basePos[iX*segments+(iZ+1)];
		math::vector3d &d=m_basePos[(iX+1)*segments+(iZ+1)];

		float dx=pos.x-iX;
		float dz=pos.z-iZ;
		if(dx>dz){
			height=a.y+(d.y-b.y)*dz+(b.y-a.y)*dx;
			if(outNormal){
				(*outNormal)=(d*terrScale-a*terrScale).crossProduct(b*terrScale-a*terrScale);
			}
		}
		else{
			height=a.y+(d.y-c.y)*dx+(c.y-a.y)*dz;
			if(outNormal){
				(*outNormal)=-(d*terrScale-a*terrScale).crossProduct(c*terrScale-a*terrScale);
			}
		}

		if(outNormal){
			(*outNormal).Normalize();
		}
		height=(height-terrPivot.y)*terrScale.y+m_position.y;
	}
	return height;

}
void SPatchTerrain::applySmoothing(){

	math::vector3d*vtx=&m_basePos[0];
	for(int i=0;i<segments;++i){
	for(int j=0;j<segments;j++){
		//3x3 filtering

		float value=vtx[i*(segments-1)+j].y;
		int cellAvg=1;

		if(i>0){
			value+=vtx[(i-1)*(segments-1)+j].y;
			cellAvg++;
			if(j>0){
				value+=vtx[(i-1)*(segments-1)+(j-1)].y;
				cellAvg++;
			}
			if(j<segments-1){
				value+=vtx[(i-1)*(segments-1)+(j+1)].y;
				cellAvg++;
			}
		}
		if(i<segments-1){
			value+=vtx[(i+1)*(segments-1)+j].y;
			cellAvg++;
			if(j>0){
				value+=vtx[(i+1)*(segments-1)+(j-1)].y;
				cellAvg++;
			}
			if(j<segments-1){
				value+=vtx[(i+1)*(segments-1)+(j+1)].y;
				cellAvg++;
			}
		}
		if(j>0){
			value+=vtx[i*(segments-1)+(j-1)].y;
			cellAvg++;
		}
		if(j<segments-1){
			value+=vtx[i*(segments-1)+(j+1)].y;
			cellAvg++;
		}

		vtx[i*(segments-1)+j].y=value/(float)(cellAvg);
	}
	}
}

void SPatchTerrain::calcNormals(){
	video::IHardwareStreamBuffer*vStream=getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer*nStream=getStream(0,video::EMST_Normal);

	math::vector3d* vtx=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector3d* norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int cnt=0;
	math::vector3d normal;
	math::vector3d a,b,c,t;

	for(int i=0;i<segments;++i){
	for(int j=0;j<segments;j++){
		cnt=0;
		normal=0;

		if(i>0 && j>0){
			a=vtx[(i-1)*(segments-1)+j-1];
			b=vtx[(i-1)*(segments-1)+j  ]	- a;
			c=vtx[ i   *(segments-1)+j  ]	- a;
			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

			//	b=vtx[(i-1)*(segments-1)+j-1].Pos;
			b=c;
			c=vtx[ i   *(segments-1)+j-1]	- a;

			t=c.crossProduct(b);
			t.Normalize();
			normal+=t;

			cnt+=2;
		}

		if(i>0 && j<segments-1){
			a=vtx[(i-1)*(segments-1)+j  ];
			b=vtx[(i-1)*(segments-1)+j+1]	- a;
			c=vtx[ i   *(segments-1)+j+1]	- a;
			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

		//	a=vtx[(i-1)*(segments-1)+j  ].Pos;
			b=c;//vtx[ i   *(segments-1)+j+1].Pos	- a;
			c=vtx[ i   *(segments-1)+j  ]	- a;

			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

			cnt+=2;
		}

		if(i<segments-1 && j<segments-1){
			a=vtx[ i   *(segments-1)+j  ];
			b=vtx[ i   *(segments-1)+j+1]	- a;
			c=vtx[(i+1)*(segments-1)+j+1]	- a;
			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

		//	a=vtx[ i   *(segments-1)+j+1].Pos;
		//	b=vtx[(i+1)*(segments-1)+j+1].Pos	- a;
			b=c;
			c=vtx[(i+1)*(segments-1)+j  ]	- a;

			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

			cnt+=2;
		}

		if(i<segments-1 && j>0){
			a=vtx[ i   *(segments-1)+j-1];
			b=vtx[ i   *(segments-1)+j  ]	- a;
			c=vtx[(i+1)*(segments-1)+j  ]	- a;
			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

// 			a=vtx[ i   *(segments-1)+j-1].Pos;
// 			b=vtx[(i+1)*(segments-1)+j  ].Pos	- a;
			b=c;
			c=vtx[(i+1)*(segments-1)+j-1]	- a;

			t=b.crossProduct(c);
			t.Normalize();
			normal+=t;

			cnt+=2;
		}

		if(!cnt)
			normal.set(0,1,0);
		else normal.Normalize();
		norm[ i *(segments-1)+j ]=normal;

	}
	}
	vStream->unlock();
	nStream->unlock();
}





math::vector3d SPatchTerrain::TransformVectorToLocal(const math::vector3d& v)
{
	return ((v+terrPivot*m_scale-m_position)*segments)/m_scale;
}
math::vector3d SPatchTerrain::TransformVectorToWorld(const math::vector3d& v)
{
	return (v*m_scale/(float)segments)+m_position-terrPivot*m_scale;
}
void SPatchTerrain::QueryQuads2D(const math::rectf&rc,std::vector<math::rectf>& outRects,bool &splitDir)
{
	math::vector3d minP,maxP;
	minP=TransformVectorToLocal(math::vector3d(rc.ULPoint.x,0,rc.ULPoint.y));
	maxP=TransformVectorToLocal(math::vector3d(rc.BRPoint.x,0,rc.BRPoint.y));
	minP.x=floorf(minP.x);
	minP.z=floorf(minP.z);

	maxP.x=ceilf(maxP.x);
	maxP.z=ceilf(maxP.z);

	for (int i=minP.x;i<maxP.x;++i)
	{
		for (int j=minP.z;j<maxP.z;++j)
		{
		}
	}
}
void SPatchTerrain::ProjectTriangle(const math::vector3d *v)
{
}

void SPatchTerrain::exportTerrain(const  mchar*fileName){

	OS::IStreamPtr file=gFileSystem.createBinaryFileWriter(fileName);
	if(!file)return;


	math::vector3d* vtx=(math::vector3d*)&m_basePos[0];
	int cnt=m_basePos.size();

	//export Vertices
	file->write(&segments,sizeof(segments));

	for(int i=0;i<cnt;++i)
	{
		file->write(&vtx[i],sizeof(vtx[i]));
	}
	//bbox
	file->write(&terrPivot,sizeof(terrPivot));

	//patches
	file->write(&patchSize,sizeof(patchSize));
	file->write(&maxLOD,sizeof(maxLOD));
	file->write(&patchCnt,sizeof(patchCnt));
	file->write(&m_lodDist,sizeof(m_lodDist));
	
	int tmp=patchs.size();

	file->write(&tmp,sizeof(tmp));
	for(int i=0;i<patchs.size();++i){
		file->write(&patchs[i],sizeof(patchs[i]));
	}

	tmp=LodDistThreshold.size();
	file->write(&tmp,sizeof(tmp));
	for(int i=0;i<LodDistThreshold.size();++i){
		file->write(&LodDistThreshold[i],sizeof(LodDistThreshold[i]));
	}

	file->close();
}

void SPatchTerrain::importTerrain(const  mchar*fileName){
	OS::IStreamPtr file=gFileSystem.openFile(fileName,OS::BIN_READ);
	if(!file)return;

	//import Vertices
	file->read(&segments,sizeof(segments));

	m_basePos.resize(segments*segments);


	math::vector3d*vtx=&m_basePos[0];

	for(int i=0;i<segments*segments;++i)
	{
		file->read(&vtx[i],sizeof(vtx[i]));
	}
	//bbox
	file->read(&terrPivot,sizeof(terrPivot));

	//patches
	file->read(&patchSize,sizeof(patchSize));
	file->read(&maxLOD,sizeof(maxLOD));
	file->read(&patchCnt,sizeof(patchCnt));
	file->read(&m_lodDist,sizeof(m_lodDist));
	
	int tmp;

	file->read(&tmp,sizeof(tmp));
	patchs.resize(tmp);
	for(int i=0;i<patchs.size();++i){
		file->read(&patchs[i],sizeof(patchs[i]));
	}

	file->read(&tmp,sizeof(tmp));
	LodDistThreshold.resize(tmp);
	for(int i=0;i<LodDistThreshold.size();++i){
		file->read(&LodDistThreshold[i],sizeof(LodDistThreshold[i]));
	}

	applyTransformation();
}


video::IHardwareStreamBuffer* SPatchTerrain::getStream(int s,video::EMeshStreamType type){
	video::StreamsMapIT it= m_streams.streams.find(video::MeshStreamKey(s,type));
	if(it==m_streams.streams.end())return 0;
	return it->second;
}


xml::XMLElement* SPatchTerrain::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr=0;
	xml::XMLElement* e=elem->getSubElement(mT("Terrain"));
	if(!e)return 0;


	attr=e->getAttribute(mT("BaseTex"));
	if(attr){
		setBaseTexture(gTextureResourceManager.loadTexture2D(attr->value));
	}
	attr=e->getAttribute(mT("BaseNormalTex"));
	if(attr){
		setBaseNormal(gTextureResourceManager.loadTexture2D(attr->value));
	}
	attr=e->getAttribute(mT("DetailTex"));
	if(attr){
		setDetailTexture(gTextureResourceManager.loadTexture2D(attr->value));
	}
	attr=e->getAttribute(mT("DetailNormalTex"));
	if(attr){
		setDetailNormal(gTextureResourceManager.loadTexture2D(attr->value));
	}
	return e;
}
xml::XMLElement* SPatchTerrain::exportXMLSettings(xml::XMLElement*elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("Terrain"));
	elem->addSubElement(e);

	e->addAttribute(mT("HeightMap"),getHeightMapName());
	e->addAttribute(mT("LODCount"),core::StringConverter::toString(GetLODCount()));
	e->addAttribute(mT("LODDist"),core::StringConverter::toString(GetLODDistance()));
	e->addAttribute(mT("Scale"),core::StringConverter::toString(m_scale));
	e->addAttribute(mT("Position"),core::StringConverter::toString(m_position));
	video::ITexturePtr  tex;
	tex=getBaseTexture();
	if(tex)
		e->addAttribute(mT("BaseTex"),tex->getResourceName());
	tex=getBaseNormal();
	if(tex)
		e->addAttribute(mT("BaseNormalTex"),tex->getResourceName());
	tex=getDetailTexture();
	if(tex)
		e->addAttribute(mT("DetailTex"),tex->getResourceName());
	tex=getDetailNormal();
	if(tex)
		e->addAttribute(mT("DetailNormalTex"),tex->getResourceName());

	return e;
}
}
}
