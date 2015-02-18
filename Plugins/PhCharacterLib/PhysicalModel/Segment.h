
/********************************************************************
	created:	2009/10/18
	created:	18:10:2009   22:46
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\Segment.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	Segment
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef ___Segment___
#define ___Segment___

#include "CompileConfig.h"
#include <list>
#include "SegmentDesc.h"

class NxActor;
namespace mray{

	namespace scene
	{
		class IDebugDrawManager;
	}


namespace PhysicsBasedCharacter
{
	class Character;
	class Segment;
	typedef std::list<Segment*> SegmentList;
	class MRAY_PH_CHARACTER_DLL Segment
	{
	private:
	protected:
	
		core::string m_name;
	
		Segment*m_parent;
		physics::IDOF6Joint3D* m_joint;
		//GCPtr<scene::ISceneNode> m_shape;
	
		physics::IPhysicalNode* m_physicalNode;
	
		SegmentList m_children;

		double m_swing1Offset,m_swing2Offset;
		scene::BoneNode* m_bone;
		Character* m_creator;
// 		math::matrix4x4 m_boneToSegment;
// 		math::matrix4x4 m_SegmentToBone;
	
	public:
		static math::vector3d matrixToEularYXZ(const math::matrix4x4& M);
		static math::vector3d quaternionToVector3D(const math::quaternion& q);

		Segment(Character* creator,Segment*parent);
		virtual~Segment();
	
		void update(float dt);
		void Visualize(scene::IDebugDrawManager* renderer);
	
		void loadFromDesc(SegmentDesc*desc,physics::IPhysicManager*phManager,scene::Skeleton* skeleton,float scale);
	
		void setName(const core::string& n);
		const core::string& getName();
	
		void setParent(Segment*p);
		Segment* getParent();

		scene::BoneNode* GetBone(){return m_bone;}
	
		SegmentList* getDirectChildren();
		void getAllChildren(SegmentList&children);
		Segment* getChild(const core::string &name);
		physics::IPhysicalNode* getActor();
		physics::IDOF6Joint3D* getJoint();


		float getMass();


		void moveGlobalTranslation(const math::vector3d& pos);
		void moveGlobalOrientation(const math::quaternion& orient);
		void setGlobalTranslation(const math::vector3d& pos);
		void setGlobalOrientation(const math::quaternion& orient);
		void setGlobalLinearVelocity(const math::vector3d& po);
		void setGlobalAngularVelocity(const math::vector3d& po);
		
		math::vector3d getGlobalAngularVelocity();
		math::quaternion getGlobalOrientation();
		math::vector3d getGlobalTranslation();
		math::vector3d getGlobalLinearVelocity();


		math::quaternion getRelativeOrientation(const math::quaternion& frame);
		math::vector3d getRelativeAngularVelocity(const math::quaternion& frame);
		math::vector3d getRelativeTranslation(const math::quaternion& frame);
		math::vector3d getRelativeLinearVelocity(const math::quaternion& frame);

		void applyGlobalTorque(const math::vector3d &torque);
		void applyRelativeTorque(const math::quaternion& frame, const math::vector3d &torque);
		void applyGlobalForce(const math::vector3d &force);
		void applyGlobalImpact(const math::vector3d &force);

		void setTargetOrientation(const math::vector3d &orient);
		void setTargetOrientation(const math::quaternion &orient);
		void setTargetAngularVelocity(const math::vector3d &velocity);
		void setAngularDriveProperties(float stifness, float damping,float torqueLimit, bool driveOrientation, bool driveVelocity);
		//For SIMBICON

		math::vector3d getSpecialTorque(const math::vector3d& ks, const math::vector3d& kd) ;
		void applySpecialTorque( const math::vector3d &torque );
		math::vector3d getSpecialAngularVelocity();
		math::quaternion getSpecialOrientation();
		
		
		math::vector3d target;
		math::quaternion startOrient;
		math::vector3d getPD_Torque(const math::quaternion& frame, float ks, float kd);

		
	};
}
}


#endif //___Segment___

