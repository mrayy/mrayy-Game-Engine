
/********************************************************************
	created:	2009/05/24
	created:	24:5:2009   18:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StreamWriter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	StreamWriter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___StreamWriter___
#define ___StreamWriter___

#include "mtypes.h"
#include "GCPtr.h"
#include "mstring.h"
#include "ListenerContainer.h"

namespace mray{
namespace OS{

	class IStream;

	class IStreamWriterListener
	{
	public:
		virtual void OnWriteData(const void*data,size_t count){}
	};

class MRAY_DLL StreamWriter:public ListenerContainer<IStreamWriterListener*>
{
private:
protected:
	IStream* m_stream;
	DECLARE_FIRE_METHOD(OnWriteData,(const void*data,size_t count),(data,count));
public:
	StreamWriter();
	StreamWriter(IStream* stream);
	virtual~StreamWriter();

	void			setStream(IStream*stream);
	IStream*	getStream();

	int	writeByte(uchar v);
	int	write(const void*data,size_t count);
	int	writeLine(const core::string& str);
	int	writeString(const core::string& str);
	int	binWriteInt(int v);
	int	txtWriteInt(int v);
	int	binWriteFloat(float v);
	int	txtWriteFloat(float v);

	template<class T>
	int writeValue(const T& v)
	{
		return write((const void*)&v,sizeof(T));
	}

	int binWriteString(const core::stringc& str);
	int binWriteString(const core::stringw& str);

	template <class T>
	StreamWriter& operator<<(const T& v)
	{
		writeValue(v);
		return *this;
	}


};

MakeSharedPtrType(StreamWriter);

}
}


#endif //___StreamWriter___
