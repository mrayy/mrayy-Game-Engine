
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RDPacketsID.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RDPacketsID
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RDPacketsID___
#define ___RDPacketsID___


namespace mray{
namespace debug{


enum ERemoteDebugPackets
{
	ERDP_NewWatch=0x1000,
	ERDP_UpdateWatch,
	ERDP_RemoveWatch,
	ERDP_LogMessage
};

static const ushort RDDefaultPort=5005;

}
}


#endif //___RDPacketsID___
