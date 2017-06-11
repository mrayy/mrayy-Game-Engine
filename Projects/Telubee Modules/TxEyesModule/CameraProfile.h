

#ifndef CameraProfile_h__
#define CameraProfile_h__



namespace mray
{
	
class CameraProfile
{
protected:

	core::string m_name;

	struct ParamPair
	{
		core::string name;
		core::string value;
	};
	std::map<core::string,ParamPair> m_params;
public:
	CameraProfile(){}
	virtual ~CameraProfile(){}


	const core::string& GetName(){ return m_name; }

	bool GetValue(const core::string& param, core::string &v);


	void LoadFromXML(xml::XMLElement* elem);
};


class CameraProfileManager
{
protected:
	std::map<core::string, CameraProfile*> m_cameras;
public:
	CameraProfileManager();
	virtual ~CameraProfileManager();

	CameraProfile* GetProfile(const core::string& name);
	const std::map<core::string, CameraProfile*>& GetProfiles(){ return m_cameras; }

	void LoadFromXML(const core::string& path);
};

}

#endif // CameraProfile_h__
