

/********************************************************************
	created:	2013/08/24
	created:	24:8:2013   16:32
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\UserFrame.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	UserFrame
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __UserFrame__
#define __UserFrame__




namespace mray
{

	class UserFrame;
	class InteractionAreaSchema;
	class UserFrameSchema;

	struct InteractionArea
	{
	public:
		InteractionArea(const InteractionAreaSchema& s,UserFrame* o);

		core::string name;
		UserFrame* owner;
		math::rectf percentageArea;
		math::rectf absArea;
		float weight;
		float intersectionAmount;

		math::rectf intersectRect;

		bool absolute;
		nite::JointType targetJoint;

		void Reset(){weight=intersectionAmount=0;}

	};

	class CUserData;
typedef std::vector<InteractionArea> IntractAreaList;
class UserFrame
{
protected:
	std::vector<uchar> m_frameData;
	math::rectf m_userRect;
	int m_userID;
	IntractAreaList m_interactionAreas;
	std::map<core::string,int> m_areaMap;
	GCPtr<video::ITexture> m_texture;
	CUserData* m_userData;
public:
	UserFrame();
	virtual~UserFrame();

	void SetID(int id){m_userID=id;}
	int GetID(){return m_userID;}
	void SetBoundingBox(const math::rectf& bb);
	const math::rectf& GetBoundingBox(){return m_userRect;}

	void SetUserData(CUserData* d){m_userData=d;}
	CUserData* GetUserData(){return m_userData;}

	std::vector<uchar>& GetFrameData(){return m_frameData;}
	
	IntractAreaList& GetInteractionAreas(){return m_interactionAreas;}
	InteractionArea* GetArea(const core::string&name);

	void RefreshTexture();
	bool CalculateBoundingBoxes();
	void CalculateWeights();
	video::ITexture* GetTexture(){return m_texture;}

	void LoadSchema(const UserFrameSchema* s);

};

}


#endif
