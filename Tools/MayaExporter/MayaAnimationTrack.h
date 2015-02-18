


/********************************************************************
	created:	2012/02/13
	created:	13:2:2012   15:17
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaAnimationTrack.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaAnimationTrack
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaAnimationTrack__
#define __MayaAnimationTrack__


namespace mray
{
namespace exporter
{
	class AnimationTrackDesc;

class MayaAnimationTrack
{
private:
protected:
public:
	enum EType
	{
		Vector3d,
		Quaternion
	};
	struct KeyFrame
	{
		KeyFrame():t(0)
		{}
		KeyFrame(float time,const math::vector3d& val):t(time),v(val)
		{}
		KeyFrame(float time,const math::quaternion& quat):t(time),q(quat)
		{}
		double t;
	//	union
	//	{
		math::vector3d v;
		math::quaternion q;
	//	};
	};

	EType trackType;
	MString name;
	std::vector<KeyFrame> keyframes;
public:
	MayaAnimationTrack():trackType(Vector3d)
	{}
	virtual~MayaAnimationTrack(){}

	KeyFrame GetKeyAt(float time);
	void read(MFnAnimCurve* x,MFnAnimCurve* y,MFnAnimCurve* z);
	
};


}
}

#endif
