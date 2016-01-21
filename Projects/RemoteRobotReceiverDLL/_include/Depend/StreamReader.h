
/********************************************************************
	created:	2009/05/24
	created:	24:5:2009   18:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StreamReader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	StreamReader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___StreamReader___
#define ___StreamReader___

#include "mtypes.h"

namespace mray{
namespace OS{

	class IStream;

class  StreamReader
{
private:
protected:
	IStream* m_stream;

public:
	StreamReader();
	StreamReader(IStream* stream);
	virtual~StreamReader();

	void			setStream(IStream*stream);
	IStream*		getStream();

	uchar			readByte();
	int				read(void*data,size_t count);
	std::string	readLine(char comment='#');
	bool			binReadBool();
	int				binReadInt();
	long			binReadLong();
	int				txtReadInt();
	float			binReadFloat();
	float			txtReadFloat();
	std::string		readString();


	template<class T>
	int readValue(T& v)
	{
		return read((void*)&v,sizeof(T));
	}

	std::string	binReadString();


	template <class T>
	StreamReader& operator>>(T& v)
	{
		readValue(v);
		return *this;
	}
};



}
}


#endif //___StreamReader___
