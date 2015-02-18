

#ifndef ___SQ3BspLevel___
#define ___SQ3BspLevel___

 

#include <ISceneNode.h>
#include <mtypes.h>
#include <SMesh.h>
#include <IStream.h>
#include <ITexture.h>
#include <BitVector.h>

namespace mray{
namespace scene{

class MRAY_DLL SQ3BSPLevel:public ISceneNode
{

	

	struct sBSPHeader{
		int strID;
		int version;
	};

	struct sBSPLump
	{
		int offset;					// The offset into the file for the start of this lump
		int length;					// The length in bytes for this lump
	};

	struct sBSPVertex
	{
		float vPosition[3];			// (x, y, z) position. 
		float vTextureCoord[2];		// (u, v) texture coordinate
		float vLightmapCoord[2];	// (u, v) lightmap coordinate
		float vNormal[3];			// (x, y, z) normal vector
		uint color;						// RGBA color for the vertex 
	};

	struct sBSPFace
	{
		int textureID;				// The index into the texture array 
		int effect;					// The index for the effects (or -1 = n/a) 
		int type;					// 1=polygon, 2=patch, 3=mesh, 4=billboard 
		int startVertIndex;			// The starting index into this face's first vertex 
		int numOfVerts;				// The number of vertices for this face 
		int startIndex;				// The starting index into the indices array for this face
		int numOfIndices;			// The number of indices for this face
		int lightmapID;				// The texture index for the lightmap 
		int lMapCorner[2];			// The face's lightmap corner in the image 
		int lMapSize[2];			// The size of the lightmap section 
		math::vector3d lMapPos;			// The 3D origin of lightmap. 
		math::vector3d lMapVecs[2];		// The 3D space for s and t unit vectors. 
		math::vector3d vNormal;			// The face normal. 
		int size[2];				// The bezier patch dimensions. 
	};

	enum ELumps
	{
		kEntities = 0,				// Stores player/object positions, etc...
		kTextures,					// Stores texture information
		kPlanes,				    // Stores the splitting planes
		kNodes,						// Stores the BSP nodes
		kLeafs,						// Stores the leafs of the nodes
		kLeafFaces,					// Stores the leaf's indices into the faces
		kLeafBrushes,				// Stores the leaf's indices into the brushes
		kModels,					// Stores the info of world models
		kBrushes,					// Stores the brushes info (for collision)
		kBrushSides,				// Stores the brush surfaces info
		kVertices,					// Stores the level vertices
		kIndices,					// Stores the level indices
		kShaders,					// Stores the shader files (blending, anims..)
		kFaces,						// Stores the faces for the level
		kLightmaps,					// Stores the lightmaps for the level
		kLightVolumes,				// Stores extra world lighting information
		kVisData,					// Stores PVS and cluster info (visibility)
		kMaxLumps					// A constant to store the number of lumps
	};

	struct sBSPTexture
	{
		char strName[64];			// The name of the texture w/o the extension 
		int flags;					// The surface flags (unknown) 
		int textureType;			// stores the types of brushes that we are dealing with.
									//if the first bit is 1 then its solid otherwise it's non solid,water
	};

	struct sBSPModel
	{
		float bbox[6];
		int faceStart;
		int faceCount;
		int brushStart;
		int brushCount;
	};

	struct sBSPShader
	{
		char shaderName[64];
		int surfaceFlags;
		int contentFlags;
	};

	struct sBSPLightmap
	{
		byte imageBits[128][128][3];   // The RGB data in a 128x128 image
	};

	// This stores a node in the BSP tree
	struct sBSPNode
	{
		int plane;					// The index into the planes array 
		int front;					// The child index for the front node 
		int back;					// The child index for the back node 
		math::vector3di min;				// The bounding box min position. 
		math::vector3di max;				// The bounding box max position. 
	}; 
	struct sBSPNode_new
	{
		int plane;					// The index into the planes array 
		int front;					// The child index for the front node 
		int back;					// The child index for the back node 
		math::box3d bbox;			// The bounding box 
	}; 

	// This stores a leaf (end node) in the BSP tree
	struct sBSPLeaf
	{
		int cluster;				// The visibility cluster 
		int area;					// The area portal 
		math::vector3di min;				// The bounding box min position 
		math::vector3di max;				// The bounding box max position 
		int leafface;				// The first index into the face array 
		int numOfLeafFaces;			// The number of faces for this leaf 
		int leafBrush;				// The first index for into the brushes 
		int numOfLeafBrushes;		// The number of brushes for this leaf 
	}; 
	struct sBSPLeaf_new
	{
		int cluster;				// The visibility cluster 
		int area;					// The area portal 
		math::box3d bbox;				// The bounding box min position 
		int leafface;				// The first index into the face array 
		int numOfLeafFaces;			// The number of faces for this leaf 
		int leafBrush;				// The first index for into the brushes 
		int numOfLeafBrushes;		// The number of brushes for this leaf 
	}; 

	// This stores a splitter plane in the BSP tree
	struct sBSPPlane
	{
		math::vector3d vNormal;			// Plane normal. 
		float d;					// The plane distance from origin 
	};

	// This stores the cluster data for the PVS's
	struct sBSPVisData
	{
		int numOfClusters;			// The number of clusters
		int bytesPerCluster;		// The amount of bytes (8 bits) in the cluster's bitset
		std::vector<byte> pBitsets;				// The array of bytes that holds the cluster bitsets				
	};

