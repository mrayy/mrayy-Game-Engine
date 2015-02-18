
/********************************************************************
	created:	2008/10/19
	created:	19:10:2008   19:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\BoneNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	BoneNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Bone Implementation
*********************************************************************/


#ifndef ___BoneNode___
#define ___BoneNode___

#include "IMovable.h"
#include "GCPtr.h"
#include "mString.h"



namespace mray{
namespace scene{

class Skeleton;

MakeSharedPtrType(BoneNode);

typedef std::list<BoneNodePtr> BonesList;
typedef BonesList::iterator BonesListIT;

class MRAY_DLL BoneNode:public IMovable
{
protected:
	Skeleton*m_skeleton;
	IMovable*m_parent;
	BoneNode*m_parentBone;

	MovableNodeList m_children;

	math::matrix4x4 m_relMatrix;
	math::matrix4x4 m_absTransformation;
	math::matrix4x4 m_prevTransformation;

	math::vector3d m_position;
	math::vector3d m_scale;

	math::quaternion m_orintation;
	math::quaternion m_absOrintation;

	math::vector3d m_basePosition;
	math::vector3d m_absBasePosition;

	math::quaternion m_baseOrintation;
	math::quaternion m_absBaseOrintation;

	math::matrix4x4 m_baseTransformation;
	math::matrix4x4 m_invBaseTransformation;

	bool m_updateChilds;
	bool m_absBasePosChanged;

	bool m_locked;
	bool m_posChange;
	bool m_oriChange;
	math::vector3d m_tmpPosition;
	math::quaternion m_tmpOri;

	bool m_transformationDirty;

	bool m_inheritTransformation;

	BonesList m_subBones;

	uint m_id;

	core::string m_name;

	bool NeedChildUpdate();

	void updateBaseTransformation();

public:

	BoneNode(const core::string&name,uint id,Skeleton*skeleton,BoneNode*parent);
	virtual~BoneNode();

	Skeleton* getSkeleton();
	void setSkeleton(Skeleton*s);

	virtual const core::string& getNodeName()const;
	virtual bool setNodeName(const core::string&name);

	virtual bool setID(const uint& id){m_id=id;return true;}
	virtual const uint& getID(){return m_id;}

	virtual void removeChild(IMovable*elem,bool parentSpace=false);
	virtual void addChild(IMovableCRef elem,bool parentSpace=false);
	virtual void removeFromParent(bool parentSpace=false);
	virtual const MovableNodeList& getChildren();

	virtual void SetInheritTransformation(bool inherit){ m_inheritTransformation = inherit; }
	virtual bool GetInheritTransformation()const{ return m_inheritTransformation; }

	BoneNode*getParentBone();
	void setParentBone(BoneNode*p);

	IMovable*getParent();
	virtual void setParent(IMovable*p);

	BoneNode*createChild(const core::string&name);
	void addBone(BoneNode*bone);
	BoneNode* getBone(const  core::string&name);
	const BonesList& getSubBones();

	void removeBone(BoneNode*bone);
	void removeBone(const  core::string&name);

	void lockBone();
	void unlock();



	virtual math::vector3d getAbsolutePosition()const;
	virtual math::quaternion getAbsoluteOrintation()const;
	virtual math::vector3d getAbsoluteScale()const;
	virtual const math::vector3d& getPosition()const;
	virtual const math::quaternion& getOrintation()const;
	virtual const math::vector3d& getScale()const;


	virtual bool setPosition(const math::vector3d& v);
	virtual bool setOrintation(const math::quaternion& v);
	virtual bool setScale(const math::vector3d& v);

	virtual void setBasePosition(const math::vector3d& v);
	virtual const math::vector3d& getBasePosition()const;
	virtual const math::vector3d& getAbsoluteBasePosition()const;

	virtual void setBaseOrintation(const math::quaternion& q);
	virtual const math::quaternion& getBaseOrintation()const;
	virtual const math::quaternion& getAbsoluteBaseOrintation()const;

	virtual const math::matrix4x4& getBaseTransformation()const	;
	virtual const math::matrix4x4& getInvBaseTransformation()const;


	// absTransformation * invBaseTransformation
	virtual math::matrix4x4 getFinalBoneTransformation()const;
	virtual void getFinalBoneTransformation(math::matrix4x4& m)const;

	//captures current transformation (position,rotation) and set it as base transformation
	virtual void captureTransformation();

	virtual const math::matrix4x4&getPrevAbsoluteTransformation()const;
	virtual const math::matrix4x4&getAbsoluteTransformation()const;
	virtual math::matrix4x4&getAbsoluteTransformation();
	virtual const math::matrix4x4&getRelativeTransformation()const;

	void updateRelativeTransformation();
	virtual void updateAbsoluteTransformation();

	virtual void preRender();
	void update(float dt);

	virtual const math::box3d& getBoundingBox();
	virtual math::box3d getTransformedBoundingBox();

	virtual BoneNode* getDuplicate();
};


}

}
#endif //___BoneNode___

