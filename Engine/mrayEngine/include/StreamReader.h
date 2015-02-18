
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
#include "GCPtr.h"
#include "mstring.h"
#include "ListenerContainer.h"

namespace mray{
namespace OS{

	class IStream;

	class IStreamReaderListener
	{
	public:
		virtual void OnReadData(const void*data,size_t count){}
	};
class MRAY_DLL StreamReader:public ListenerContainer<IStreamReaderListener*>
{
private:
protected:
	IStream* m_stream;
	DECLARE_FIRE_METHOD(OnReadData,(const void*data,size_t count),(data,count));

public:
	StreamReader();
	StreamReader(IStream* stream);
	virtual~StreamReader();

	void			setStream(IStream*stream);
	IStream*		getStream();

	uchar			readByte();
	int				read(void*data,size_t count);
	core::string	readLine(char comment='#');
	bool			binReadBool();
	int				binReadInt();
	long			binReadLong();
	int				txtReadInt();
	float			binReadFloat();
	float			txtReadFloat();
	core::string	readString();


	template<class T>
	int readValue(T& v)
	{
		return read((void*)&v,sizeof(T));
	}

	core::stringc	binReadStringC();
	core::stringw	binReadStringW();

	core::string	binReadString();


	template <class T>
	StreamReader& operator>>(T& v)
	{
		readValue(v);
		return *this;
	}
};

MakeSharedPtrType(StreamReader);


}
}


#endif //___StreamReader___
