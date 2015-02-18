
#ifndef ___SendPacketQueue___
#define ___SendPacketQueue___

#include <GCPtr.h>
#include <mTypes.h>
#include <NetDataTypes.h>
#include <IMutex.h>
#include <hash_set>

#include "CompileConfig.h"

namespace mray{
namespace network{

class ISocket;


class MRAY_DLL SendPacketQueue
{
public:
	MakeSharedPtrType(SPacketItem);
	class SPacketItem{
	public:
		SPacketItem(){
			packetID=~0;
			nextTime=0;
		//	next=0;
		//	more=0;
			remove=0;
			packetSendCounter=0;
		}
		virtual~SPacketItem(){
			data.clear();
		}
		bool isReliable(){
			return mode==ESM_RELIABLE || mode==ESM_RELIABLE_SEQ;
		}

	/*	void deleteAttachedItems(){

			SPacketItem*pnext=next;
			while(pnext){
				SPacketItem*tnext=pnext->next;
				pnext->drop();
				pnext=tnext;
			}
			next=0;
		}*/

		bool remove;
		int packetSendCounter;

		ESendMode mode;
		ulong packetID;
		uint  nextTime;
		CMsgBuffer data;
/*
		bool splited;
		uint splitId;
		bool more;
		SPacketItem*next;
*/
		NetAddress address;
	};


protected:
	std::list<SPacketItem*> m_sendItems;
	std::list<ulong> m_recivedSeq;

	//grave Yards
	std::list<SPacketItem*> m_packetsGraveYard;


	ulong m_lastSendID;
	ulong m_lastArrivedID;

	int m_maxResend;

	ISocket*	m_socket;
	NetAddress	m_assignedAddress;
	uint		m_nextTimeSend;

	GCPtr<OS::IMutex> m_sendItemMutex;


	bool isSeqRecived(ulong seq);

	int getHeaderSize();
	bool sendPacket(SPacketItem*item);

	int initPacketData(SPacketItem*item);
	static void toPacketData(CMsgBuffer*data,SPacketItem*item);

	SPacketItem* peekPacketFromGraveyard();
	void addPacketToGraveyard(SPacketItem* pack);
public:

	SendPacketQueue(ISocket*socket);
	virtual~SendPacketQueue();

	void update(int allowedTime);

	void addPacket(const void*data,int size,const NetAddress& address,bool immediate,ESendMode mode);
	bool gotReplay(uint packID);
	void removeSeq(ulong seq);

	// \@return: used if the msg was sequence and it shouldn't be dropped cuz it arrived at the correct time
	bool toPacket(CMsgBuffer*data,SPacketItem &item);

};


}
}



#endif



