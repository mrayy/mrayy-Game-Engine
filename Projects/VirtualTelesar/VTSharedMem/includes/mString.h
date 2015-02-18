

#ifndef __mString__
#define __mString__

#include "mTypes.h"
//#include "mAllocater.h"
//#include "PoolMemoryAllocator.h"
#include "HeapAllocator.h"
//#include "CMemory.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <tchar.h>

#define MRAY_DLL


#ifdef UNICODE

#define strToInt _wtoi
#define strToFloat _wtof
#define strToUL wcstoul
#define strToL wcstol
#define strLength wcslen
#define compareStr wcscmp
#define strCopy wcscpy
#define strSearch wcsstr
#define strPrint swprintf

#else

#define strToInt atoi
#define strToFloat atof
#define strToUL strtoul
#define strToL strtol
#define strLength strlen
#define compareStr strcmp
#define strCopy strcpy
#define strSearch strstr
#define strPrint sprintf
#endif

namespace mray{
namespace core{




template <typename T>
class tstring:public std::basic_string<T,std::char_traits<T>, std::allocator<T> >
{
	typedef std::basic_string<T> Parent;
public:

	//! Default constructor
	tstring()
	{
	}


 
	//! Constructor
	tstring(const basic_string<T>& other):Parent(other)
	{
	}




	//! Constructor for copying a tstring from a pointer with a given length
	tstring(const T* c, int length)
	{
	}



	//! Constructor for unicode and ascii tstrings
	tstring(const T* c):Parent(c)
	{
	}



	int compare_ignore_case(const tstring<T>& other)const{
		const T* mptr=c_str();
		const T* optr=other.c_str();
		while(*mptr && *optr){
			if ( tolower(*mptr)!= tolower(*optr)){
				if (*mptr < *optr)
					return 1;
				else 
					return -1;
			}

			++mptr;
			++optr;
		}
	
		if(length() < other.length())
			return 1;
		if(length() > other.length())
			return -1;
		else return 0;
	}



	//! Makes the tstring lower case.
	void make_lower()
	{
		T* ptr=_Myptr();
		while(ptr)
		{
			if (*ptr>='A' && *ptr<='Z')
				*ptr += 'a' - 'A';
			++ptr;
		}
	}



	//! Makes the tstring upper case.
	void make_upper()
	{
		T* ptr=_Myptr();
		while(ptr)
		{
			if (*ptr>='a' && *ptr<='z')
				*ptr += 'A' - 'a';
			++ptr;
		}
	}



	//! Compares the tstring ignoring case.
	/** \param other: Other tstring to compare.
	\return Returns true if the tstring are equal ignoring case. */
	bool equals_ignore_case(const tstring<T>& other) const
	{
		const T* mptr=c_str();
		const T* optr=other.c_str();
		while(*mptr && *optr){
			if ( tolower(*mptr)!= tolower(*optr))
				return false;
			++mptr;
			++optr;
		}


		return *mptr == *optr;
	}



	//! compares the first n characters of the tstrings
	bool equalsn(const T* str, int n) const
	{
		if (!str)
			return false;
		int i;
		const T* ptr=c_str();
		for(i=0; *ptr && *str && i < n; ++i)
			if (*ptr != *str)
				return false;

		// if one (or both) of the tstrings was smaller then they
		// are only equal if they have the same length
		return (i == n);// || (!*ptr && !*str);
	}



	//! replaces all characters of a special type with another one
	void replaceChar(T toReplace, T replaceWith)
	{
		T* ptr=_Myptr();
		while(*ptr)
		{
			if (*ptr == toReplace)
				*ptr = replaceWith;
			++ptr;
		}
	}

	void appendChar(T c)
	{
		if(length()==1)
		{
			resize(length()+2);
		}
		else
			resize(length()+1);
		(*this)[length()-2]=c;
		(*this)[length()-1]=0;

	}
	
	int findlast(T c)const 
	{
		const T* ptr=c_str()+length()-2;
		for (int i=length()-2; *ptr; --i,--ptr)
		{
			if(*ptr==c)
				return i;
		}
		return -1;
	}

	int findFirstNotInList(const T* c, int count) const
	{
		const T* ptr=c_str();
		for (int i=0; *ptr; ++i,++ptr)
		{
			const T* o;
			for(o=c;*o;++i)
			{
				if (*ptr == *o)
					break;
			}

			if (*o==0)
				return i;
		}

		return -1;
	}

	int findlastNotInList(const T* c, int count) const
	{
		const T* ptr=c_str()+length()-2;
		for (int i=length()-2; *ptr; --i,--ptr)
		{
			const T* o;
			for(o=c;*o;++i)
			{
				if (*ptr == *o)
					break;
			}

			if (*o==0)
				return i;
		}

		return -1;
	}

	//! trims the tstring.
	/** Removes whitespace from begin and end of the tstring. */
	void trim(const tstring<T>& whitespace)
	{
		const char*ptr=c_str();
		int begin=-1;
		for(int i=0;*ptr;++i,++ptr)
		{
			if(whitespace.find(*ptr)!=-1)
			{
				begin=i;
				break;
			}
		}
		if(begin==-1)
			return;
		// find start and end of real tstring without whitespace
		int begin = findFirstCharNotInList(whitespace.c_str(), whitespace.length());
		if (begin == -1)
			return;

		int end = findlastCharNotInList(whitespace.c_str(), whitespace.length());
		if (end == -1)
			return;

		*this = substr(begin, (end +1) - begin);
	}




	static const tstring<T> Empty;

private:


};
template <class T>
const tstring<T> tstring<T>::Empty=(T*)"";

//! Typedef for character tstrings
typedef tstring<char> stringc;

//! Typedef for wide character tstrings
typedef tstring< wchar_t> stringw; 


#ifdef UNICODE
typedef tstring< mchar> string;

#else
typedef tstring<char> string;
#endif

#define mT(v) _T(v)


int MRAY_DLL wchar_to_char(char*a,const  wchar_t *b);
int MRAY_DLL wchar_to_char(const  wchar_t *in,stringc&str);
int MRAY_DLL char_to_wchar( wchar_t*a,const char*b);
int MRAY_DLL char_to_wchar(const char *in,stringw&str);

int MRAY_DLL char_to_string(const char *in,string&str);
string MRAY_DLL char_to_string(const char *in);

int MRAY_DLL wchar_to_string(const wchar_t *in,string&str);
string MRAY_DLL wchar_to_string(const wchar_t *in);

int MRAY_DLL string_to_wchar(const string &in,stringw&str);
stringw MRAY_DLL string_to_wchar(const string &in);

stringc MRAY_DLL string_to_char(const string &in);
int MRAY_DLL string_to_char(const string &in,stringc&str);

template <class T>
tstring<T> operator+(const T*s1,const tstring<T>&s2){
	tstring<T> tmp(s1);
	tmp.append(s2);
	return tmp;
}

/*
bool isspace(char c){
	return (c==' ' || c=='\n' || c=='\t' || c=='r');
}
bool isdigit(char c){
	return (c>='0' && c<='9');
}
bool isAlpha(char c){
	return (c>='a' && c<='z' || c>='A' && c<='Z');
}*/


};//namespace
};//mray

#endif

