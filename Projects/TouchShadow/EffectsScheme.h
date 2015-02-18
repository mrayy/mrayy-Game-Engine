

/********************************************************************
	created:	2013/09/08
	created:	8:9:2013   1:58
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\EffectsScheme.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	EffectsScheme
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __EffectsScheme__
#define __EffectsScheme__



#include "EffectManager.h"

namespace mray
{

	struct InteractEffectData
	{
	public:
		core::string effect;
		EffectCreatePack pack;
		void LoadFromXML(xml::XMLElement* e)
		{
			effect=e->getValueString("Type");
			pack.LoadFromXML(e);
		}
	};
class EffectsScheme
{
protected:
	std::vector<InteractEffectData> m_effects;
public:
	EffectsScheme();
	virtual~EffectsScheme();

	void PlayRandom(const math::vector2d& pos,float strength);

	void LoadFromXML(xml::XMLElement* e);
};

}


#endif
