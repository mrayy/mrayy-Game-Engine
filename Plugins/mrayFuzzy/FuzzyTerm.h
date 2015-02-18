
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:15
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyTerm.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyTerm
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyTerm___
#define ___FuzzyTerm___


namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace AI{

class FuzzyTerm
{
private:
protected:
public:
	FuzzyTerm(){}
	virtual~FuzzyTerm(){}

	virtual float GetDOM()=0;
	virtual void ClearDOM()=0;

	virtual void ORWithDOM(float dom)=0;
	
	virtual xml::XMLElement* exportXML(xml::XMLElement*elem)=0;

};

}
}


#endif //___FuzzyTerm___
