

/********************************************************************
	created:	2010/11/05
	created:	5:11:2010   15:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LanguageFontSpecification.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LanguageFontSpecification
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef LanguageFontSpecification_h__
#define LanguageFontSpecification_h__

#include "UTFString.h"
#include "mString.h"



namespace mray
{
namespace GUI
{

class MRAY_DLL LanguageFontSpecification
{
private:
protected:
	struct PatternConvert
	{
		std::vector<uint> letters;
		uint result;

		bool CheckPattern(const uint*first,const uint*&ptr);
		bool operator <(const PatternConvert&o){return letters.size()>o.letters.size();}
	};
	struct LetterConvert
	{
		uint letter;
		uint result;
	};
	struct LetterAttrs
	{
		uint c;
		bool RTL;
	};
	typedef std::map<uint,LetterConvert> LettersMap;

	std::map<uint,LetterAttrs> m_letters;
	std::vector<PatternConvert> m_patterns;
	LettersMap m_frontConnectedLetters;
	LettersMap m_middleConnectedLetters;
	LettersMap m_backConnectedLetters;


	std::vector<uint> m_tmp;

	core::string m_name;

	uint _GetResultLetter(uint*letters);
	void _FlipLetters(uint* b,uint* e);

	std::vector<uint> _FixPatterns(const std::vector<uint>&str);
	std::vector<uint> _FixLetters(const std::vector<uint>&str);
	std::vector<uint> _FixDirections(const std::vector<uint>&str);

public:
	LanguageFontSpecification();
	virtual ~LanguageFontSpecification();

	const core::string& GetName(){return m_name;}

	core::UTFString ProcessText(const core::UTFString&str);

	void Clear();
	
	void Load(const core::string&fileName);
};

}
}


#endif // LanguageFontSpecification_h__
