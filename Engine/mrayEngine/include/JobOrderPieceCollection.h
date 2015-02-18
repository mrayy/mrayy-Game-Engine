

/********************************************************************
	created:	2011/02/06
	created:	6:2:2011   20:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\JobOrderPieceCollection.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	JobOrderPieceCollection
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef JobOrderPieceCollection_h__
#define JobOrderPieceCollection_h__

#include "JobOrder.h"



namespace mray
{
	class IJobPiece;

class MRAY_DLL JobOrderPieceCollection:public JobOrder
{
private:
protected:
	std::vector<IJobPiece*> m_pieces;
	OS::IMutex* m_mutex;
public:
	JobOrderPieceCollection(OS::IMutex* mutex);
	virtual ~JobOrderPieceCollection();

	void AddPiece(IJobPiece*p);
	void ClearPieces();
	int GetPiecesCount();


	virtual bool ExecuteJob();

	void SignalJobsDone();
};

}
#endif // JobOrderPieceCollection_h__
