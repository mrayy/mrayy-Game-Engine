
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>	
#include <fstream>	
#include <iostream>	
#include <vector>	
#include <time.h>	
#include <Point3d.h>	
#include <assert.h>
#include <list>

#include <matrix4x4.h>
#include <quaternion.h>

#include <mrayEngine.h>
#include <GLDev.h>
#include <SMorphMeshBuffer.h>
#include <MeshSerializer.h>

using namespace std;
using namespace mray;
using namespace math;

float weightTrunc=0.01f;


struct VERTEX
{
	float x,y,z;
	float nx,ny,nz;
};
struct TRIANGLE
{
	int i1,i2,i3;
	int& operator[](int i){
		assert(i<3);
		if(i==0)return i1;
		if(i==1)return i2;
		return i3;
	}
};
struct TriangleList
{
	TRIANGLE Verticies,texCoords;
};
typedef  math::vector2di UVTRIANGLE;


struct SSkeleton{
	SSkeleton(SSkeleton*_parent):parent(_parent){};
	SSkeleton* getJoint(const char*_name){
		if(name.compare(_name)==0)return this;
		SSkeleton*j=0;
		list<SSkeleton*>::iterator it=children.begin();
		for(;it!=children.end();++it){
			j=(*it)->getJoint(_name);
			if(j)return j;
		}
		return 0;

	}
	SSkeleton*parent;

	string name;
	vector3d position;
	vector3d ori;

	list<SSkeleton*>children;
	void clear(){
		list<SSkeleton*>::iterator it=children.begin();
		for(;it!=children.end();++it)
			(*it)->clear();
		children.clear();
		mWeights.clear();;
	}
	struct SWeights{
		float weights;
		int  vtx;
	};
	struct meshWights{
		string meshName;
		list<SWeights>weights;
		~meshWights(){weights.clear();}
	};
	list<meshWights> mWeights;
};

list<SSkeleton*> skeletons;

std::vector<VERTEX> verticies;
std::vector<UVTRIANGLE> texcoords;
std::vector<TriangleList> indexList;
std::vector<int> duplicateVert;

int numVert;
int numTri;
int numUV;
int numFrames;

vector3d scale;
vector3d MaxPoint,MinPoint;



char jointName[64];

struct SMeshJoint{
	string meshName;
	string jointName;
};



struct _FrameKeys
{
	char FrameName[32];
	int  start,end;
	int	 count;
};

_FrameKeys *Frames;


void fixIndicies(){
	int isize=indexList.size();
	duplicateVert.clear();
	for (int i=0;i<isize;++i)
	{
		TriangleList&v1=indexList[i];
		for (int j=i+1;j<isize;j++){
			TriangleList&v2=indexList[j];
			for (int k1=0;k1<3;k1++)
			{
				for (int k2=0;k2<3;k2++)
				{
					if(v1.Verticies[k1]==v2.Verticies[k2] && 
						v1.texCoords[k1]!=v2.texCoords[k2])
					{
						if(v2.Verticies[k2]>=numVert){
							printf("err");
						}
						duplicateVert.push_back(v2.Verticies[k2]);
						v2.Verticies[k2]=numVert+duplicateVert.size()-1;
					}
				}
			}
		}
	}
}


void readJoints(fstream&file,SSkeleton*skeleton){
	file>>jointName;
	skeleton->name=jointName;
	vector3d pos,ori;
	file>>pos.x>>pos.y>>pos.z;
	file>>ori.x>>ori.y>>ori.z;
	//	pos.x*=-1;
	skeleton->position=pos;
	skeleton->ori=ori;
	//skeleton->position=mat1.getTranslation();
	//	if(skeleton->parent)
	//		skeleton->position+=skeleton->parent->position;

	//	skeleton->position=pos;
	int cCnt;
	file>>cCnt;//childs count for this joint
	for(int i=0;i<cCnt;i++){
		SSkeleton*joint=new SSkeleton(skeleton);
		readJoints(file,joint);
		skeleton->children.push_back(joint);
	}
}
void readCollisionSpheres(fstream&file){
	
	int numberOfChilds;
	file>>numberOfChilds;
	
	vector3d pos;
	float scale;
	file>>pos.x;
	file>>pos.y;
	file>>pos.z;
	file>>scale;

	for(int i=0;i<numberOfChilds;++i){
		readCollisionSpheres(file);
	}
}

