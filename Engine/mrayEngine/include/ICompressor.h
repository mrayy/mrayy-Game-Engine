
/********************************************************************
	created:	2009/05/31
	created:	31:5:2009   19:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICompressor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICompressor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICompressor___
#define ___ICompressor___

#include "mstring.h"

namespace mray{
	namespace OS
	{
		class IStream;
	}
namespace Compress{

class ICompressor
{
private:
protected:
	core::string m_type;
public:
	ICompressor(const core::string& t):m_type(t){}
	virtual~ICompressor(){}

	virtual bool isLossless()=0;
	const core::string& getType(){return m_type;}

	virtual int compress(OS::IStream*input,OS::IStream* output)=0;
	virtual int decompress(OS::IStream*input,OS::IStream*output)=0;

};

}
}


#endif //___ICompressor___
