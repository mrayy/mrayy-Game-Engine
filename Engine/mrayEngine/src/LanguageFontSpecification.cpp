#include "stdafx.h"

#include "LanguageFontSpecification.h"

#include "XMLTree.h"
#include "IFileSystem.h"


namespace mray
{
namespace GUI
{

bool LanguageFontSpecification::PatternConvert::CheckPattern(const uint*first,const uint*&ptr)
{
	const uint*p=first;
	int i=0;
	for(;i<letters.size() && *p;++i,++p)
	{
		if(letters[i]!=*p)return false;
	}
	if(i==letters.size() )
	{
		ptr=p;
		return true;
	}
	return false;
}

LanguageFontSpecification::LanguageFontSpecification()
{
}

LanguageFontSpecification::~LanguageFontSpecification()
{
}

uint LanguageFontSpecification::_GetResultLetter(uint*letters)
{
	bool frontLetter=(m_frontConnectedLetters.find(letters[2])!=m_frontConnectedLetters.end());

	LettersMap::iterator middleLetterF=m_frontConnectedLetters.find(letters[1]);
	LettersMap::iterator middleLetterM=m_middleConnectedLetters.find(letters[1]);
	LettersMap::iterator middleLetterB=m_backConnectedLetters.find(letters[1]);

	bool backLetter=(m_backConnectedLetters.find(letters[0])!=m_backConnectedLetters.end());
	
	if(!frontLetter)
	{
		if(!backLetter)return letters[1];//not connectable
		//front connection exists?
		if(middleLetterF!=m_frontConnectedLetters.end())
			return middleLetterF->second.result;
		else return letters[1];//not connectable
	}else
	{
		if(!backLetter)
		{
			if(middleLetterB!=m_backConnectedLetters.end())
				return middleLetterB->second.result;//back connection
			else return letters[1];//not connectable
		}else
		{
			if(middleLetterM!=m_middleConnectedLetters.end())
				return middleLetterM->second.result;//middle connection
			else return letters[1];//not connectable
		}
	}
	return letters[1];
}
void LanguageFontSpecification::_FlipLetters(uint* b,uint* e)
{
	for(;b<e;++b,--e)
	{
		math::Swap(*b,*e);
	}
}

std::vector<uint> LanguageFontSpecification::_FixPatterns(const std::vector<uint>&str)
{
	if(!m_patterns.size())return str;
	const uint* p=&str[0];
	std::vector<uint> result;
	int i;
	for(;*p;++p)
	{
		for(i=0;i<m_patterns.size();++i)
		{
			const uint* ptr;
			if(m_patterns[i].CheckPattern(p,ptr))
			{
				result.push_back(m_patterns[i].result);
				p=ptr-1;
			}else
				result.push_back(*p);
		}
	}
	result.push_back(0);
	return result;
}

std::vector<uint> LanguageFontSpecification::_FixLetters(const std::vector<uint>&str)
{
	const uint* p=&str[0];
	std::vector<uint> result;
	uint workingList[3]={0,0,0};
	int cnt=str.size();
	for(;*p;++p)
	{
		workingList[0]=workingList[1];
		workingList[1]=*p;
		workingList[2]=*(p+1);
		result.push_back(_GetResultLetter(workingList));
	}
	result.push_back(0);
	return result;
}


std::vector<uint> LanguageFontSpecification::_FixDirections(const std::vector<uint>&str)
{
	std::vector<uint> result=str;
	uint* p=&result[0];
	uint* b=p;

	bool rtl=false;
	int cnt=str.size();
	for(;*p;++p)
	{
		std::map<uint,LetterAttrs>::iterator it= m_letters.find(*p);
		if(it==m_letters.end())continue;
		if(it->second.RTL )
		{
			if(!rtl){
				rtl=true;
				b=p;
			}
		}else if(rtl)
		{
			rtl=false;
			_FlipLetters(b,p-1);
		}
	}
	if(rtl)
	{
		_FlipLetters(b,p-1);
	}
	return result;
}

core::UTFString LanguageFontSpecification::ProcessText(const core::UTFString&str)
{
	if(str.Length()<=1)return str;
	m_tmp.resize(str.Length()+1);
	for(int i=0;i<str.Length();++i)
		m_tmp[i]=str[i];
	m_tmp[str.Length()]=0;
	m_tmp = _FixLetters(_FixDirections(_FixPatterns(m_tmp)));
	core::UTFString result;
	result.Set(&m_tmp[0]);
	return result;
}

void LanguageFontSpecification::Clear()
{
	m_letters.clear();
	m_patterns.clear();
	m_frontConnectedLetters.clear();
	m_middleConnectedLetters.clear();
	m_backConnectedLetters.clear();
}

void LanguageFontSpecification::Load(const core::string&fileName)
{
	OS::IStreamPtr stream=gFileSystem.openFile(fileName);
	if(!stream)return;
	xml::XMLTree tree;
	if(!tree.load(stream))
	{
		stream->close();
		return;
	}
	stream->close();
	xml::XMLAttribute*attr;
	xml::XMLElement* elem=tree.getSubElement(mT("LanguageSpecification"));
	if(!elem)return;
	attr=elem->getAttribute(mT("Name"));
	if(attr)m_name=attr->value;
	xml::XMLElement* e;
	e=elem->getSubElement(mT("Letters"));
	core::UTFString tmpStr;
	if(e)
	{
		xml::xmlSubElementsMapIT it= e->getElementsBegin();
		xml::xmlSubElementsMapIT end= e->getElementsEnd();
		for(;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement* le=dynamic_cast<xml::XMLElement*>(*it);
			if(le->getName().equals_ignore_case(mT("Letter")))
			{
				LetterAttrs c;
				attr=le->getAttribute(mT("C"));
				if(attr && attr->value!=mT("")){
					
					c.c=core::StringConverter::toUInt(attr->value);
				}
				attr=le->getAttribute(mT("RTL"));
				if(attr ){
					c.RTL=core::StringConverter::toBool(attr->value);
				}
				m_letters[c.c]=c;

			}else if(le->getName().equals_ignore_case(mT("Pattern")))
			{
				PatternConvert p;
				LetterAttrs c;
				attr=le->getAttribute(mT("Result"));
				if(!attr || attr->value==mT(""))continue;
				p.result=core::StringConverter::toUInt(attr->value);
				xml::xmlSubElementsMapIT cit= le->getElementsBegin();
				for (;cit!=le->getElementsEnd();++cit)
				{
					if((*it)->GetType()!=xml::ENT_Element)continue;
					xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
					attr=e->getAttribute(mT("C"));
					if(!attr || attr->value==mT(""))continue;
					for(uint i=0;i<tmpStr.Length();++i)
					{
						p.letters.push_back(core::StringConverter::toUInt(attr->value));
					}
				}
				if(!p.letters.size())continue;
				c.c=p.result;
				attr=le->getAttribute(mT("RTL"));
				if(attr ){
					c.RTL=core::StringConverter::toBool(attr->value);
				}
				m_letters[c.c]=c;
				m_patterns.push_back(p);
			}
		}
	}
#define LoadConversion(arr,tag)\
	e=elem->getSubElement(tag);\
	if(e)\
	{\
		xml::xmlSubElementsMapIT it= e->getElementsBegin();\
		xml::xmlSubElementsMapIT end= e->getElementsEnd();\
		for(;it!=end;++it)\
		{\
			if((*it)->GetType()!=xml::ENT_Element)continue;\
			xml::XMLElement* le=dynamic_cast<xml::XMLElement*>(*it);\
			if(le->getName().equals_ignore_case(mT("Letter")))\
			{\
				LetterConvert c;\
				attr=le->getAttribute(mT("Char"));\
				if(!attr || attr->value==mT(""))continue;\
				c.letter=core::StringConverter::toUInt(attr->value);\
				attr=le->getAttribute(mT("Result"));\
				if(!attr || attr->value==mT(""))continue;\
				c.result=core::StringConverter::toUInt(attr->value);\
				arr[c.letter]=c;\
			}\
		}\
	}
	LoadConversion(m_frontConnectedLetters,mT("FrontConnected"));
	LoadConversion(m_middleConnectedLetters,mT("MiddleConnected"));
	LoadConversion(m_backConnectedLetters,mT("BackConnected"));
}

}
}