void Load(char*filename)
{
	fstream file;
	file.open(filename,ios::in);
	char objName[32];
	char FrameName[32];
	if(!file)return;
	int TotalObjects;
	int TotalCycles;
	int TotalCollSpheres;
	int TotalLocators;
	bool hasSkeleton;

	UVTRIANGLE uv;
	TRIANGLE tri;
	TRIANGLE uvTri;
	VERTEX vert;

	scene::SMesh mesh;

	file>>hasSkeleton;
	file>>TotalObjects;

	printf("Total Objects : %d\n",TotalObjects);

	int hasCollisionSphere=0;
	file>>hasCollisionSphere;
	if(hasCollisionSphere){
		printf("hasCollisionSphere..\n");
		readCollisionSpheres(file);
	}


	file>>TotalCycles;
	Frames		=new _FrameKeys[TotalCycles];

	printf("Cycles Count : %d\n",TotalCycles);

	int FrameIndex=0;
	for(int i=0;i<TotalCycles;++i)
	{
		Frames[i].count=0;
		file>>Frames[i].count;
		file>>FrameName;

		printf("Cycles[%d]:\n",i);
		printf("\tName: %s\n",FrameName);
		printf("\tFrames: %d\n",Frames[i].count);

		for(int j=0;j<Frames[i].count;j++)
		{
			file>>FrameIndex;
		}
	}

	
	file>>TotalLocators;

	printf("Locators Countt : %d\n",TotalLocators);

	vector3d Pos,Rot,scale;
	printf("Exporting objects..\n");
	bool texFromFiles=false;
	for(int k=0;k<TotalObjects;k++)
	{
		file>>objName;
		printf(">object Name : %s\n",objName);

		scene::MeshBufferData*bdata= mesh.addNewBuffer();
		scene::IMeshBuffer*buff= bdata->getMeshBuffer();

		file>>Pos.x>>Pos.y>>Pos.z;
		file>>Rot.x>>Rot.y>>Rot.z;
		file>>scale.x>>scale.y>>scale.z;
		buff->setMeshName(objName);

		file>>numVert>>numTri>>numUV;

		verticies.resize(numVert);

		printf("Exporting UVs..\n");

		texcoords.resize(numUV);

		for(int i=0;i<numUV;++i)
		{
			file>>uv.x>>uv.y;
			texcoords[i]=uv;
		}
		indexList.resize(numTri);

		for(int i=0;i<numTri;++i)
		{
			file>>tri.i1>>tri.i2>>tri.i3;
			indexList[i].Verticies=tri;
		}
		for(int i=0;i<numTri;++i)
		{
			file>>tri.i1>>tri.i2>>tri.i3;
			indexList[i].texCoords=tri;
// 			file>>uvTri.v1>>uvTri.v2>>uvTri.v3;
		}
		fixIndicies();

		int tNumVert=numVert+duplicateVert.size();

		video::IHardwareStreamBuffer*posStream= buff->createStream(0,video::EMST_Position,video::ESDT_Point3f,tNumVert,video::IHardwareBuffer::EUT_Static,0,true);
		video::IHardwareStreamBuffer*normalStream= buff->createStream(0,video::EMST_Normal,video::ESDT_Point3f,tNumVert,video::IHardwareBuffer::EUT_Static,0,true);
		video::IHardwareStreamBuffer*texStream= buff->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,tNumVert,video::IHardwareBuffer::EUT_Static,0,true);
		video::IHardwareStreamBuffer*colorStream= buff->createStream(0,video::EMST_Color,video::ESDT_Point3f,tNumVert,video::IHardwareBuffer::EUT_Static,0,true);
		video::IHardwareIndexBuffer*indBuff=buff->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,numTri*3,video::IHardwareIndexBuffer::EUT_Static,true);
		

		math::vector3d*pos=(math::vector3d*) posStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector3d*norm=(math::vector3d*) normalStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector2d*tex=(math::vector2d*) texStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector4d*clr=(math::vector4d*) colorStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

		ushort*indData=(ushort*)indBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);

		//printf("Exporting Triangles..\n");
		for(int i=0;i<numTri;++i)
		{
			indData[0]=indexList[i].Verticies.i1;
			indData[1]=indexList[i].Verticies.i2;
			indData[2]=indexList[i].Verticies.i3;
			indData+=3;
		}
		indBuff->unlock();

		for(int i=0;i<numTri;++i)
		{
			tex[indexList[i].Verticies.i1]=texcoords[indexList[i].texCoords.i1];
			tex[indexList[i].Verticies.i2]=texcoords[indexList[i].texCoords.i2];
			tex[indexList[i].Verticies.i3]=texcoords[indexList[i].texCoords.i3];
		}
		for(int i=0;i<tNumVert;++i)
		{
			float color[3];
			file>>color[0]>>color[1]>>color[2];
			clr[0].x=color[0];
			clr[1].y=color[1];
			clr[2].z=color[2];
			clr+=3;
		}
		char FrameName[32],LastName[32];
		int x=0,n=0,frameC=0;
		int i=0;
		printf("Exporting Cycles..\n");
		for(int j=0;j<TotalCycles;j++)
		{
			Frames[j].start=i;
			for(int k=0;k<Frames[j].count;k++)
			{
				scene::SMorphMeshBuffer* morphTarget=new scene::SMorphMeshBuffer(buff);
				file>>MinPoint.x>>MinPoint.y>>MinPoint.z>>
				MaxPoint.x>>MaxPoint.y>>MaxPoint.z;
				
				buff->setBoundingBox(math::box3d(MinPoint,MaxPoint));

				video::IHardwareStreamBuffer* tvStream=morphTarget->createStream(0,video::EMST_Position,video::ESDT_Point3f,tNumVert,
					video::IHardwareBuffer::EUT_WriteOnly,true,false);
				video::IHardwareStreamBuffer* tnStream=morphTarget->createStream(0,video::EMST_Normal,video::ESDT_Point3f,tNumVert,
					video::IHardwareBuffer::EUT_WriteOnly,true,false);
				math::vector3d* tvert=(math::vector3d*)tvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
				math::vector3d* tnorm=(math::vector3d*)tnStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
				
				for(n=0;n<numVert;n++)
				{
					file>>vert.x>>vert.y>>vert.z>>
					vert.nx>>vert.ny>>vert.nz;
					verticies[n]=vert;
					tvert[n].x=vert.x;
					tvert[n].y=vert.y;
					tvert[n].z=vert.z;
					tnorm[n].x=vert.nx;
					tnorm[n].y=vert.ny;
					tnorm[n].z=vert.nz;
				}
				for (int n=0;n<duplicateVert.size();n++)
				{
					int idx=duplicateVert[n];
					int k=n+numVert;
					tvert[k].x=verticies[idx].x;
					tvert[k].y=verticies[idx].y;
					tvert[k].z=verticies[idx].z;
					tnorm[k].x=verticies[idx].nx;
					tnorm[k].y=verticies[idx].ny;
					tnorm[k].z=verticies[idx].nz;

				}
				i++;
				buff->addMorphTarget(morphTarget);
			}
			Frames[j].end=i-1;
		}
	}


	SSkeleton*joint=0;
	if(hasSkeleton){
		printf("reading Skeletons.\n");
		joint=new SSkeleton(0);
		readJoints(file,joint);
		skeletons.push_back(joint);
	

		printf("reading weights..\n");

		string MeshName;

		int jointsCnt;
		char Name[64];
		int vtxCnt;

		for(int obj=0;obj<TotalObjects;obj++){
			file>>Name;
			MeshName=Name;
			file>>jointsCnt;
			file>>vtxCnt;
			string *jointsNames=new string[jointsCnt];

			float **vtxWeight=new float*[vtxCnt];
			for(int i=0;i<vtxCnt;i++)
				vtxWeight[i]=new float[jointsCnt];

			for(int i=0;i<jointsCnt;i++){
				file>>Name;
				jointsNames[i]=Name;
			}

			for(int i=0;i<vtxCnt;i++){
				for(int j=0;j<jointsCnt;j++){
					file>>vtxWeight[i][j];
				}
			}
			SSkeleton::meshWights weights;
			bool flag=0;
			SSkeleton*mj=0;
			for(int i=0;i<jointsCnt;i++){
				list<SSkeleton*>::iterator it=skeletons.begin();
				SSkeleton*j=0;
				strcpy(jointName,jointsNames[i].c_str());
				for(;it!=skeletons.end() && !j;++it)
					j=(*it)->getJoint(jointName);
				if(!j){
					printf("error reading joints,joint not found : %s..\n",jointName);
					return;
				}
				if(!flag){
					mj=j;
					flag=1;
				}
				SSkeleton::SWeights w;
				for(int v=0;v<vtxCnt;v++){
					if(vtxWeight[v][i]>weightTrunc){
						w.vtx=v;
						w.weights=vtxWeight[v][i];
						weights.weights.push_back(w);
					}
				}
				//duplicate vertices..
				for (int v=0;v<duplicateVert.size();++v)
				{
					int idx=duplicateVert[v];
					if(vtxWeight[idx][i]>weightTrunc){
						w.vtx=v+vtxCnt;
						w.weights=vtxWeight[idx][i];
						weights.weights.push_back(w);
					}
				}
				weights.meshName=MeshName;
				j->mWeights.push_back(weights);
				weights.weights.clear();
			}


			//	mj->mWeights.push_back(weights);
			//	weights.weights.clear();


			for(int i=0;i<vtxCnt;i++)
				delete [] vtxWeight[i];
			delete[] vtxWeight;
			delete[] jointsNames;
		}
	

		printf("writting Skeleton..\n");

	/*	list<SSkeleton*>::iterator it=skeletons.begin();

		writeJoints(fOut,(*it));
		it=skeletons.begin();
		writeWeights(fOut,(*it));
		(*it)->clear();
		skeletons.clear();*/

	}
	delete [] Frames;
	file.close();
	
	scene::MeshSerializer serializer;
	serializer.Serialize(&mesh,0);

	printf("Done..\n");

}


int main(int argc,char*argv[])
{
	FILE *fOut;
	
	printf("Welcom to M-RAY Converter\n");
	printf("          -----          \n");
	printf("this program convert m-ray ascii models to binary\n");
	printf("Enjoy :D\n\n");

	char szFName[MAX_PATH];
	char szFName2[MAX_PATH];
	int cnt=0;
	char**files;

	if(argc<2)
	{
		files=new char*[1];
		cnt=1;
		printf("File Name:");
		scanf("%s",szFName2);
		files[0]=szFName2;
	}else
	{
		cnt=argc-1;
		files=&argv[1];
		//strcpy(szFName2,argv[1]);
	}

	for(int i=0;i<cnt;++i){
		strcpy(szFName,files[i]);
		szFName[strlen(files[i])-3]='\0';
		strcat(szFName,"mdl");
		fOut=fopen(szFName,"wb");
		Load(files[i]);

		fclose(fOut);
	}
	getchar();

	return 0;
}
