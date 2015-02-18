

#ifndef ___SCENE_NODE___
#define ___SCENE_NODE___


#include "ICullable.h"
#include "mString.h"



#include "CPropertieSet.h"
#include "ISerializable.h"

#include "IAnimators.h"

#include "IRenderable.h"

#include "GCPtr.h"


namespace mray{
namespace scene{

class ISceneVisitor;
class ISceneManager;

class ISceneNodeDesc;

enum ESceneNodeType
{
	ENodeType_SimpleNode,
	ENodeType_Camera,
	ENodeType_Light,
	ENodeType_Count
};


typedef std::list<ISceneNode*> MountPointsList;

MakeSharedPtrType(ISceneNode)

typedef std::vector<IRenderablePtr> AttachNodesList;

class MRAY_DLL ISceneNode:public ICullable
{
	DECLARE_RTTI
protected:

	math::vector3d m_position;
	math::vector3d m_scale;

	IMovable* m_parent;
	MovableNodeList m_Children;
	std::list<GCPtr<IAnimators>> Animators;

	AttachNodesList m_attachedNodes;

	MountPointsList m_mountPoints;

	ISceneManager* m_sceneMngr;

	bool m_isNeedUpdate;
	bool m_updateChilds;

	math::matrix4x4 absTransformation;
	math::matrix4x4 relTransformation;

	math::matrix4x4 m_prevTransformation;

	core::string m_Name;
	uint m_id;

	bool m_drawBoundingBox;

	bool m_isVisible;

	bool m_drownLastFrame;
	

	virtual void fillProperties();

	math::quaternion m_absOrintaion;
	math::vector3d   m_absScale;

	math::quaternion m_orintation;


	math::vector3d m_lastPosition;
	math::vector3d m_lastScale;
	math::quaternion m_lastOrintation;


	math::vector3d m_initialPosition;
	math::vector3d m_initialScale;
	math::quaternion m_initialOrintation;

	math::box3d m_boundingBox;
	
	bool NeedChildUpdate();

	bool hasChanged();


public:

	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Position,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Orintation,math::quaternion,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Scale,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ID,uint,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(DrawBoundingBox,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Visible,bool,MRAY_DLL);

public:


	//Properties
	bool setID(const uint& val);
	const uint& getID();

	bool setNodeName(const core::string& val);
	const core::string& getNodeName()const;

	virtual bool setDrawBoundingBox(const bool& val);
	const bool& getDrawBoundingBox();

	ISceneNode(const  core::string&name,int id,ISceneManager* sceneMngr);
	virtual ~ISceneNode();


	void removeAll();

	void SetDrown(bool d);
	bool IsDrown();

	virtual int GetNodeType();

	void AttachNode(const IRenderablePtr &node);
	void RemoveNode(IRenderable* node);
	int GetAttachedNodesCount();
	const AttachNodesList& GetAttachedNodes();
	IRenderable* GetAttachedNode(int i);
	void ClearAttachedNodes();


	bool isVisible()const;
	bool setVisible(const bool& v);
	bool setVisible(bool v,bool cascade);

	virtual math::vector3d getAbsolutePosition()const;
	virtual math::quaternion getAbsoluteOrintation()const;
	virtual math::vector3d getAbsoluteScale()const;
	virtual const math::vector3d& getPosition()const;
	virtual const math::quaternion& getOrintation()const;
	virtual const math::vector3d& getScale()const;

	virtual bool setPosition(const math::vector3d& v);
	virtual bool setOrintation(const math::quaternion& q);
	virtual bool setScale(const math::vector3d& v);

	void setTranformsAsInitial();
	void resetTranformsToInitial();

	ISceneManager*getSceneManager();

	virtual void SetSceneManager(ISceneManager* mngr);


	virtual const math::box3d& getBoundingBox();
	virtual math::box3d getTransformedBoundingBox();

	const MovableNodeList& getChildren();

//	void setParentPos(vector3d pos){parentPos=pos;}
	

	void addAnimator(const IAnimatorsPtr& anim);

	std::list<IAnimatorsPtr>* getAnimators();
	void removeAnimator(IAnimators* anim);

	virtual void preRender();
	virtual void update(float dt);
	virtual void render(IRenderPass*pass);

	virtual void removeChild(IMovable*elem,bool parentSpace=false);
	virtual void addChild(IMovableCRef elem,bool parentSpace=false);
	void removeFromParent(bool parentSpace=false);


	virtual void removeMountPoint(const core::string&name);
	virtual void addMountPoint(ISceneNode *p);
	virtual ISceneNode *getMountPoint(const core::string&point);
	virtual void attachToMountPoint(const core::string&point,IMovable*elem);
	virtual const MountPointsList& getMountPoints();

	virtual math::matrix4x4&getAbsoluteTransformation();
	virtual const math::matrix4x4&getPrevAbsoluteTransformation()const;
	virtual const math::matrix4x4&getAbsoluteTransformation()const;
	virtual const math::matrix4x4&getRelativeTransformation()const;

	virtual void updateRelativeTransformation();
	virtual void updateAbsoluteTransformation();

	void _UpdateBoundingBox();

	IMovable* getElementById(int id,bool searchChildren=0);

	IMovable* getParent();
	void setParent(IMovable*);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);

	virtual void OnVisit(ISceneVisitor*visitor);

	void SaveToDesc(ISceneNodeDesc* desc)const;
	void LoadFromDesc(const ISceneNodeDesc* desc);

	virtual void _OnAddedToSceneManager(ISceneManager* mngr){}
	virtual void _OnRemovedFromSceneManager(ISceneManager* mngr){}
};



};//scene
};//mray

#endif





