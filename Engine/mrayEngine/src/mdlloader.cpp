#include "stdafx.h"
 

//Mdl Loader
//Created by Mohamad Yamen Saraiji
// This is a part of M-RAY Engine

// this is the mdlloader
// plugin created by Mohamad Yamen Saraiji for Maya
// the plugin exports models with there animation

#include "MdlLoader.h"

#include "IFileSystem.h"

#include "IStream.h"
#include "ILogManager.h"
#include "ITexture.h"
#include "ICycles.h"
#include "RenderMaterial.h"
#include "SMorphMeshBuffer.h"
#include "TangentCalculater.h"
#include "MountPoint.h"

#include "MeshAnimatorManager.h"
#include "MorphAnimationTrack.h"

#include "TextureResourceManager.h"
#include "BoneNode.h"
#include "MeshBufferData.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

scene::BoneNode* mdlLoader::loadJoint(scene::Skeleton*skeleton,scene::BoneNode*parent){

	math::vector3d pos,ori;
	m_stream->read(tmpStr,32*sizeof(char));
	core::char_to_string(tmpStr,m_tmpStr);

	scene::BoneNode*joint=skeleton->createBone(m_tmpStr,parent);
	joint->setNodeName(m_tmpStr);
	m_stream->read(&pos,sizeof(math::vector3d));
	m_stream->read(&ori,sizeof(math::vector3d));
	//	pos.x*=-1;

	math::quaternion q(ori);/*
	q=math::quaternion(ori.x,math::vector3d::XAxis)*
		math::quaternion(ori.y,math::vector3d::YAxis)*
		math::quaternion(ori.z,math::vector3d::ZAxis);*/
	joint->setBasePosition(pos);
	joint->setBaseOrintation(q);
	joint->setPosition(pos);
/**/	joint->setOrintation(q);
	int jCnt;
	m_stream->read(&jCnt,sizeof(int));

	for(int i=0;i<jCnt;++i){
		
		scene::BoneNode*j=loadJoint(skeleton,joint);
		joint->addBone(j);
	}

	return joint;
}

scene::Skeleton* mdlLoader::readSkeleton()
{
	//char skeletonName[32];
	//m_stream->read(skeletonName,32*sizeof(char));
	scene::Skeleton* skeleton=new scene::Skeleton(m_stream->getStreamName());

	scene::BoneNode*b=loadJoint(skeleton,0);
	skeleton->refreshRoots();

	return skeleton;
}
bool mdlLoader::loadWeights(scene::SMesh*mesh,scene::Skeleton*skeleton){

	core::string wTempStr;
	char jName[32];
	int tmpI;
	m_stream->read(jName,32*sizeof(char));
	core::char_to_string(jName,wTempStr);


	int meshCnt;
	m_stream->read(&meshCnt,sizeof(int));
	scene::ISkinCluster::SBoneWeights *weights=0;
	for(int i=0;i<meshCnt;++i){
		m_stream->read(tmpStr,32*sizeof(char));
		scene::IMeshBuffer*buff=mesh->getBufferByName(core::StringConverter::toString(tmpStr).c_str());
		if(!buff)return 0;

		scene::ISkinCluster*cluster=buff->getSkinCluster();
		if(!cluster)
		{
			cluster=new scene::ISkinCluster(skeleton);
			buff->setSkinCluster(cluster);
		}
		//buff->setSkeletonName();


		weights=cluster->getBoneWeights(wTempStr.c_str());
		m_stream->read(&tmpI,sizeof(int));//vertices
		video::IHardwareStreamBuffer* vb= buff->getStream(0,video::EMST_Position);
		weights->vertices.resize(vb->length());
		int vI;
		float vW;
		float*vertArr=&weights->vertices[0];
		memset(vertArr,0,vb->length()*sizeof(float));
		for(int i=0;i<tmpI;++i){
			m_stream->read(&vI,sizeof(int));
			m_stream->read(&vW,sizeof(float));
			vertArr[vI]=vW;
		}
	}
	scene::BoneNode*b= skeleton->getBone(wTempStr);
	if(!b)
		return true;
	const scene::BonesList& bones=b->getSubBones();
	scene::BonesList::const_iterator it=bones.begin();
	for (;it!=bones.end();++it)
	{
		loadWeights(mesh,skeleton);
	}

	return true;
}

