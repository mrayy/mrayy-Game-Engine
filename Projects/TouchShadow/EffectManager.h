

/********************************************************************
	created:	2013/09/16
	created:	16:9:2013   20:37
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\EffectManager.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	EffectManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __EffectManager__
#define __EffectManager__




namespace mray
{
	class EffectCreatePack
	{
	public:
		math::vector2d pos;
		std::map<core::string,core::string> params;

		virtual core::string GetParam(const core::string& name,const core::string& default)
		{
			std::map<core::string,core::string>::iterator it=params.find(name);
			if(it==params.end())
				return default;
			return it->second;
		}

		void LoadFromXML(xml::XMLElement* e)
		{
			xml::XMLElement *elem=e->getSubElement("Param");
			while(elem)
			{
				params[elem->getValueString("Name")]=elem->getValueString("Value");
				elem=elem->nextSiblingElement("Param");
			}
		}
	};
	class IEffect
	{
	public:
		virtual void Draw(const math::rectf& rc)=0;
		virtual void Update(float dt)=0;
		virtual bool IsDone()=0;
	};
class IEffectFactory
{
public:
	virtual core::string GetType()const=0;

	virtual IEffect* CreateEffect(EffectCreatePack* desc)=0;

};

class EffectManager
{
protected:
	std::list<IEffect*> m_effects;
	std::map<core::string,IEffectFactory*> m_factories;
public:
	EffectManager();
	virtual~EffectManager();

	void AddFactory(IEffectFactory* fact);
	void AddEffect(IEffect* eff);

	IEffect* CreateEffect(const core::string& type,EffectCreatePack* desc);

	void Draw(const math::rectf& rc);
	void Update(float dt);

	void ClearEffects();
};

}


#endif
