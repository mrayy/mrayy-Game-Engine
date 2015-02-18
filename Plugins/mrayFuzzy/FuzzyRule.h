
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   19:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyRule.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyRule
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyRule___
#define ___FuzzyRule___


namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace AI{

	class FuzzyTerm;

class FuzzyRule
{
private:
protected:
	FuzzyTerm* m_antecedent;
	FuzzyTerm* m_consequence;

public:
	FuzzyRule(FuzzyTerm* antecedent,FuzzyTerm* consequence);
	virtual~FuzzyRule();
	
	//set antecedent's confidence to zero
	void ClearAntecedentConfidence();

	//update consequence DOM with antecedent's DOM
	void Calculate();

	virtual xml::XMLElement* exportXML(xml::XMLElement*elem);
};

}
}

#endif //___FuzzyRule___
