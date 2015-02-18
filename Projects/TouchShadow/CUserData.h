

/********************************************************************
	created:	2013/06/19
	created:	19:6:2013   11:46
	filename: 	C:\Development\IVRC\User Tracking\CUserData.h
	file path:	C:\Development\IVRC\User Tracking
	file base:	CUserData
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CUserData__
#define __CUserData__




namespace mray
{
	class UserDebugData;
class CUserData
{
protected:
	core::string m_name;
	nite::UserData m_userData;

	nite::Skeleton m_transformedSkeleton;

	UserDebugData* m_debugString;
	nite::UserTracker* m_tracker;
public:
	CUserData(const nite::UserData& state);
	virtual~CUserData();

	const core::string& GetName()const{return m_name;}
	nite::UserId GetID()const;
	bool IsVisible()const;
	bool IsLost()const;
	math::vector3d GetCenterOfMass()const;
	const nite::BoundingBox& GetBoundingBox()const;

	void SetTracker(nite::UserTracker* t){m_tracker=t;}
	nite::UserTracker* GetTracker(){return m_tracker;}
	const nite::Skeleton& GetSkeleton()const {return m_userData.getSkeleton();}
	const nite::Skeleton& GetTransformedSkeleton()const {return m_transformedSkeleton;}

	const nite::PoseData& getPose(nite::PoseType type) const {return m_userData.getPose(type);}

	void TransformByMatrix(const math::matrix4x4& m);

	void UpdateState(const nite::UserData& state);

	UserDebugData* GetDebugString(){return m_debugString;}
};

}


#endif
