
#include "stdafx.h"
#include "MayaAnimationTrack.h"
#include "ExporterParams.h"


namespace mray
{
namespace exporter
{

void MayaAnimationTrack::read(MFnAnimCurve* x,MFnAnimCurve* y,MFnAnimCurve* z)
{
	uint keysCount=std::min(x->numKeys(),std::min(y->numKeys(),z->numKeys()));
	keyframes.resize(keysCount);
	for(int i=0;i<keysCount;++i)
	{
		keyframes[i].t=x->time(i).value();
		if(trackType==Vector3d)
		{
			keyframes[i].v.set(x->value(i),y->value(i),z->value(i));
		}else
		{
			keyframes[i].q.fromEulerAngles(x->value(i),y->value(i),z->value(i));
		}
	}
}

MayaAnimationTrack::KeyFrame MayaAnimationTrack::GetKeyAt(float time)
{
	KeyFrame res(time,0);
	if(keyframes.size()==0)
		return res;
	if(keyframes[0].t>time)
	{
		res.v=keyframes[0].v;
		res.q=keyframes[0].q;
		return res;
	}
	for(int i=1;i<keyframes.size();++i)
	{
		if(keyframes[i].t>time)
		{
			float x=(time-keyframes[i-1].t)/(keyframes[i].t-keyframes[i-1].t);
			res.v=math::lerp(keyframes[i-1].v,keyframes[i].v,x);
			res.q.Slerp(keyframes[i-1].q,keyframes[i].q,x);
			return res;
		}
	}
	res.v=keyframes[keyframes.size()-1].v;
	res.q=keyframes[keyframes.size()-1].q;
	return res;
}


}
}
