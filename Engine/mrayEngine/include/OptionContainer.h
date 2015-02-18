

/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\OptionContainer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	OptionContainer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___OptionContainer___
#define ___OptionContainer___


#include "SOptionElement.h"
#include "compileConfig.h"


namespace mray{


class MRAY_DLL OptionContainer
{
public:
	typedef std::map<core::string,SOptionElement> OptionsMap;
protected:
	OptionsMap m_options;
public:
	OptionContainer();
	virtual~OptionContainer();

	void AddOption(const SOptionElement&elem);
	SOptionElement* GetOptionByName(const core::string &name);
	const SOptionElement* GetOptionByName(const core::string &name)const;
	const core::string& GetOptionValue(const core::string &name)const;

	SOptionElement& operator[](const core::string& name);
	const SOptionElement* operator[](const core::string& name)const;

	const OptionsMap& GetOptions()const
	{
		return m_options;
	}
	OptionsMap& GetOptions()
	{
		return m_options;
	}

	void Clear();
};

}


#endif //___OptionContainer___