scene::SMesh*mdlLoader::load(const  core::string&filename)
{
	if(!canLoad(filename))return 0;
	OS::IStreamPtr file=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!file)return 0;
	return load(file);
}

scene::SMesh*mdlLoader::load(OS::IStream* file)
{
	int totalMeshs;
	int totalCycles;
	int totalLocators;
	int hasCollisionSphere=0;
	bool texFromFiles;
	bool hasSkeleton=false;

	m_stream=file;

	file->seek(0,OS::ESeek_Set);

	file->read(&totalMeshs,sizeof(int));
	file->read(&hasCollisionSphere,sizeof(int));
	file->read(&hasSkeleton,sizeof(bool));

	scene::SMesh* mesh=new scene::SMesh();
	
//	AnimMesh->collisionSphere=collSphere;
	file->read(&totalCycles,sizeof(int));

	scene::MeshBufferData*meshBufferD;
	scene::IMeshBuffer*meshBuffer1;
	char name[32];
	int totalFrames=0;
	
	scene::SCycle* cycles=new scene::SCycle;
//	mesh->setCycles(cycles);
	cycles->totalCycles=totalCycles;
	cycles->CycleProp.resize(totalCycles);//=new scene::SCycle::scene::SCycleProp[totalCycles];

    //reading cycles name and count	
	for(int i=0;i<totalCycles;++i)
	{
		file->read(&cycles->CycleProp[i].count,sizeof(int));
		file->read(name,32*sizeof(char));
		cycles->CycleProp[i].name=name;
		for(int j=0;j<(cycles->CycleProp[i].count);j++)
		{
			totalFrames++;
			int tmp;
			file->read(&tmp,sizeof(int));
			cycles->CycleProp[i].cycleIndex.push_back(tmp);
		}
	}
//	AnimMesh->getMesh()->Mesh.reallocate(totalMeshs);

    //reading Locators
	file->read(&totalLocators,sizeof(int));
/*
	for(int i=0;i<totalLocators;++i)
	{
		 mchar locName[32];
		 file->read(locName,32*sizeof( mchar));
		 scene::MountPoint* pt=new scene::MountPoint(locName);;

	//	char_to_wchar(wTempStr,locName);

		 bool first=false;
		scene::SLocator *loc=new scene::SLocator(( mchar*)locName,0);
		if(!loc->frames)loc->frames=new scene::SLocator::FramesAttr;
		math::vector3d pos,rot,scale;
		for(int c=0;c<totalCycles;c++){
			for(int j=0;j<(cycles->CycleProp[c].count);j++)
			{
				math::vector3d v;
				file->read(&v,sizeof(math::vector3d));
				v.x*=-1;
				loc->frames->position.push_back(v);
				if(!first){
					pt->setPosition(v);
				}
				file->read(&v,sizeof(math::vector3d));
				if(!first){
					pt->setOrintation(math::quaternion(v));
				}
				loc->frames->orintation.push_back(math::quaternion(v));
				file->read(&v,sizeof(math::vector3d));
				loc->frames->scale.push_back(v);
			}
		}
		cycles->locators.push_back(loc);
	//	loc->addRef();
	//	AnimMesh->addChild(loc);
	}
*/
	core::string tname;
	file->read(&texFromFiles,sizeof(bool));
	for(int obj=0;obj<totalMeshs;obj++)
	{
		bool haRenderMaterial;
		meshBufferD=mesh->addNewBuffer();
		meshBuffer1=meshBufferD->getMeshBuffer();

		file->read(name,32*sizeof(char));
		tname=core::StringConverter::toString(name);
		meshBuffer1->setMeshName(tname);
		math::vector3d tmpV;
		math::quaternion tmpQ;
		file->read(&tmpV,sizeof(math::vector3d));
//		meshBuffer1->setPosition(tmpV);
		file->read(&tmpV,sizeof(tmpV));
		
		tmpQ=math::quaternion(tmpV);
//		meshBuffer1->setRotation(tmpQ);
		file->read(&tmpV,sizeof(math::vector3d));
//		meshBuffer1->setScale(tmpV);
		file->read(&haRenderMaterial,sizeof(bool));
#if 0
		if(haRenderMaterial){
			video::SColor clr;
			float tmpF;
			file->read(&clr,sizeof(video::SColor));
			meshBufferD->getMaterial()->SetAmbient(clr);
			file->read(&clr,sizeof(video::SColor));
			meshBufferD->getMaterial()->SetDiffuse(clr);
			file->read(&clr,sizeof(video::SColor));
			meshBufferD->getMaterial()->SetSpecular(clr);
			file->read(&clr,sizeof(video::SColor));
			meshBufferD->getMaterial()->SetEmissive(clr);
			file->read(&tmpF,sizeof(float));
			meshBufferD->getMaterial()->SetShininess(tmpF);
			file->read(&tmpF,sizeof(float));
			meshBufferD->getMaterial()->SetAlpha(tmpF);

			for(int s=0;s<video::RS_RenderStates_Count;++s){
				video::EState st;
				file->read(&st,sizeof(video::EState));
				meshBufferD->getMaterial()->setRenderState((video::RenderStates)s,st);
			}
			//Old file format
			int tmpI;
			file->read(&tmpI,sizeof(int));
			file->read(&tmpF,sizeof(float));
			file->read(&tmpF,sizeof(float));


			 mchar texName[64];
		//	 mchar tempTex[64];
			file->read(texName,sizeof( mchar)*64);
		//	char_to_wchar(tempTex,texName);
			if(compareStr(( mchar*)texName,mT(""))!=0){
				video::ITexturePtr tex=gTextureResourceManager.getResource(texName);
				GCPtr<video::TextureUnit> texUnit=new video::TextureUnit();
				if(!texFromFiles){
					bool b=0;
					file->read(&b,sizeof(bool));//we have a texture here!
					if(b && !tex){
						bool b[4];
						math::Point2di size;
						uchar depth;
						int dataType;
						int dataFormat;
						file->read(&b[0],sizeof(bool));
						file->read(&b[1],sizeof(bool));
						file->read(&b[2],sizeof(bool));
						file->read(&b[3],sizeof(bool));

						file->read(&size,sizeof(math::Point2di));
						file->read(&depth,sizeof(uchar));
						file->read(&dataType,sizeof(int));
						file->read(&dataFormat,sizeof(int));

						
						tex=gTextureResourceManager.createEmptyTexture2D(gFileSystem.getShortFileName(( mchar*)texName),true);

						tex->setBilinearFilter(b[0]);
						tex->setTrilinearFilter(b[1]);
						tex->setMipmapsFilter(b[2]);
						tex->setAnisotropicFilter(b[3]);

						video::ImageInfo info;
						info.createData(math::vector3d(size.x,size.y,1),(video::EPixelFormat)dataFormat);

						file->read((void*)info.imageData,info.imageDataSize*sizeof(uchar));
						video::ImageInfo*lst[1]={&info};
						tex->loadSurfaces(lst,1);

						gTextureResourceManager.addResource(tex,texName);
						meshBufferD->getMaterial()->setTexture(tex,0);
					}else{
				/*		if(tex)
							tex->addRef();*/
						meshBufferD->getMaterial()->setTexture(tex,0);
						if(b){
							//now we have to pass texture data
							math::Point2di imageSize;
							uchar colorDepth;
							bool b=0;
							file->read(&b,sizeof(bool));
							file->read(&b,sizeof(bool));
							file->read(&b,sizeof(bool));
							file->read(&b,sizeof(bool));

							file->read(&imageSize,sizeof(math::Point2di));
							file->read(&colorDepth,sizeof(uchar));
							file->seek(2*sizeof(int),OS::ESeek_Cur);
							int dataSize=imageSize.x*imageSize.y*colorDepth/8;
							file->seek(dataSize*sizeof(uchar),OS::ESeek_Cur);
						}
					
					}
				}else
				{
					tex=gTextureResourceManager.loadTexture2D(texName);
					meshBufferD->getMaterial()->setTexture(tex,0);
				}
				texUnit->SetTexture(tex);
			}
		}
#endif
		int NumberOfVertices;
		int NumberOfUV;
		int NumberOfTri;
		int NumberOfIndices;

		file->read(&NumberOfVertices,sizeof(int));
		file->read(&NumberOfUV,sizeof(int));
		file->read(&NumberOfTri,sizeof(int));
		NumberOfIndices=3*NumberOfTri;

		video::IHardwareStreamBuffer* vStream=meshBuffer1->createStream(0,video::EMST_Position,video::ESDT_Point3f,NumberOfVertices,
			video::IHardwareBuffer::EUT_Static,true);
		video::IHardwareStreamBuffer* nStream=meshBuffer1->createStream(0,video::EMST_Normal,video::ESDT_Point3f,NumberOfVertices,
			video::IHardwareBuffer::EUT_Static,true);
		video::IHardwareStreamBuffer* uvStream=meshBuffer1->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,NumberOfVertices,
			video::IHardwareBuffer::EUT_Static,true);
		int* invertedIndexPtr=meshBuffer1->getInvertedIndexPtr();

		math::vector2d uvCoord;
		math::vector3d vecf;
		math::Point3di veci;
		int i=0;
		//Reading UVCoords
//		meshBuffer1->setOrginalUVCount(NumberOfUV);
/*		math::vector2d* ouv= meshBuffer1->getOrginalUV();
		for(i=0;i<NumberOfUV;++i)
		{
			file->read(&uvCoord,2*sizeof(float));
			ouv[i]=uvCoord;
		}
*/
		m_tmpInd.resize(3*NumberOfTri);

		ushort*pind=&m_tmpInd[0];
		//Reading Indicies
		for(i=0;i<NumberOfTri;++i)
		{
			file->read(&veci,3*sizeof(int));
			pind[3*i]=(ushort)veci.x;
			pind[3*i+1]=(ushort)veci.y;
			pind[3*i+2]=(ushort)veci.z;
		}
		video::IHardwareIndexBuffer*indBuff=meshBuffer1->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpInd.size(),video::IHardwareIndexBuffer::EUT_Static,true);
		indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpInd[0]);
		//Reading UVTriangles Coord
		m_tmpTri.resize(NumberOfTri);
		for(i=0;i<NumberOfTri;++i)
		{
			file->read(&veci,3*sizeof(int));

			m_tmpTri[i].x=veci.x;
			m_tmpTri[i].y=veci.y;
			m_tmpTri[i].z=veci.z;
// 			pind[3*i]=veci.x;
// 			pind[3*i+1]=veci.z;
// 			pind[3*i+2]=veci.y;
		}

		math::vector2d* tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
