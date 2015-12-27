
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

namespace mray{
namespace OS{

	class IStream;

class  StreamWriter
{
private:
protected:
	IStream* m_stream;
public:
	StreamWriter();
	StreamWriter(IStream* stream);
	virtual~StreamWriter();

	void			setStream(IStream*stream);
	IStream*	getStream();

	int	writeByte(uchar v);
	int	write(const void*data,size_t count);
	int	writeLine(const std::string& str);
	int	writeString(const std::string& str);
	int	binWriteInt(int v);
	int	binWriteFloat(float v);

	template<class T>
	int writeValue(const T& v)
	{
		return write((const void*)&v,sizeof(T));
	}


	template <class T>
	StreamWriter& operator<<(const T& v)
	{
		writeValue(v);
		return *this;
	}


};


}
}


#endif //___StreamWriter___
