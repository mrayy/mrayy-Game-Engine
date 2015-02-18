
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>	
#include <fstream>	
#include <iostream>	
#include <matrix4x4.h>
#include <quaternion.h>
#include <vector>
#include <mstring.h>
#include <map>
#include <ChunkSerializer.h>
#include <WinFile.h>
#include <StreamWriter.h>

using namespace std;
using namespace mray;
using namespace math;
using namespace core;


#define TRACKGROUP_CHUNK_ID 0x2
#define TRACK_CHUNK_ID 0x3

struct AnimHeader
{
	int version;
};

struct _FrameKeys
{
	char FrameName[32];
	int  start,end;
	int	 count;
	vector<int> frames;
};

struct TrackHeader{
	char trackName[32];
	int numberOfKeys;
	uchar transType;
	int affectedNodeID;
	ulong catagoryBits;
	ulong priority;
};

struct KeyFrame
{
	float time;

	vector3d pos;
	vector3d rot;
	vector3d scale;
};

typedef vector<KeyFrame> KeyFrameList;
struct AnimationTrack
{
	stringc name;
	KeyFrameList keyFrames;
};

typedef map<stringc,AnimationTrack> AnimTrackList;

struct AnimGroup
{
	stringc name;
	AnimTrackList tracks;
};

typedef vector<AnimGroup> AnimMixerList;
AnimMixerList animMixer;


_FrameKeys *Frames;
char jointName[64];
vector3d jointPos;
vector3d jointScale;

vector3d orient(vector3d ori,vector3d pos){
	
	matrix4x4 m;
	m.setRotationDegrees(ori);
	m.transformVectPre(pos);
	return pos;
}
vector3d orientRot(vector3d ori,vector3d rot){
	
	matrix4x4 m1,m2,m3;
	m1.setRotationDegrees(ori);
	m2.setRotationDegrees(rot);
	m1.getTransposeMatrix(m3);
	m2*=m3;
	return m2.getRotation();
}


KeyFrame tempKF;
vector3d jointOri;
vector3d jointRot;

void readFrame(fstream&file,AnimTrackList&trackList,float time){

	file>>jointName;
	tempKF.time=time;
	file>>tempKF.pos.x>>tempKF.pos.y>>tempKF.pos.z;
	file>>tempKF.rot.x>>tempKF.rot.y>>tempKF.rot.z;
//	file>>jointOri.x>>jointOri.y>>jointOri.z;
	file>>tempKF.scale.x>>tempKF.scale.y>>tempKF.scale.z;

	//look for the join
	AnimTrackList::iterator it= trackList.find(jointName);
	AnimationTrack*track=0;
	if(it==trackList.end())
	{
		//if joint not exist,add it
		AnimationTrack t;
		t.name=jointName;
		AnimTrackList::_Pairib p= trackList.insert(AnimTrackList::value_type(jointName,t));
		track=&p.first->second;
	}else{
		track=&it->second;
	}

	vector3d tmp;

	track->keyFrames.push_back(tempKF);


	tmp=jointPos;
	int cCnt;
	file>>cCnt;//childs count for this joint
	for(int i=0;i<cCnt;i++){
		readFrame(file,trackList,time);
	}
}


void Load(char*filename)
{
	fstream file;
	file.open(filename,ios::in);
	char FrameName[32];
	char AnimationName[32];
	if(!file)return;
	int TotalCycles;

	animMixer.clear();

	file>>AnimationName;
	//fwrite(&AnimationName,32,sizeof(char),fOut);

	file>>TotalCycles;
	//fwrite(&TotalCycles,1,sizeof(int),fOut);
	Frames		=new _FrameKeys[TotalCycles];
	printf("Total Cycles : %d\n",TotalCycles);

	int FrameIndex=0;
	for(int i=0;i<TotalCycles;i++)
	{
		AnimGroup track;
		Frames[i].count=0;
		file>>Frames[i].count;
		//fwrite(&Frames[i].count,1,sizeof(int),fOut);
		file>>FrameName;


		track.name=FrameName;
		animMixer.push_back(track);

		printf("Cycles[%d]:\n",i);
		printf("\tName: %s\n",FrameName);
		printf("\tFrames: %d\n",Frames[i].count);

		//fwrite(FrameName,32,sizeof(char),fOut);
		for(int j=0;j<Frames[i].count;j++)
		{
			file>>FrameIndex;
			Frames[i].frames.push_back(FrameIndex);
			//fwrite(&FrameIndex,1,sizeof(int),fOut);
		}
	}

	int i=0;
	printf("Parsing Cycles..\n");
	AnimMixerList::iterator it= animMixer.begin();
	for(int j=0;j<TotalCycles;j++)
	{
		Frames[j].start=i;
		for(int k=0;k<Frames[j].count;k++)
		{
			readFrame(file,it->tracks,Frames[j].frames[k]);
			i++;
		}
		Frames[j].end=i-1;
		it++;
		//fwrite(&Frames[j].start,1,sizeof(int),fOut);
		//fwrite(&Frames[j].end,1,sizeof(int),fOut);
	}
	delete [] Frames;
	file.close();
		
	printf("Done..\n");

}

void Export(OS::IStream*fOut)
{
	OS::StreamWriter writer;
	writer.setStream(fOut);

	AnimHeader header;
	header.version=0x1;
	writer.write(&header,sizeof(header));
	loaders::ChunkSerializer<uchar>::SChunk chunk;

	
	AnimMixerList::iterator it=animMixer.begin();

	TrackHeader theader;
	for(;it!=animMixer.end();it++)
	{
		loaders::ChunkSerializer<uchar>::writeChunk(fOut,TRACKGROUP_CHUNK_ID,0);

		char groupName[32];
		strncpy(groupName,it->name.c_str(),32);
		writer.write(groupName,sizeof(groupName));

		theader.affectedNodeID=0;
		theader.catagoryBits=0;
		theader.transType=3;//Transformation
		theader.priority=0;
		AnimTrackList::iterator it2=it->tracks.begin();
		for(;it2!=it->tracks.end();it2++){
			AnimationTrack&t=it2->second;
			loaders::ChunkSerializer<uchar>::writeChunk(fOut,TRACK_CHUNK_ID,0);
			strncpy(theader.trackName,t.name.c_str(),32);
			theader.numberOfKeys=t.keyFrames.size();
			KeyFrameList::iterator it3=t.keyFrames.begin();
			writer.write(&theader,sizeof(theader));
			for(;it3!=t.keyFrames.end();it3++){
				KeyFrame&kf=*it3;
				writer.write(&kf.time,sizeof(kf.time));
				writer.write(&kf.pos,sizeof(kf.pos));
				writer.write(&kf.rot,sizeof(kf.rot));
				writer.write(&kf.scale,sizeof(kf.scale));
			}
		}
	}
	
}


int main(int argc,char*argv[])
{

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
	Load(szFName2);

	OS::WinFile fOut;
	fOut.open(szFName,OS::BIN_WRITE);

	Export(&fOut);
	fOut.close();

	getchar();
}
