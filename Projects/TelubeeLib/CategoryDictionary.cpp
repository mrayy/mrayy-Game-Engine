
#include "stdafx.h"
#include "CategoryDictionary.h"

#include <XMLTree.h>

namespace mray
{

	void CategoryDictionary::AddValue(const std::string& category, const std::string& name, const std::string& value)
	{

		CategoryMap::iterator it = _categories.find(category);
		if (it == _categories.end())
			_categories[category]=ValueMap();

		_categories[category][name] = value;
	}
	void CategoryDictionary::RemoveValue(const std::string& category, const std::string& name)
	{
		CategoryMap::iterator it = _categories.find(category);
		if (it == _categories.end())
			return ;
		ValueMap::iterator it2 = it->second.find(name);
		if (it2 == it->second.end())
			return ;
		it->second.erase(it2);
	}
	std::string CategoryDictionary::GetValue(const std::string& category, const std::string& name, const std::string& def)
	{

		CategoryMap::iterator it = _categories.find(category);
		if (it == _categories.end())
			return def;
		ValueMap::iterator it2 = it->second.find(name);
		if (it2 == it->second.end())
			return def;

		return it2->second;
	}

	CategoryDictionary::ValueMap& CategoryDictionary::GetCategory(const std::string& category)
	{
		CategoryMap::iterator it = _categories.find(category);
		if (it == _categories.end())
			return ValueMap();
		return it->second;
	}

	void CategoryDictionary::RemoveCategory(const std::string& category){
		CategoryMap::iterator it = _categories.find(category);
		if (it == _categories.end())
			return;
		_categories.erase(it);
	}
	void CategoryDictionary::Clear()
	{
		_categories.clear();
	}

	xml::XMLElement* CategoryDictionary::GetAsXML()
	{
		CategoryMap::iterator it = _categories.begin();
		xml::XMLElement *root=new xml::XMLElement("Categories");
		xml::XMLElement *e;
		for (; it != _categories.end();++it)
		{
			e = new xml::XMLElement("Category");
			e->addAttribute("Name", it->first);

			ValueMap::iterator it2 = it->second.begin();
			xml::XMLElement *e2;
			for (; it2 != it->second.end(); ++it2)
			{
				e2 = new xml::XMLElement("Value");
				e2->addAttribute("N", it2->first);
				e2->addAttribute("V", it2->second);
				e->addSubElement(e2);
			}
			root->addSubElement(e);
		}

		return root;
	}
}

