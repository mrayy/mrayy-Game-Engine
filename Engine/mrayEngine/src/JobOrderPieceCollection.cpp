#include "stdafx.h"

#include "JobOrderPieceCollection.h"
#include "IJobPiece.h"
#include "IMutex.h"

namespace mray
{

JobOrderPieceCollection::JobOrderPieceCollection(OS::IMutex* mutex):m_mutex(mutex)
{
}
JobOrderPieceCollection::~JobOrderPieceCollection()
{
	delete m_mutex;
}

void JobOrderPieceCollection::AddPiece(IJobPiece*p)
{
//	m_mutex->lock();
	m_pieces.push_back(p);
//	m_mutex->unlock();
}
void JobOrderPieceCollection::ClearPieces()
{
//	m_mutex->lock();
	m_pieces.clear();
//	m_mutex->unlock();
}

int JobOrderPieceCollection::GetPiecesCount()
{
//	m_mutex->lock();
	int sz=m_pieces.size();
//	m_mutex->unlock();

	return sz;
	
}

bool JobOrderPieceCollection::ExecuteJob()
{
//	m_mutex->lock();
	bool res=true;
	for (int i=0;i<m_pieces.size();++i)
	{
		res&=m_pieces[i]->Execute();
	}
//	m_mutex->unlock();
	return res;
}

void JobOrderPieceCollection::SignalJobsDone()
{
	for (int i=0;i<m_pieces.size();++i)
	{
		if(m_pieces[i]->listener)
			m_pieces[i]->listener->SignalJobDone(m_pieces[i]);
	}
}

}

