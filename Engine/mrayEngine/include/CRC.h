
/********************************************************************
	created:	2010/11/28
	created:	28:11:2010   0:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CRC.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CRC
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CRC_h__
#define CRC_h__


#include "CompileConfig.h"
#include "mTypes.h"
#include "GCPtr.h"

namespace mray{
namespace OS{
	class IStream;

#define INITIAL_CRC_VAL 0xffffffff

class MRAY_DLL CRC
{
	static bool m_inited;

	static uint m_crcTable[256];

	static void initCRCTable();
public:
	static uint calcCRC(const char*buff,int len,uint crcVal=INITIAL_CRC_VAL);
	static uint calcCRC(IStream* stream,uint crcVal=INITIAL_CRC_VAL);
};

}
}



#endif

