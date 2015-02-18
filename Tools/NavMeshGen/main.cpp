
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
#include <set>

#include <matrix4x4.h>
#include <quaternion.h>
#include <NavMesh.h>
#include <mrayEngine.h>
#include <mrayWin32.h>
#include <XMLWriter.h>


using namespace std;
using namespace mray;
using namespace math;

float weightTrunc=0.01f;
float scaleFactor=1.0f;



std::vector<math::vector3d> verticies;
std::vector<ushort> triangles;
std::vector<int> attributes;
std::map<std::string,int> attributesStr;
int lastAttr=0;

int numVert;
int numTri;

char tmpStr[200];

void Load(char*filename,const char*attrFileName)
{
	fstream file;
	file.open(filename,ios::in);
	if(!file)return;

	GCPtr<OS::IStream> stream=gFileSystem.openFile(attrFileName,OS::BIN_WRITE);

	int TotalObjects;
	math::vector3d vert;


	file>>TotalObjects;

	printf("Total Objects : %d\n",TotalObjects);


	stream->write(&TotalObjects,sizeof(TotalObjects));

	for(int k=0;k<TotalObjects;k++)
	{
		file>>numVert>>numTri;


		verticies.resize(numVert);
		triangles.resize(3*numTri);
		attributes.resize(numTri);

		stream->write(&numTri,sizeof(numTri));
		for(int i=0;i<numTri;++i)
		{;
			int idx=3*i;
			file>>tmpStr;
			std::map<std::string,int>::iterator it= attributesStr.find(tmpStr);
			if(it==attributesStr.end()){
				lastAttr++;
				attributesStr[tmpStr]=lastAttr;
				attributes[i]=lastAttr;
			}else
				attributes[i]=it->second;

			//write triangle attribute value
			stream->write(&attributes[i],sizeof(attributes[i]));

			file>>triangles[idx]>>triangles[idx+1]>>triangles[idx+2];
			//fwrite(&tri,1,sizeof(TRIANGLE),fOut);
		}
		for(int i=0;i<numVert;i++)
		{
			file>>vert.x>>vert.y>>vert.z;
			verticies[i]=vert*scaleFactor;
		}
	}

	file.close();

	stream->close();

}

void Export(const char*baseFileName)
{
	core::string navName=baseFileName;
	navName+=mT(".nav");

	core::string attrsName=baseFileName;
	attrsName+=mT(".xml");

	AI::NavMesh mesh(0);
	mesh.SetMaxSlope(0.5);
	mesh.SetMaxStepSize(0.1);
	GCPtr<OS::IStream> stream=gFileSystem.openFile(navName,OS::BIN_WRITE);
	mesh.Generate(&verticies[0],verticies.size(),&triangles[0],triangles.size(),&attributes[0],true);
	mesh.Serialize(stream);
	stream->close();

	stream=gFileSystem.openFile(attrsName,OS::TXT_WRITE);
	xml::XMLWriter writer;

	xml::XMLElement elem("NavMeshAttrs");
	std::map<std::string,int>::iterator it= attributesStr.begin();
	for(;it!=attributesStr.end();++it)
	{
		xml::XMLElement* e=new xml::XMLElement("Attribute");
		e->addAttribute(mT("ID"),it->first.c_str());
		e->addAttribute(mT("Value"),core::StringConverter::toString(it->second));
		e->addAttribute(mT("Weight"),mT("1.0"));
		elem.addSubElement(e);
	}
	writer.addElement(&elem);

	OS::StreamWriter w(stream);
	w.writeString(writer.flush());

	stream->close();
}

int main(int argc,char*argv[])
{
	GCPtr<OS::IOSystem> osSystem=new OS::WinOSystem();
	Engine* engine=new Engine(osSystem);

	printf("Welcom to M-RAY Converter\n");
	printf("          -----          \n");
	printf("this program convert m-ray ascii NavMesh to binary\n");
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

	printf("Scale factor:");
	scanf("%f",&scaleFactor);

	for(int i=0;i<cnt;++i){
		strcpy(szFName,files[i]);
		szFName[strlen(files[i])-4]='\0';
		core::stringc attrName=szFName;
		attrName+=mT(".attr");
		Load(files[i],attrName.c_str());
		Export(szFName);
	}
	getchar();

	osSystem=0;
	delete engine;

	return 0;
}
