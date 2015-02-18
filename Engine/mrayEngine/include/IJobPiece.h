

/********************************************************************
	created:	2011/02/06
	created:	6:2:2011   20:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IJobPiece.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IJobPiece
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IJobPiece_h__
#define IJobPiece_h__


namespace mray
{

class IJobPiece
{
public:
	class Listener
	{
	public:
		virtual void SignalJobDone(IJobPiece*j)=0;
	};
private:
protected:
public:
	Listener* listener;

	IJobPiece():listener(0)
	{}
	virtual ~IJobPiece(){}

	virtual bool Execute()=0;
	
};

}
#endif // IJobPiece_h__

