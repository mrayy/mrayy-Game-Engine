

#ifndef ___FPSCalc___
#define ___FPSCalc___

#include "mTypes.h"
#include "CompileConfig.h"


namespace mray{
namespace core{

//! FPS(Fram per second)

	class MRAY_CORE_DLL FPSCalcImpl;
class MRAY_CORE_DLL FPSCalc
{
	FPSCalcImpl* m_impl;
public:
	FPSCalc();
	virtual ~FPSCalc();
	float getFPS()const;
	float dt()const;

	void setTimeFactor(float t);

	void resetTime(float t);

	void setFixedFPS(float fps,bool set=true);

	//! register a new frame
	void regFrame(float time);

	float getWorstFPS()const;
	float getBestFPS()const;
	float getAverageFPS()const;
};


};//core
};//mray


#endif













