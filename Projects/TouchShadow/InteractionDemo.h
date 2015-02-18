
/********************************************************************
	created:	2013/09/07
	created:	7:9:2013   22:44
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\InteractionDemo.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	InteractionDemo
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __InteractionDemo__
#define __InteractionDemo__




namespace mray
{
	class UserFrameSchema;
	class ChannelMap;
	class InteractionRules;
	class EffectsScheme;
	class AudioWaveSet;
	class AreaInteraction;
	class ToolObject;

class InteractionDemo
{
protected:
	core::string m_name;
	core::string m_image;
	video::ITexturePtr m_imageTexture;
	UserFrameSchema *m_userSchema;
	ChannelMap* m_channels;
	//InteractionRules* m_rules;
	AreaInteraction* m_areaInteraction;
	EffectsScheme* m_effects;
	AudioWaveSet* m_audioWave;
	ToolObject* m_tool;
public:
	InteractionDemo();
	virtual~InteractionDemo();

	const core::string& GetName()const{return m_name;}
	const core::string& GetImageName()const{return m_image;}

	video::ITexturePtr GetImage(){return m_imageTexture;}

	const UserFrameSchema* GetUserFrame()const{return m_userSchema;}
	const ChannelMap* GetChannels()const{return m_channels;}

	AudioWaveSet* GetAudioWave(){return m_audioWave;}

	EffectsScheme* GetEffects(){return m_effects;}

	ToolObject* GetTool(){return m_tool;}


	void Start();
	void Stop();

	void LoadFromXML(xml::XMLElement* e);
	bool LoadFromXML(const core::string& path);

	void Run();
};

}


#endif
