#include "stdafx.h"



#include "CConvexQuickHull.h"


namespace mray{
namespace scene{


CConvexQuickHull::CConvexQuickHull(GCPtr<scene::SMesh>mesh):IConvexHull()
{
	createConvexHull(mesh);
}

CConvexQuickHull::~CConvexQuickHull(){
	vtxCopy.clear();
}


void CConvexQuickHull::copyVertices(){
	if(!convexMesh)return;

	GCPtr<video::IHardwareStreamBuffer> vStream=convexMesh->getStream(0,video::EMST_Position);
	
	vtxCopy.resize(vStream->length());
	for(int i=0;i<vStream->length();++i){
		vtxCopy[i]=i;
	}

}

bool CConvexQuickHull::getExtreme(IConvexHull::SFace &face,int &bestV){
	math::Plane p=face.plane;
	float dist,best=0;
	bool ok=0;
	GCPtr<video::IHardwareStreamBuffer> vStream=convexMesh->getStream(0,video::EMST_Position);
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	for(int i=0;i<face.vtx.size();++i){
		dist=p.getDistance(vert[face.vtx[i]]);
		if(dist>best){
			ok=1;
			best=dist;
			bestV=face.vtx[i];
		}
	}
	return ok;
}

bool CConvexQuickHull::getMinMax(int &p1,int &p2,int &p3){
	if(!vtxCopy.size())return 0;
	
	p1=p2=p3=vtxCopy[0];
	GCPtr<video::IHardwareStreamBuffer> vStream=convexMesh->getStream(0,video::EMST_Position);
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	for(int i=1;i<vtxCopy.size();++i){
		if(vert[vtxCopy[i]].x<vert[p1].x){
			p1=vtxCopy[i];
		}else if(vert[vtxCopy[i]].x>vert[p2].x){
			p2=vtxCopy[i];
		}
		else if(vert[vtxCopy[i]].y>vert[p3].y)
			p3=vtxCopy[i];		
	}
	return 1;
}

void CConvexQuickHull::addVerticesToFace(std::vector<int>&vertices,IConvexHull::SFace &face){
	std::vector<int>vout;
//	vout.set_free_when_destroyed(0);
	face.vtx.clear();

	math::Plane p=face.plane;
	GCPtr<video::IHardwareStreamBuffer> vStream=convexMesh->getStream(0,video::EMST_Position);
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	for(int i=0;i<vertices.size();++i){
		if(vertices[i]==face.tri[0] || vertices[i]==face.tri[1] || vertices[i]==face.tri[2])
			continue;
		int dir=p.getVertLocation(vert[vertices[i]]);
		if(dir>=0){
			face.vtx.push_back(vertices[i]);
		}
		if(dir<0)
			vout.push_back(vertices[i]);
	}
	vertices=vout;//.set_pointer(&vout[0],vout.size());

}


void CConvexQuickHull::createConvexHull(GCPtr<scene::SMesh>mesh){
	if(!mesh)return;
	convexFaces.clear();
	vtxCopy.clear();
	copyMesh(mesh);
	copyVertices();


	GCPtr<video::IHardwareStreamBuffer> vStream=convexMesh->getStream(0,video::EMST_Position);
	math::vector3d*vtx=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

	int p1,p2,p3;
	if(!getMinMax(p1,p2,p3))return;

	SFace face1;
	face1.setTri(vtx[p1],vtx[p2],vtx[p3],p1,p2,p3);
	SFace face2;
	face2.setTri(vtx[p1],vtx[p3],vtx[p2],p1,p3,p2);

	addVerticesToFace(vtxCopy,face1);
	addVerticesToFace(vtxCopy,face2);

	convexFaces.push_back(face1);
	convexFaces.push_back(face2);
	int vextreme=0;

	std::vector<std::list<SFace>::iterator>faceToDelete;
	std::vector<int>tmpArray;

	std::list<SFace>::iterator it=convexFaces.begin();
	std::list<SFace>::iterator it2;


	int cnt=0;
	for(;it!=convexFaces.end() ;){
		faceToDelete.clear();
		bool postRemove=0;
		if(getExtreme(*it,vextreme)){
			faceToDelete.push_back(it);
			tmpArray.resize((*it).vtx.size());
			mraySystem::memCopy(&tmpArray[0],&(*it).vtx[0],(*it).vtx.size()*sizeof(int));
			it2=convexFaces.begin();
			for(;it2!=convexFaces.end();it2++){
				if(it==it2)
					continue;
				if((*it2).plane.getVertLocation(vtx[vextreme])>0){
					int lastSz=tmpArray.size();
					tmpArray.resize(tmpArray.size()+(*it2).vtx.size());
					mraySystem::memCopy(&tmpArray[0]+lastSz,&(*it2).vtx[0],(*it2).vtx.size()*sizeof(int));
					faceToDelete.push_back(it2);
				}
			}
			//find hole boundry
			std::vector<int> bound;
			for(int i=0;i<faceToDelete.size();++i){
				if(faceToDelete.size()==1){
					bound.push_back((*faceToDelete[0]).tri[0]);
					bound.push_back((*faceToDelete[0]).tri[1]);
					bound.push_back((*faceToDelete[0]).tri[2]);
				}
				else
				for(int j=i+1;j<faceToDelete.size();j++){
					//if(i==j)continue;
					int match=0;
					for(int k=0;k<3 && match<2;k++){
						match=0;
						for(int l=0;l<3;l++){
							if((*faceToDelete[i]).tri[k]==(*faceToDelete[j]).tri[l]){
								match=1;
								break;
							}
						}
						if(!match)
							bound.push_back((*faceToDelete[i]).tri[k]);
					}
					for(int l=0;l<3;l++){
						bound.push_back((*faceToDelete[j]).tri[l]);
					}
				}
				//delete face
				if(faceToDelete[i]==it){
					postRemove=1;
				}else
					convexFaces.erase(faceToDelete[i]);
			}
			faceToDelete.resize(0);
			//now add new  faces

			for(int i=0;i<bound.size()-1;++i){
				face1.setTri(vtx[bound[i]],vtx[bound[i+1]],vtx[vextreme],bound[i],bound[i+1],vextreme);
				face1.vtx.clear();
				addVerticesToFace(tmpArray,face1);
				convexFaces.push_back(face1);
			}
			face1.setTri(vtx[bound[bound.size()-1]],vtx[bound[0]],vtx[vextreme],bound[bound.size()-1],bound[0],vextreme);
			addVerticesToFace(tmpArray,face1);
			convexFaces.push_back(face1);
			tmpArray.resize(0);
			cnt++;
		}

		if(postRemove){
			std::list<SFace>::iterator itp=it;
			itp++;
			convexFaces.erase(it);
			it=itp;
		}else
			++it;
		if(it==convexFaces.end() || cnt==4)//
			break;
	}
	tmpArray.clear();
	faceToDelete.clear();

	vStream->unlock();

	//now its time for indices...

	std::vector<ushort> tmpInd;

	tmpInd.resize(3*convexFaces.size());
	ushort*pind=&tmpInd[0];
	it=convexFaces.begin();
	int n=0;
	for(;it!=convexFaces.end();++it){
		pind[3*n+0]=(*it).tri[0];
		pind[3*n+1]=(*it).tri[2];
		pind[3*n+2]=(*it).tri[1];

		n++;
	}
	GCPtr<video::IHardwareIndexBuffer>indBuff=convexMesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,tmpInd.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&tmpInd[0]);
}

}
}


