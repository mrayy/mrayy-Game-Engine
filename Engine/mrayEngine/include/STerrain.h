

#ifndef ___STERRAIN___
#define ___STERRAIN___


#include "SMesh.h"
#include "ITerrain.h"


namespace mray{
namespace video{
	class ITexture;
}
namespace scene{


enum TERR_PATCH_SIZE
{
	PS_1=1,
	PS_2=2,
	PS_4=4,
	PS_8=8,
	PS_16=16,
	PS_32=32,
	PS_64=64,
	PS_128=128,
};

class MRAY_DLL STerrain:public ITerrain
{
protected:
	GCPtr<SMesh> terrMesh;
	video::ITexturePtr  heightMap;
	
	TERR_PATCH_SIZE patchSize;
	int vertPerEdge;
	int maxLOD;


public:

	STerrain();
	virtual ~STerrain();

	bool loadTerrain(const  mchar*fileName,TERR_PATCH_SIZE patchSz);
	

	void setTexture(const  mchar* texName,int layer);
	virtual void setTexture(video::ITexturePtr   tex,int layer=0);
	virtual float getHeight(float x,float z,math::vector3d *outNormal=0);

	const IMeshBufferPtr& getTerrainMeshBuffer(){return terrMesh->getBuffer(0);}

	virtual scene::SMesh* getMesh(){return terrMesh;}

	int getVertPerEdge(){return vertPerEdge;}
	void setVertexPos(int vertex,math::vector3d pos);
	int getVertexFromPos(math::vector3d Pos);

	virtual void setMaterial(const video::RenderMaterialPtr& mtrl,int i=0);
	virtual const video::RenderMaterialPtr& getMaterial(int i=0);
	virtual int getMaterialCount(){return 1;}

	virtual void setPosition(const math::vector3d&  v);
	virtual void setRotation(const math::vector3d&  v);
	virtual void setScale(const math::vector3d&  v);
	
	virtual math::box3d getBoundingBox(){return terrMesh->getBuffer(0)->getBoundingBox();}
	
};


}
}



#endif







