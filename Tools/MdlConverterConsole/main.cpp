
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
#include "mTypes.h"

#include <matrix4x4.h>
#include <quaternion.h>


using namespace std;
using namespace mray;
using namespace math;

float weightTrunc=0.01f;


struct VERTEX
{
	VERTEX():invertedIndex(0){}
	float x,y,z;
	float nx,ny,nz;
	uint invertedIndex;
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
struct UVTRIANGLE
{
	float u,v;
};


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

		for (int j=i+1;j<isize;j++)
		{
			TriangleList&v2=indexList[j];
			for (int k1=0;k1<3;k1++)
			{
				for (int k2=0;k2<3;k2++)
				{
					//we can check for multi uv channel here
					if(v1.Verticies[k1]==v2.Verticies[k2] && 
						v1.texCoords[k1]!=v2.texCoords[k2])
					{
						if(v2.Verticies[k2]>=numVert)
						{
							printf("error:duplicated index exceeded number of vertecies\n");
						}
						duplicateVert.push_back(v2.Verticies[k2]);
						v2.Verticies[k2]=numVert+duplicateVert.size()-1;
					}
				}
			}
		}
	}
}

int char_to_wchar(wchar_t*a,const char*b){
	int len=strlen(b);
	int k=0;
	int i=0;
	for(;i<len;++i)
		a[i]=b[i];
	a[i]=0;
	return i;
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
#ifdef BLAAAAAAA
	matrix4x4f mat1,mat2;
	//	if(skeleton->parent)
	//		mat2.setTranslation(skeleton->parent->position);
	quaternion qx;
	quaternion qy;
	quaternion qz;
	quaternion q;
	if(skeleton->parent){
		/*	qx.fromAngle(skeleton->parent->ori.x,vector3d(1,0,0));
		qy.fromAngle(skeleton->parent->ori.y,vector3d(0,1,0));
		qz.fromAngle(skeleton->parent->ori.z,vector3d(0,0,1));

		q=qz*qy*qx;*/

		if(0){
			pos=orient(skeleton->parent->ori,pos);
			pos+=skeleton->parent->position;

			matrix4x4f m1,m2;
			m1.setRotationDegrees(skeleton->parent->ori);
			m2.setRotationDegrees(ori);
			m1*=m2;
			skeleton->ori=m1.getRotation();
		}

		//	mat1.setRotationDegrees(skeleton->parent->ori);
		//	q.toMatrix(mat1);
		//	mat1.setTranslation(skeleton->parent->position);
		//	mat2.setTranslation(pos);
		//	mat1*=mat2;
		//mat1.transformVectPost(pos);
		/*	q.fromMatrix(mat1);*/

		//	skeleton->position=skeleton->parent->position+q*pos;
		//	skeleton->position=pos;
		skeleton->position=pos;//q*pos+skeleton->parent->position;//mat1.getTranslation()+skeleton->parent->position;
		//skeleton->position=mat1.getTranslation()+skeleton->parent->position;
	}else{
		skeleton->position=pos;
		skeleton->ori=ori;
	}
	//	mat1.transformVect(pos,skeleton->position);
	if(ori.x || ori.y|| ori.z)
	{
		printf("");
	}
#else

#endif
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
void writeJoints(FILE*fOut,SSkeleton*skeleton){
	if(!skeleton)return;
	int tempI;
	fwrite(skeleton->name.c_str(),1,32*sizeof(char),fOut);
	fwrite(&skeleton->position,1,sizeof(vector3d),fOut);
	fwrite(&skeleton->ori,1,sizeof(vector3d),fOut);
	tempI=skeleton->children.size();
	fwrite(&tempI,1,sizeof(int),fOut);
	list<SSkeleton*>::iterator it=skeleton->children.begin();
	for(;it!=skeleton->children.end();++it){
		writeJoints(fOut,*it);
	}
}
void writeWeights(FILE*fOut,SSkeleton*skeleton){
	if(!skeleton)return;
	int tempI;
	fwrite(skeleton->name.c_str(),1,32*sizeof(char),fOut);
	fwrite(&(tempI=skeleton->mWeights.size()),1,sizeof(int),fOut);

	list<SSkeleton::meshWights>::iterator itm=skeleton->mWeights.begin();
	for(;itm!=skeleton->mWeights.end();itm++){
		fwrite((*itm).meshName.c_str(),1,32*sizeof(char),fOut);
		fwrite(&(tempI=(*itm).weights.size()),1,sizeof(int),fOut);

		list<SSkeleton::SWeights>::iterator itw=(*itm).weights.begin();
		for(;itw!=(*itm).weights.end();itw++){
			fwrite(&(*itw).vtx,1,sizeof(int),fOut);
			fwrite(&(*itw).weights,1,sizeof(float),fOut);
		}
	}
	list<SSkeleton*>::iterator it=skeleton->children.begin();
	for(;it!=skeleton->children.end();++it){
		writeWeights(fOut,*it);
	}
}

void readCollisionSpheres(fstream&file,FILE*&fOut){
	
	int numberOfChilds;
	file>>numberOfChilds;
	
	fwrite(&numberOfChilds,1,sizeof(int),fOut);
	vector3d pos;
	float scale;
	file>>pos.x;
	file>>pos.y;
	file>>pos.z;
	file>>scale;
	fwrite(&pos,1,sizeof(vector3d),fOut);
	fwrite(&scale,1,sizeof(float),fOut);

	for(int i=0;i<numberOfChilds;++i){
		readCollisionSpheres(file,fOut);
	}
}

void Load(char*filename,FILE*fOut)
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


	file>>hasSkeleton;
	file>>TotalObjects;
	fwrite(&TotalObjects,1,sizeof(int),fOut);

	printf("Total Objects : %d\n",TotalObjects);

	int hasCollisionSphere=0;
	file>>hasCollisionSphere;
	fwrite(&hasCollisionSphere,1,sizeof(int),fOut);
	if(hasCollisionSphere){
		printf("hasCollisionSphere..\n");
		readCollisionSpheres(file,fOut);
	}
	fwrite(&hasSkeleton,1,sizeof(hasSkeleton),fOut);


	file>>TotalCycles;
	fwrite(&TotalCycles,1,sizeof(int),fOut);
	Frames		=new _FrameKeys[TotalCycles];

	printf("Cycles Count : %d\n",TotalCycles);

	int FrameIndex=0;
	for(int i=0;i<TotalCycles;++i)
	{
		Frames[i].count=0;
		file>>Frames[i].count;
		fwrite(&Frames[i].count,1,sizeof(int),fOut);
		file>>FrameName;

		printf("Cycles[%d]:\n",i);
		printf("\tName: %s\n",FrameName);
		printf("\tFrames: %d\n",Frames[i].count);

		fwrite(FrameName,32,sizeof(char),fOut);
		for(int j=0;j<Frames[i].count;j++)
		{
			file>>FrameIndex;
			fwrite(&FrameIndex,1,sizeof(int),fOut);
		}
	}

	
	file>>TotalLocators;
	fwrite(&TotalLocators,1,sizeof(int),fOut);

	printf("Locators Countt : %d\n",TotalLocators);

	for(int i=0;i<TotalLocators;++i){
		char loc_name[32];
		vector3d pos,rotate,scale;
		file>>loc_name;
		wchar_t tempW[150];
		char_to_wchar(tempW,loc_name);
		fwrite(tempW,1,32*sizeof(wchar_t),fOut);
		printf("curr Locator : %s\n",loc_name);
		for(int c=0;c<TotalCycles;c++)
		{
			for(int j=0;j<Frames[c].count;j++){
				file>>pos.x;
				file>>pos.y;
				file>>pos.z;
				
				file>>rotate.x;
				file>>rotate.y;
				file>>rotate.z;
				
				file>>scale.x;
				file>>scale.y;
				file>>scale.z;
					
				fwrite(&pos,1,sizeof(vector3d),fOut);
				fwrite(&rotate,1,sizeof(vector3d),fOut);
				fwrite(&scale,1,sizeof(vector3d),fOut);
			}
		}
	}

	vector3d Pos,Rot,scale;
	printf("Exporting objects..\n");
	bool texFromFiles=false;
	fwrite(&texFromFiles,1,sizeof(bool),fOut);
	for(int k=0;k<TotalObjects;k++)
	{
		file>>objName;
		printf(">object Name : %s\n",objName);

		file>>Pos.x>>Pos.y>>Pos.z;
		file>>Rot.x>>Rot.y>>Rot.z;
		file>>scale.x>>scale.y>>scale.z;
		fwrite(objName,1,32*sizeof(char),fOut);
		fwrite(&Pos,1,sizeof(vector3d),fOut);
		fwrite(&Rot,1,sizeof(vector3d),fOut);
		fwrite(&scale,1,sizeof(vector3d),fOut);
		bool hasMaterial=0;
		fwrite(&hasMaterial,1,sizeof(bool),fOut);
/*		fscanf(file,"%f %f %f %f %f %f\n",&MinPoint.x,&MinPoint.y,&MinPoint.z,
			&MaxPoint.x,&MaxPoint.y,&MaxPoint.z);
		fwrite(&MinPoint,1,sizeof(vector3d),fOut);
		fwrite(&MaxPoint,1,sizeof(vector3d),fOut);
*/
		file>>numVert>>numTri>>numUV;

		verticies.resize(numVert);

		printf("Exporting UVs..\n");

		texcoords.resize(numUV);

		for(int i=0;i<numUV;++i)
		{
			file>>uv.u>>uv.v;
			texcoords[i]=uv;
		}
		indexList.resize(numTri);

		for(int i=0;i<numTri;++i)
		{
			file>>tri.i1>>tri.i2>>tri.i3;
			indexList[i].Verticies=tri;
			//fwrite(&tri,1,sizeof(TRIANGLE),fOut);
		}
		for(int i=0;i<numTri;++i)
		{
			file>>tri.i1>>tri.i2>>tri.i3;
			indexList[i].texCoords=tri;
// 			file>>uvTri.v1>>uvTri.v2>>uvTri.v3;
// 			fwrite(&uvTri,1,sizeof(TRIANGLE),fOut);
		}
		//fixIndicies();

		int tNumVert=numVert+duplicateVert.size();
		fwrite(&tNumVert,1,sizeof(int),fOut);
		fwrite(&numUV,1,sizeof(int),fOut);
		fwrite(&numTri,1,sizeof(int),fOut);

		for(int i=0;i<numUV;++i)
		{
			fwrite(&texcoords[i],1,sizeof(UVTRIANGLE),fOut);
		}
		printf("Exporting Triangles..\n");
		for(int i=0;i<numTri;++i)
		{
			fwrite(&indexList[i].Verticies,1,sizeof(TRIANGLE),fOut);
		}
		for(int i=0;i<numTri;++i)
		{
			fwrite(&indexList[i].texCoords,1,sizeof(TRIANGLE),fOut);
		}
		/*
		for(int i=0;i<numVert;++i)
		{
			float color[3];
			file>>color[0]>>color[1]>>color[2];
			//fwrite(color,1,sizeof(color),fOut);
		}*/
		char FrameName[32],LastName[32];
		int x=0,n=0,frameC=0;
		int i=0;
		printf("Exporting Cycles..\n");
		for(int j=0;j<TotalCycles;j++)
		{
			Frames[j].start=i;
			for(int k=0;k<Frames[j].count;k++)
			{
				file>>MinPoint.x>>MinPoint.y>>MinPoint.z>>
				MaxPoint.x>>MaxPoint.y>>MaxPoint.z;
				
				fwrite(&MinPoint,1,sizeof(vector3d),fOut);
				fwrite(&MaxPoint,1,sizeof(vector3d),fOut);
				
				for(n=0;n<numVert;n++)
				{
					file>>vert.x>>vert.y>>vert.z>>
					vert.nx>>vert.ny>>vert.nz;
					verticies[n]=vert;
					verticies[n].invertedIndex=n;

					fwrite(&vert,1,sizeof(VERTEX),fOut);
				}
				for (int n=0;n<duplicateVert.size();n++)
				{
					int idx=duplicateVert[n];
					fwrite(&verticies[idx],1,sizeof(VERTEX),fOut);
				}
				i++;
			}
			Frames[j].end=i-1;
			fwrite(&Frames[j].start,1,sizeof(int),fOut);
			fwrite(&Frames[j].end,1,sizeof(int),fOut);
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

		list<SSkeleton*>::iterator it=skeletons.begin();

		writeJoints(fOut,(*it));
		it=skeletons.begin();
		writeWeights(fOut,(*it));
		(*it)->clear();
		skeletons.clear();

	}
	delete [] Frames;
	file.close();
		
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
		Load(files[i],fOut);

		fclose(fOut);
	}
	getchar();

	return 0;
}
