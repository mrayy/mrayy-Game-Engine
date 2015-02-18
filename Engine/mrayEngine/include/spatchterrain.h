

#ifndef ___SPatchTerrain___
#define ___SPatchTerrain___


#include "SMesh.h"
#include "ITerrain.h"

#include "ITexture.h"
#include "IHeightMapLoader.h"

namespace mray{
	namespace video
	{
		class RenderPass;
	}
namespace scene{

	class IViewNode;
	class SPatchTerrain;

	class MRAY_DLL TerrainTextureLayer:public IObject
	{
	protected:
		video::TextureUnit* m_maskTexture;
		video::TextureUnit* m_baseTexture;
		video::TextureUnit* m_detailTexture;

		float m_BaseTilt;
		float m_DetailTilt;

		float m_alpha;

		friend class SPatchTerrain;
	public:
		TerrainTextureLayer();
		virtual~TerrainTextureLayer();

		void SetMaskTexture(video::ITexture* tex)const;
		video::ITexture* GetMaskTexture();
		video::TextureUnit* GetMaskTextureUnit(){return m_maskTexture;}

		void SetBaseTexture(video::ITexture* tex)const;
		video::ITexture* GetBaseTexture();
		video::TextureUnit* GetBaseTextureUnit(){return m_baseTexture;}

		void SetDetailTexture(video::ITexture* tex)const;
		video::ITexture* GetDetailTexture()const;
		video::TextureUnit* GetDetailTextureUnit()const{return m_detailTexture;}

		void  SetBaseTilt(float v){m_BaseTilt=v;}
		float GetBaseTilt()const{return m_BaseTilt;}

		void  SetDetailTilt(float v){m_DetailTilt=v;}
		float GetDetailTilt()const{return m_DetailTilt;}

		void  SetAlpha(float v){m_alpha=v;}
		float GetAlpha()const{return m_alpha;}
	};

class MRAY_DLL SPatchTerrain:public ITerrain
{
protected:
	typedef std::vector<uint> IndexArray;

	struct SPatch{
		SPatch():top(0),bottom(0),left(0),right(0),isChanged(1),distToCam(0),currentLod(-1)
		{}

		//GCPtr<video::IHardwareIndexBuffer> m_indexBuffer;
		std::vector<IndexArray> m_indicies;
		int currentLod;
		math::box3d BBox;
		math::vector3d center;
		bool isChanged;

		float distToCam;

		int top, bottom;
		int  left, right;
	};

	std::vector<TerrainTextureLayer*> m_layers;

	GCPtr<loaders::IHeightMapLoader> m_heightMap;

	std::vector<SPatch> patchs;
	std::vector<math::vector3d>  LodDistThreshold;
	std::vector<math::vector3d>  m_basePos;
	/*
	math::vector3d terrPos;
	math::quaternion terrRot;
	math::vector3d terrScale;*/
	math::vector3d terrPivot;

	math::vector3d m_position;
	math::vector3d m_scale;

	math::matrix4x4 transMatrix;

	video::StreamsData m_streams;
	std::vector<uint> m_tempIndicies;

	video::RenderMaterialPtr  m_material;
	video::RenderMaterialPtr  m_layerMaterial;


	int numberOfIndices;

	video::ITexturePtr   heightMap;
	core::string m_heightMapName;

	math::box3d BoundingBox;
	
	int patchSize;
	int maxLOD;
	int segments;
	int patchCnt;
	float m_lodDist;

	int m_skip;

	float tRepeat;

	math::vector3d center;

	math::vector3d oldCamPos;
	math::vector3d oldCamRot;


	float cameraRotateDelta;
	float cameraPosDelta;

	bool m_firstUpdate;

	void calcPatchs();
	void calcDistThreshold();

	void recalcLOD(IViewNode*vn,bool forceCalc=false);
	void recalcPatchesToRender();

	bool calcPatchIndicies(int pX,int pZ,SPatch*patch);

	int getIndex(int pX,int pZ,SPatch*patch,int vX,int vZ);

	void applyTransformation();

	void applySmoothing();

	void calcNormals();

	void initialize(const GCPtr<loaders::IHeightMapLoader>& heightMap,bool software);

	void _DrawLayer(TerrainTextureLayer* layer,video::RenderPass* mtrl);
public:

	SPatchTerrain(const math::vector3d&  pos,const math::vector3d& scale);
	virtual ~SPatchTerrain();

	void AddTextureLayer(TerrainTextureLayer* layer);
	const std::vector<TerrainTextureLayer*>& GetTextureLayers()const;

	const math::vector3d& GetPosition()const{return m_position;}
	const math::vector3d& GetScale()const{return m_scale;}

	bool loadTerrain(const GCPtr<loaders::IHeightMapLoader>& heightMap,int lod,float lodDist,bool software=false);
	bool loadTerrain(const GCPtr<loaders::IHeightMapLoader>& heightMap,int patchPerSide,int lod,float lodDist,bool software=false);

	video::IHardwareStreamBuffer* getStream(int s,video::EMeshStreamType type);

	int GetLODCount();
	float GetLODDistance();
	int GetPatchCount();
	int GetSegementsCount();

	virtual bool preRender(IRenderPass*pass);
	virtual bool render(IRenderPass*pass);
	virtual void update(float dt);
	//virtual void update(float dt);

	virtual const core::string& getHeightMapName();
	const GCPtr<loaders::IHeightMapLoader>& getHeightMap();

	virtual void setTexture(const video::ITexturePtr&   tex,int layer=0);

	virtual void setBaseTexture(const video::ITexturePtr&   tex);
	virtual void setBaseNormal(const video::ITexturePtr&   tex);
	virtual void setDetailTexture(const video::ITexturePtr&   tex);
	virtual void setDetailNormal(const video::ITexturePtr&   tex);

	virtual const video::ITexturePtr&   getBaseTexture();
	virtual const video::ITexturePtr&   getBaseNormal();
	virtual const video::ITexturePtr&   getDetailTexture();
	virtual const video::ITexturePtr&   getDetailNormal( );

	virtual math::box3d getBoundingBox(){return BoundingBox;}
	

	virtual void setPosition(const math::vector3d& v);
	//virtual void setRotation(const math::vector3d& v);
	virtual void setScale(const math::vector3d& v);


	void setTransofrmation(const math::vector3d& pos/*,math::quaternion orintation*/,const math::vector3d& scale);

	math::vector3d TransformVectorToLocal(const math::vector3d& v);
	math::vector3d TransformVectorToWorld(const math::vector3d& v);

	virtual float getHeight(float x,float z,math::vector3d *outNormal=0);
	void OnHeightChanged(int vtx,float h);
	virtual void QueryQuads2D(const math::rectf&rc,std::vector<math::rectf>& outRects,bool &splitDir);

	void ProjectTriangle(const math::vector3d *v);

	virtual void exportTerrain(const  mchar*fileName);
	virtual void importTerrain(const  mchar*fileName);

	virtual void setMaterial(const video::RenderMaterialPtr& mtrl,int i=0);
	virtual const video::RenderMaterialPtr&  getMaterial(int i=0);
	virtual int getMaterialCount();

	virtual void setShader(int mesh,const video::GPUShaderPtr& shader);
	virtual void setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback);

	virtual const video::GPUShaderPtr& getShader(int i);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);
};

}
}

#endif