	// This stores the brush data
	struct sBSPBrush 
	{
		int brushSide;				// The starting brush side for the brush 
		int numOfBrushSides;		// Number of brush sides for the brush
		int textureID;				// The texture index for the brush
	}; 

	// This stores the brush side data, which stores indices for the normal and texture ID
	struct sBSPBrushSide 
	{
		int plane;					// The plane index
		int textureID;				// The texture index
	}; 

	

	int m_numOfEntities;

	int m_numOfVerts;
	int m_numOfFaces;
	int m_numOfIndices;
	int m_numOfTextures;
	int m_numOfLightmaps;

	int m_numOfModels;
	int m_numOfShaders;

	int m_numOfNodes;			// The number of nodes in the level
	int m_numOfLeafs;			// The leaf count in the level
	int m_numOfLeafFaces;		// The number of leaf faces in the level
	int m_numOfPlanes;			// The number of planes in the level
	
	int m_numOfBrushes;			// The number of brushes in our world
	int m_numOfBrushSides;		// The number of brush sides in our world
	int m_numOfLeafBrushes;		// The number of leaf brushes
	
	int m_traceType;			// This stores if we are checking a ray, sphere or a box
	float m_traceRatio;			// This stores the ratio from our start pos to the intersection pt.
	float m_traceRadius;		// This stores the sphere's radius for a collision offset

	math::box3d m_vTraceBox;		// This stores the maximum values of the AABB (top corner)
	math::vector3d m_vExtents;		// This stores the largest length of the box
	
	math::vector3d m_vCollisionNormal;	// This stores the normal of the plane we collided with

	bool m_bGrounded;
	bool m_bCollided;			// This tells if we just collided or not
	bool m_tryToStep;

	float m_maxStepHeight;


	std::vector<S3DVertex> vertices;
	std::vector<uint> indices;
	std::vector<sBSPFace> faces;
	std::vector<sBSPTexture> texturesPath;
	std::vector<sBSPLightmap> lightMaps;
	std::vector<GCPtr<video::TextureUnit>> lightMapsTex;

	std::vector<sBSPNode_new>	 m_pNodes;		// The nodes in the bsp tree
	std::vector<sBSPLeaf_new>	 m_pLeafs;		// The leafs in the bsp tree
	std::vector<math::Plane>	 m_pPlanes;		// The planes stored in the bsp tree
	std::vector<int>			 m_pLeafFaces;	// The leaf's faces in the bsp tree
	sBSPVisData					m_clusters;	// The clusters in the bsp tree for space partitioning

	std::vector<sBSPBrush>		m_pBrushes;		// This is our brushes
	std::vector<sBSPBrushSide>	m_pBrushSides;	// This holds the brush sides
	std::vector<int>			m_pLeafBrushes;  // The indices into the brush array


	int m_VisibleFaces; //for debug only


	core::BitVector m_facesDrawn;

	math::box3d bbox;

	std::vector<GCPtr<video::RenderMaterial>> materials;

	sBSPHeader header;
	sBSPLump lumps[kMaxLumps];

	math::vector3d tryToStep(math::vector3d start,math::vector3d end);

	math::vector3d trace(math::vector3d start,math::vector3d end);

	// This recursively checks all the nodes until we find leafs that store the brushes
	void checkNode(int nodeIndex, float startRatio, float endRatio, math::vector3d start, math::vector3d end);
	
	// This checks our movement vector against the brush and it's sides
	void checkBrush(sBSPBrush *pBrush, math::vector3d start, math::vector3d end);
	
	void RenderFace(int i);
	
	inline int isClusterVisible(int current, int test);

	int findLeaf(const math::vector3d &vPos);

	void loadTextures(OS::IStreamPtr file);
	void loadNodes(OS::IStreamPtr file);
	void loadVisData(OS::IStreamPtr file);
	void loadVertices(OS::IStreamPtr file);
	void loadLeafs(OS::IStreamPtr file);
	void loadPlanes(OS::IStreamPtr file);
	void loadShaders(OS::IStreamPtr file);
	void loadModels(OS::IStreamPtr file);
	void loadLightMaps(OS::IStreamPtr file);
	void loadFaces(OS::IStreamPtr file);
	void loadBrushes(OS::IStreamPtr file);

public:
	SQ3BSPLevel();
	virtual ~SQ3BSPLevel();

	bool loadLevel(OS::IStreamPtr file);

	virtual void preRender(IRenderPass*pass);
	virtual void render();
	virtual void update(float dt);

	virtual const video::RenderMaterialPtr& getMaterial(int i){return materials[i];}
	virtual int getMaterialCount(){return materials.size();}
	virtual void setMaterial(const video::RenderMaterialPtr& mat,int i){materials[i]=mat;}

	virtual math::box3d getBoundingBox(){
		return bbox;
	}

	math::vector3d traceRay(math::vector3d start,math::vector3d end);
	math::vector3d traceSphere(math::vector3d start,math::vector3d end,float radius);
	math::vector3d traceBox(math::vector3d start,math::vector3d end,math::box3d aabb);
	math::vector3d traceBox(math::vector3d start,math::vector3d end,math::vector3d extent);

	bool isOnGround(){return m_bGrounded;}

};

}
}

#endif




