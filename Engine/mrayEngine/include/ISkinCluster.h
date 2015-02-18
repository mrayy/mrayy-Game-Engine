
#ifndef ___ISkinCluster___
#define ___ISkinCluster___


//#include "SBoneNode.h"

#include "mString.h"
#include "GCPtr.h"



namespace mray{
namespace scene{

	class IMeshBuffer;
	class Skeleton;

class MRAY_DLL ISkinCluster
{
public:
	struct MRAY_DLL SBoneWeights;
	typedef std::list<SBoneWeights> BoneWeightList;

	ISkinCluster(Skeleton*skeleton);
	ISkinCluster();
	~ISkinCluster();

	void setBoneWeights(int bone,int arrSize,float*weights);
	ISkinCluster::SBoneWeights*getBoneWeights(int bone);
	ISkinCluster::SBoneWeights*getBoneWeights(const core::string&name);

	void setVertexWeight(int bone,int vtx,float weight);
	float getVertexWeight(int bone,int vtx);
	const BoneWeightList& getBoneWeightList()const{return bonesWeights;}
	
	void calcWeights(IMeshBuffer *mesh,Skeleton*skeleton,float minR,float maxR);

	
	struct MRAY_DLL SBoneWeights{
		core::string name;
		int boneIndex;
		std::vector<float> vertices;

		~SBoneWeights(){
			vertices.clear();
		}
		bool operator < (const SBoneWeights&o)const {return boneIndex<o.boneIndex;}
		bool operator == (const SBoneWeights&o)const {return boneIndex==o.boneIndex;}

		SBoneWeights*getWeights(int bone){
			if(boneIndex==bone)return this;/*
			C_List<GCPtr<SBonesWeights>>::iterator it=children.begin();
			for(;it!=children.end();++it){
				GCPtr<SBonesWeights>tmp=(*it)->getWeights(bone);
				if(tmp)return tmp;
			}*/
			return 0;
		}
	};

private:
	
	void buildWeights(Skeleton*skeleton);
	BoneWeightList bonesWeights;
};

MakeSharedPtrType(ISkinCluster);

}
}

#endif






