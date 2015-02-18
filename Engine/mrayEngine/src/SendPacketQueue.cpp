#include "stdafx.h"

#include <ITimer.h>
#include <TraceManager.h>
#include <IThreadManager.h>


#include "SendPacketQueue.h"
#include "INetwork.h"
#include "Engine.h"
#include "ISocket.h"
#include "MutexLocks.h"
#include "IReliableSocket.h"

namespace mray{
namespace network{

SendPacketQueue::SendPacketQueue(ISocket*socket){
	m_lastSendID=0;
	m_socket=socket;
	m_nextTimeSend=3000;
	m_maxResend=5;

	m_sendItemMutex=OS::IThreadManager::getInstance().createMutex();

}

SendPacketQueue::~SendPacketQueue(){
	{
		std::list<SPacketItem* >::iterator it;
		it=m_sendItems.begin();
		for(;it!=m_sendItems.end();++it){
			delete (*it);
		}
		m_sendItems.clear();
	}

	{
		std::list<SPacketItem* >::iterator it;
		it=m_packetsGraveYard.begin();
		for(;it!=m_packetsGraveYard.end();++it){
			delete (*it);
		}
		m_packetsGraveYard.clear();
	}
}

bool SendPacketQueue::sendPacket(SPacketItem*item){
	INetwork*net=INetwork::getInstancePtr();
	int res=net->sendto(m_socket,(const char*)item->data.pointer(),item->data.size(),&item->address,0);

	if(res>0){
		m_socket->addBytesSent(res);
		return true;
	}
	return false;

}

SendPacketQueue::SPacketItem* SendPacketQueue::peekPacketFromGraveyard(){
	std::list<SPacketItem*>::iterator it=m_packetsGraveYard.begin();
	if(it!=m_packetsGraveYard.end()){
		SendPacketQueue::SPacketItem*ret=*it;
		m_packetsGraveYard.erase(it);
		ret->data.resize(0);
		ret->remove=0;
		ret->packetID=~0;
		ret->nextTime=0;
		ret->packetSendCounter=0;
		return ret;
	}
	return 0;
}
void SendPacketQueue::addPacketToGraveyard(SPacketItem* pack){
//	if(!pack)return;
	m_packetsGraveYard.push_back(pack);
//	pack->drop();
}

void SendPacketQueue::update(int allowedTime){

	traceFunction(eNetwork);

	INetwork*net=INetwork::getInstancePtr();

	ullong startTime=gEngine.getTimer()->getSeconds();
	std::list<SPacketItem* >::iterator it;
	std::list<SPacketItem* >::iterator it2;
	bool done=false;
	bool remove=0;
	{
		OS::ScopedLock locker(m_sendItemMutex);
		it=m_sendItems.begin();
		for(;it!=m_sendItems.end() && !done;){
				remove=0;
				int currTime = gEngine.getTimer()->getSeconds();
				if(currTime-startTime>allowedTime){
					done = true;
				}
				else{
					if((*it)->remove){
						remove=1;
					}else
					if((*it)->nextTime<=currTime){
						if((*it)->packetSendCounter<m_maxResend)
						{
						//send the packet
							if((*it)->packetID==~0 && ((*it)->mode==ESM_NOT_RELIABLE_SEQ || 
								(*it)->mode==ESM_RELIABLE_SEQ )){
									(*it)->packetID=m_lastSendID++;
							}
							if(sendPacket(*it)){
								(*it)->nextTime=currTime+m_nextTimeSend;
								(*it)->packetSendCounter++;
							}
						}else
							remove=1;
					}
					if(remove){
						it2=it;++it2;
						addPacketToGraveyard(*it);
						m_sendItems.erase(it);
						it=it2;
					}else
						++it;
				}
			}
		
	}
	
}

int SendPacketQueue::getHeaderSize(){
	static SPacketItem item;
	return sizeof(ESendMode)+sizeof(item.packetID);
}

int SendPacketQueue::initPacketData(SendPacketQueue::SPacketItem*item){
	item->data.reset();
	item->data.write(&item->mode,sizeof(item->mode));
	item->data.write(&item->packetID,sizeof(item->packetID));
	return item->data.size();
}
void SendPacketQueue::toPacketData(CMsgBuffer*data,SendPacketQueue::SPacketItem*item){
	
	item->data.resize(0);
	data->read(&item->mode,sizeof(item->mode));
	data->read(&item->packetID,sizeof(item->packetID));

	item->data.write((byte*)data->pointer()+data->getPos(),data->left());
}

void SendPacketQueue::addPacket(const void*data,int size,const NetAddress& address,bool immediate,ESendMode mode){
	traceFunction(eNetwork);

	SPacketItem*  item=peekPacketFromGraveyard();
	if(!item)
		item=new SPacketItem();
	int headerSize=getHeaderSize();
	std::list<SPacketItem*>::iterator it;
	
	if(mode==ESM_RELIABLE || mode==ESM_RELIABLE_SEQ ||  mode==ESM_NOT_RELIABLE_SEQ){
		if(immediate)
			item->packetID=m_lastSendID++;
	}
	item->mode=mode;
	item->nextTime = (uint)gEngine.getTimer()->getSeconds();

	item->address=address;

	int totalSize=headerSize+size;
	int copyed=0;

	int leftSize=0;

	copyed=size;
	initPacketData(item);
	item->data.increaseUsed(copyed);
	memcpy((byte*)item->data.pointer()+headerSize,data,copyed);
	if(item->isReliable() && m_socket->getReliableSocket()){
		m_socket->getReliableSocket()->send(item->data.pointer(),item->data.size(),immediate,item->address);
	}else{
		if(!immediate)
		{
			{
				OS::ScopedLock lock(m_sendItemMutex);
				m_sendItems.push_back(item);
				it=m_sendItems.end();--it;
			}
		}else{
			//send packet

			SPacketItem *pitem=item;
			if(immediate){
				uint currtime=(uint)gEngine.getTimer()->getSeconds();
				sendPacket(pitem);
				if(!pitem->isReliable()){
					pitem->remove=1;
				}
				else
					pitem->nextTime=currtime+m_nextTimeSend;

				if(!item->isReliable()){
					addPacketToGraveyard(item);
					item=0;
				}
			}
		}
	}
	
}

bool SendPacketQueue::gotReplay(uint packID){
	std::list<SPacketItem* >::iterator it;

	{
		OS::ScopedLock lock(m_sendItemMutex);
		it=m_sendItems.begin();
		for(;it!=m_sendItems.end();++it){
			if((*it)->packetID==packID){
				addPacketToGraveyard(*it);
				m_sendItems.erase(it);
//				printf("Got Replay %d\n",packID);
				return true;
			}
		}
		
	}
	return false;
}

bool SendPacketQueue::toPacket(CMsgBuffer*data,SPacketItem &item){
	toPacketData(data,&item);
	if(item.mode==ESM_RELIABLE_SEQ || item.mode==ESM_NOT_RELIABLE_SEQ){
//		if(item.packetID>=m_lastArrivedID)
//			return false;
		if(isSeqRecived(item.packetID))
			return false;

		m_lastArrivedID=item.packetID;
	}
	

	return true;

}

void SendPacketQueue::removeSeq(ulong seq){
	std::list<ulong>::iterator it=m_recivedSeq.begin();
	for(;it!=m_recivedSeq.end();++it){
		if((*it)==seq){
			m_recivedSeq.erase(it);
			return;
		}
	}
}
bool SendPacketQueue::isSeqRecived(ulong seq){
	std::list<ulong>::iterator it=m_recivedSeq.begin();
	for(;it!=m_recivedSeq.end();++it){
		if((*it)==seq)return true;
	}
	return false;
}


}
}


