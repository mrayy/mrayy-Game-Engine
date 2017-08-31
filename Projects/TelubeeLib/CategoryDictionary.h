

#ifndef __CATEGORYDICTIONARY__
#define __CATEGORYDICTIONARY__

#include <map>


namespace mray
{
class CategoryDictionary
{
public:
	typedef  std::map < std::string, std::string > ValueMap;
	typedef std::map < std::string, ValueMap> CategoryMap;

protected:
	CategoryMap _categories;
public:
	CategoryDictionary(){}
	~CategoryDictionary(){ Clear(); }


	void AddValue(const std::string& category, const std::string& name, const std::string& value);
	void RemoveValue(const std::string& category, const std::string& name);
	void RemoveCategory(const std::string& category);
	std::string GetValue(const std::string& category, const std::string& name, const std::string& def="");

	const CategoryMap& GetCategories()const { return _categories; }
	ValueMap& GetCategory(const std::string& category);

	void Clear();

	xml::XMLElement* GetAsXML();
};

}


#endif
