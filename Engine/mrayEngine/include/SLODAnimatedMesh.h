



#ifndef ___SLODAnimatedMesh___
#define ___SLODAnimatedMesh___


#include "MeshRenderableNode.h"
#include "IAnimationMixer.h"

namespace mray{
namespace scene{


struct SLODMesh
{
	SLODMesh():mesh(0),distance(0){
	}
	SLODMesh(const scene::SMeshPtr&m,float dist):mesh(m),distance(dist){
	}
	bool operator <(SLODMesh&other)const{
		return distance<other.distance;
	}
	SMeshPtr mesh;
	float distance;
};

class MRAY_DLL SLODAnimatedMesh:public MeshRenderableNode
{
	DECLARE_RTTI
protected:
	std::vector<SLODMesh>m_meshs;
	int currLOD;
	std::vector<GCPtr<animation::IAnimationMixer>> m_animationMixers;
	core::string m_modelFile;

public:

	DECLARE_PROPERTY_TYPE(Model,core::string,MRAY_DLL);

	SLODAnimatedMesh();

	virtual ~SLODAnimatedMesh();

	bool SetModelFileName(const core::string& name);
	const core::string& GetModelFileName();

	void addLOD(const SLODMesh& lod);
	void addLOD(const scene::SMeshPtr& mesh,float dist);

	virtual scene::SMesh*getMesh();
	int getCurrLOD(){return currLOD;}

	virtual bool preRender(IRenderPass*pass);
	virtual void update(float dt);
	
	void loadAnimation(const core::string&fileName);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);

};

}
}
#endif




