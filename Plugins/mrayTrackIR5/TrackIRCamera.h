


#ifndef ___TrackIRCamera___
#define ___TrackIRCamera___


namespace mray
{


class TrackIR;



class TrackIRCamera
{
protected:
	TrackIR* m_trackIR;

public:
	TrackIRCamera();
	virtual~TrackIRCamera();

	//Init the camera, return true if successful
	// TrackIR5 application should be running in the background
	bool Init(ushort programID);
	//shut the camera down and clean all resources
	bool Shutdown();

	//true if the camera is inited
	bool IsInited();

	//receive head position and rotation
	bool ReceiveData(math::vector3d& pos,math::vector3d& rot);
};

}


#endif


