

/********************************************************************
	created:	2010/07/30
	created:	30:7:2010   23:53
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest\GrassRenderNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\VegetationTest
	file base:	GrassRenderNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GrassRenderNode_h__
#define GrassRenderNode_h__


#include "MeshRenderableNode.h"



namespace mray
{
	namespace video
	{
		class IHardwarePixelBuffer;
	}
namespace scene
{

class MRAY_DLL GrassRenderNode:public MeshRenderableNode
{
public:
	struct GrassData
	{
		math::vector3d pos;
		math::vector2d texPos;
		float shadow;
		float scale;
	};
private:
protected:
	//GCPtr<StaticGeometryBatch> m_grassObjects;

	struct UVRect
	{
		UVRect():prob(0)
		{}
		UVRect(const math::rectf &r,float p):prob(p),rc(r)
		{}
		math::rectf rc;
		float prob;
	};

	std::vector<UVRect> m_uvRects;
	std::vector<GrassData> m_grassData;

	void _GenerateGrassNode(GrassData&g,int spans,math::vector3d*&vert,math::vector4d *&data,math::vector4d *&data2,uchar *&indexData,int &currIndex,
		math::box3d&bb,bool index16bit);
	

	scene::IMeshBuffer* m_buffer;
	int m_totalIndexCount;
public:
	GrassRenderNode();
	virtual ~GrassRenderNode();

	bool preRender(IRenderPass*pass);


	void AddUVRect(const math::rectf& uv,float prob);
	int GetUVRectCount();
	math::rectf GetUVRect(int idx);
	const math::rectf& GetRandomUVRect();


	void AddObject(const math::vector2d& texPos,const math::vector3d&pos,float scale,float shadow);
	int GetGrassCount(){return m_grassData.size();}
	GrassData* GetGrassData(int i);
	void ShuffleGrasses();

	void Build(int spans=3);
	void ClearGrasses();

	static std::vector<GrassRenderNode*> GenerateFromPixels(video::IHardwarePixelBuffer*tex,const math::vector2d&worldSize,
		float grassPerUnit,int patchesPerEdge,float scale);
};

}
}


#endif // GrassRenderNode_h__
