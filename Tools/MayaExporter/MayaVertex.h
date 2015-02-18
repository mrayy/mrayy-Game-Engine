

/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   11:27
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaVertex.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaVertex
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaVertex__
#define __MayaVertex__


namespace mray
{
namespace exporter
{

	struct uvset
	{
		short size;
	};
	struct texcoord
	{
		float u,v,w;
	};
	//vertex bone assignment
	struct vba
	{
		vba():weight(0),boneId(0)
		{}
		int boneId;
		float weight;
	};

class MayaVertex
{
private:
protected:
public:
	double x,y,z;
	MVector normal;
	float r,g,b,a;

	std::vector<texcoord> texcoords;
	std::vector<vba> vbas;

	long index;

};

struct vertexInfo
{
	int pointId;
	int normalId;
	float r,g,b,a;
	std::vector<float> u;
	std::vector<float> v;
	std::vector<float> vba;
	std::vector<int> boneId;
	int next;
};

struct face
{
	long v[3];
};

typedef std::vector<face> faceArray;


}
}

#endif