/*
		ouv=meshBuffer1->getOrginalUV();
		//calculate Texture coord
		for(int i=0;i<NumberOfTri;++i)
		{
			//get the right coord

			/ *
			Example:  //draft ;)
			indices:
			{0,1,2,
			 0,2,5
			 }
			 uvTriangle:
			 {
			 0,1,2
			 0,2,3
			 }
			    0-------3
			   /-\+++++/
			  /---\+++/
			 /-----\+/
			1-------2 

			uvCoord:
			before:
			0,1,2,3,4,5,....
			after:
			indices:	0,1,2,---,5
			connection: | | |     |
			uvCoord:	0,1,2,-,-,3

//			* /
			tc[m_tmpInd[3*i  ]]=ouv[m_tmpTri[i].x];
			tc[m_tmpInd[3*i+1]]=ouv[m_tmpTri[i].y];
			tc[m_tmpInd[3*i+2]]=ouv[m_tmpTri[i].z];
		}
*/
		
		//reading number of frames for each cycle
		scene::SMorphMeshBuffer* morphTarget;
		i=0;
		math::box3d box1;
		bool firstCycle=true;
		int f=0;

		math::box3d totalB;

		meshBufferD->setMorphAnimator(scene::MeshAnimatorManager::getInstance().CreateMorphAnimator(meshBuffer1));

		math::vector3d* vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
		math::vector3d* norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

		animation::MorphAnimationTrack* track;
		for(int j=0;j<cycles->totalCycles;j++)
		{
			//j		: for cycles
			//obj	: for objects
			//n		: for frames
			int ccnt=cycles->CycleProp[j].count;

			track=new animation::MorphAnimationTrack(meshBufferD->getMorphAnimator(),cycles->CycleProp[i].name,0,100);

			for(int n=0;n<ccnt;n++)
			{
				morphTarget=new scene::SMorphMeshBuffer(meshBuffer1);
				//reading Bounding Box for frame n
				math::vector3d tmp;
				file->read(&tmp,3*sizeof(float));
				box1.MinP.x=tmp.x;
				box1.MinP.y=tmp.z;
				box1.MinP.z=tmp.y;
				file->read(&tmp,3*sizeof(float));
				box1.MaxP.x=tmp.x;
				box1.MaxP.y=tmp.z;
				box1.MaxP.z=tmp.y;

				morphTarget->setBoundingBox(box1);

				video::IHardwareStreamBuffer* tvStream=morphTarget->createStream(0,video::EMST_Position,video::ESDT_Point3f,NumberOfVertices,
					video::IHardwareBuffer::EUT_WriteOnly,true,false);
				video::IHardwareStreamBuffer* tnStream=morphTarget->createStream(0,video::EMST_Normal,video::ESDT_Point3f,NumberOfVertices,
					video::IHardwareBuffer::EUT_WriteOnly,true,false);
				math::vector3d* tvert=(math::vector3d*)tvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
				math::vector3d* tnorm=(math::vector3d*)tnStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

				if(firstCycle){
					totalB=box1;
				}else
					totalB.AddBox(box1);
				//reading vertices for frame n
				for(int k=0;k<NumberOfVertices;k++)
				{
					file->read(&tvert[k].x,sizeof(float));
					file->read(&tvert[k].y,sizeof(float));
					file->read(&tvert[k].z,sizeof(float));
					
					file->read(&tnorm[k].x,sizeof(float));
					file->read(&tnorm[k].y,sizeof(float));
					file->read(&tnorm[k].z,sizeof(float));

					file->read(&invertedIndexPtr[k],sizeof(uint));


					if(firstCycle){
						vert[k]=tvert[k];
						norm[k]=tnorm[k];
					}
					i++;
				}
				if(firstCycle){
					meshBufferD->getMorphAnimator()->setTarget(0,morphTarget,1);
				}
				track->addKeyFrame(track->createKeyFrame(cycles->CycleProp[j].cycleIndex[n],morphTarget));
				meshBufferD->addAnimationTrack(track);
				meshBuffer1->addMorphTarget(morphTarget);
				tvStream->unlock();
				tnStream->unlock();
				f++;
				firstCycle=false;
			}
			file->read(&cycles->CycleProp[j].start,sizeof(int));
			file->read(&cycles->CycleProp[j].end,sizeof(int));
		}
		meshBuffer1->setBoundingBox(totalB);

		vStream->unlock();
		nStream->unlock();
		uvStream->unlock();
	}

	//scene::TangentCalculater tangCalc;
	//tangCalc.calculateTangents(mesh);

	if(hasSkeleton)
	{
		scene::Skeleton* skeleton=readSkeleton();
		if(!skeleton){
			gLogManager.log(mT("Skeleton Data couldn't be read!"),file->getStreamName(),ELL_WARNING);
		}
		else
		{
			mesh->setSkeleton(skeleton);
			loadWeights(mesh,skeleton);
		}

	}

// 	AnimMesh->animate(0);
// 	AnimMesh->getUpdateBoundingBox();
	
	mesh->calcCurrBoundingBox();

	gLogManager.log(mT("MDL mesh file loaded"),file->getStreamName(),ELL_SUCCESS);
	return mesh;
 
}
bool mdlLoader::canLoad(const  core::string&name)
{
	core::string path,ext;
	core::StringUtil::SplitPathExt(name, path, ext);
	return ext.equals_ignore_case(mT("mdl"));
}


const core::string g_sMDLExt=mT("mdl");
const core::string&mdlLoader::getExtension(){
	return g_sMDLExt;
}




}
}
