

#ifndef ___SMESH___
#define ___SMESH___


#include "Skeleton.h"
#include "IMeshBuffer.h"


namespace mray
{
	class ThreadJobManager;
	class IJobPiece;

namespace scene
{
	class MeshBufferData;
	
class MRAY_DLL SMesh:public IResource
{
protected:
	void recalcLocatorsPos(float frame);
	float lastFrame;
	int lastCycle;
	math::box3d boundingBox;
	std::vector<MeshBufferData*> m_meshData;

	SkeletonPtr m_skeleton;

	IJobPiece* m_jobPiece;

	std::vector<math::matrix4x4> m_worldMatricies;

	virtual uint calcSizeInternal();

	void setupShaderAndAnimators();
public:

	SMesh();
	virtual ~SMesh();

	void clear();

	std::vector<math::matrix4x4>& GetWorldMatricies(){return  m_worldMatricies;}
	const std::vector<math::matrix4x4>& GetWorldMatricies()const{return  m_worldMatricies;}

	int getBuffersCount();
	MeshBufferData *addNewBuffer();
	MeshBufferData* addBuffer(const IMeshBufferPtr&b);
	int getBufferIndex(const mchar* name);

	const IMeshBufferPtr&getBuffer(int index);
	MeshBufferData*getBufferData(int index);

	const IMeshBufferPtr& getBufferByName(const mchar* name);
	void removeBuffer(int index);

	void setSkeleton(const SkeletonPtr& s);
	const SkeletonPtr& getSkeleton();

	void update(float dt);
	void AddUpdateJobs(ThreadJobManager* mngr,float dt);

#ifdef OLD_MESH_CODE
	void animate(int CycleIndex,float frame,bool allowSkeleton=true);
	void animate(float frame,bool allowSkeleton=true);

	void animateSkeleton(int CycleIndex,float frame);
	void animateSkeleton(const  mchar*jointName,int CycleIndex,float frame);
	void animateSkeleton(float frame);
	
	void animateMesh(int CycleIndex,float frame);
	void animateMesh(float frame);

	void animateMesh(const mchar*meshName,int CycleIndex,float frame);

	//this function will blend the current cycle to blend cycle
	//curr cycle will goes to currFrame and blend with blendFrame
	void blendAnimation(int blendFrame,float currFrame,float blend);


	void animateMesh(int startFrame,int endFrame,float frame);

	void blendAnimation(int frame,float blend);
	void blendAnimation(int c1SFrame,int c1EFrame,float c1frame,int frame,float blend);



	void updateLocaters();
#endif

	math::box3d calcCurrBoundingBox();


	math::box3d &getBoundingBox(){return boundingBox;}
	void setBoundingBox(math::box3d b){boundingBox=b;}

	void getStartEndFrames(int CycleIndex,float &frame,int &sout,int &eout);

	scene::SMesh* getDuplicate();

	//this function only duplicate the mesh contexts , it will not duplicate the hardware Vertex Buffer!
	scene::SMesh* duplicateContextOnly();
};

MakeSharedPtrType(SMesh);

}
}

#endif