

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   14:41
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\OptiTrackDataSource.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	OptiTrackDataSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _OptiTrackDataSource_
#define _OptiTrackDataSource_



namespace mray
{
namespace animation
{
	class OptiTrackClient;
	class OptiTrackRigidBody;
}
namespace TBee
{

	class OptiTrackDataSourceListener;
class OptiTrackDataSource
{
protected:
	OptiTrackDataSourceListener* m_listener;
	animation::OptiTrackClient* m_tracker;

	struct BodyInfo
	{
		int id;
		math::vector3d position;
		math::quaternion orintation;
		core::string name;
	};
	typedef std::map<int, BodyInfo> OptiMap;
	OptiMap m_bodies;
public:
	OptiTrackDataSource();
	virtual~OptiTrackDataSource();

	void ConnectLocal();
	void Connect(const core::string& serverIP, const core::string& localIP="");
	void Disconnect();
	bool IsConnected();

	math::vector3d* GetPositionByID(int id);
	math::quaternion* GetOrientationByID(int id);


	void DataReceived(const animation::OptiTrackRigidBody* body);

};


}
}

#endif
