
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>	
#include <fstream>	
#include <iostream>	
#include <matrix4x4.h>
#include <quaternion.h>

using namespace std;
using namespace mray;
using namespace math;




struct _FrameKeys
{
	char FrameName[32];
	int  start,end;
	int	 count;
};

struct KeyFrame
{
	vector3d pos;
	vector3d rot;
	vector3d scale;
};

struct AnimationTrack
{
	string name;
	vector<KeyFrame> keyFrames;
};

_FrameKeys *Frames;
char jointName[64];
vector3d jointPos;
vector3d jointScale;

vector3d orient(vector3d ori,vector3d pos){
	
/*	quaternion qx;
	quaternion qy;
	quaternion qz;
	quaternion q;
	qx.fromAngle(ori.x,vector3d(1,0,0));
	qy.fromAngle(ori.y,vector3d(0,1,0));
	qz.fromAngle(ori.z,vector3d(0,0,1));
	q=qx*qy*qz;
	return q*pos;*/
	matrix4x4 m;
	m.setRotationDegrees(ori);
	m.transformVectPre(pos);
	return pos;
}
vector3d orientRot(vector3d ori,vector3d rot){
	
/*	quaternion qx;
	quaternion qy;
	quaternion qz;
	quaternion q;
	qx.fromAngle(ori.x,vector3d(1,0,0));
	qy.fromAngle(ori.y,vector3d(0,1,0));
	qz.fromAngle(ori.z,vector3d(0,0,1));
	q=qx*qy*qz;
	return q*pos;*/
	matrix4x4 m1,m2,m3;
	m1.setRotationDegrees(ori);
	m2.setRotationDegrees(rot);
	m1.getTransposeMatrix(m3);
	m2*=m3;
	return m2.getRotation();
}


void exportJoint(fstream&file,FILE*fOut,vector3d pos,vector3d ori){
	vector3d jointOri;
	vector3d jointRot;
	file>>jointName;
	file>>jointPos.x>>jointPos.y>>jointPos.z;
	file>>jointRot.x>>jointRot.y>>jointRot.z;
	file>>jointOri.x>>jointOri.y>>jointOri.z;
	file>>jointScale.x>>jointScale.y>>jointScale.z;
	vector3d tmp;
#ifdef BLAAAAAaa

/*
	matrix4x4 mat1,mat2;
	mat1.setRotationDegrees(ori);
	mat2.setTranslation(jointPos);
	/*mat2.setRotationDegrees(jointOri);
	
	mat1=mat2*mat1;
	quaternion q;
	q.fromEulerAngles(jointOri.x,jointOri.y,jointOri.z);
	//mat1*=mat2;
	//jointRot=q*jointRot;//mat1.getRotation();
	//*jointRot+=jointOri;//

	mat1*=mat2;
jointPos=mat1.getTranslation();
	*/
	/*
	quaternion qx;
	quaternion qy;
	quaternion qz;
	quaternion q;
	qx.fromAngle(ori.x,vector3d(1,0,0));
	qy.fromAngle(ori.y,vector3d(0,1,0));
	qz.fromAngle(ori.z,vector3d(0,0,1));
	q=qx*qy*qz;
	jointPos=q*jointPos;*/

	jointPos=orient(ori,jointPos);
	tmp=jointPos+pos;

//	quaternion q1(ori.x,ori.y,ori.z);
//	quaternion q2(jointOri.x,jointOri.y,jointOri.z);
//	jointOri=q1*jointOri;

/**/


	matrix4x4 m1,m2;
	m1.setRotationDegrees(ori);
	m2.setRotationDegrees(jointOri);
	m1*=m2;

	jointOri=m1.getRotation();

	jointRot=orient(jointOri,jointRot);

//	jointRot=orientRot(jointOri,jointRot);
//	jointOri=orient(jointRot,jointOri);
	/*
	m1.setRotationDegrees(jointOri);
	m2.setRotationDegrees(jointRot);
	m1*=m2;
	jointRot=m1.getRotation();
	ori=m1.getRotation();*/

	//jointPos.x*=-1;
#else
	tmp=jointPos;
#endif
	fwrite(jointName,1,sizeof(char)*32,fOut);
	fwrite(&jointPos,1,sizeof(vector3d),fOut);
	fwrite(&jointRot,1,sizeof(vector3d),fOut);
	fwrite(&jointScale,1,sizeof(vector3d),fOut);
	int cCnt;
	file>>cCnt;//childs count for this joint
	fwrite(&cCnt,1,sizeof(int),fOut);
	for(int i=0;i<cCnt;i++){
		exportJoint(file,fOut,tmp,jointOri);
	}
}


void Load(char*filename,FILE*fOut)
{
	fstream file;
	file.open(filename,ios::in);
	char FrameName[32];
	char AnimationName[32];
	if(!file)return;
	int TotalCycles;


	file>>AnimationName;
	fwrite(&AnimationName,32,sizeof(char),fOut);

	file>>TotalCycles;
	fwrite(&TotalCycles,1,sizeof(int),fOut);
	Frames		=new _FrameKeys[TotalCycles];
	printf("Total Cycles : %d\n",TotalCycles);

	int FrameIndex=0;
	for(int i=0;i<TotalCycles;i++)
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

	int i=0;
	printf("Exporting Cycles..\n");
	for(int j=0;j<TotalCycles;j++)
	{
		Frames[j].start=i;
		for(int k=0;k<Frames[j].count;k++)
		{
			exportJoint(file,fOut,0,0);
			i++;
		}
		Frames[j].end=i-1;
		fwrite(&Frames[j].start,1,sizeof(int),fOut);
		fwrite(&Frames[j].end,1,sizeof(int),fOut);
	}
	delete [] Frames;
	file.close();
		
	printf("Done..\n");

}


int main(int argc,char*argv[])
{
	FILE *fOut;

	matrix4x4 m1,m2,m3;

	vector3d ori(45,0,45);
	vector3d rot(0);
	vector3d out=orientRot(ori,rot);
	
	printf("Welcom to M-RAY Animation Converter\n");
	printf("          -----          \n");
	printf("this program convert m-ray ascii Animation to binary\n");
	printf("Enjoy :D\n\n");

	char szFName[MAX_PATH];
	char szFName2[MAX_PATH];
	if(argc!=2)
	{
		printf("File Name:");
		scanf("%s",szFName2);
	}else
	{
		strcpy(szFName2,argv[1]);
	}

	strcpy(szFName,szFName2);
	szFName[strlen(szFName2)-3]='\0';
	strcat(szFName,"anim");
	fOut=fopen(szFName,"wb");
	Load(szFName2,fOut);

	fclose(fOut);
	getchar();
}
