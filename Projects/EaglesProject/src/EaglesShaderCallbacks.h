
/********************************************************************
	created:	2009/04/07
	created:	7:4:2009   11:05
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\EaglesShaderCallbacks.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	EaglesShaderCallbacks
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EaglesShaderCallbacks___
#define ___EaglesShaderCallbacks___

#include <IShaderConstantsCallback.h>

namespace mray{
namespace gameMod{

	class OneTexShaderCallback:public video::IShaderConstantsCallback
	{
	protected:
		virtual void setUniform(video::IGPUShaderProgram*shader,GCPtr<video::GPUUniform> u,bool VertexU);
	public:
		OneTexShaderCallback();
		virtual ~OneTexShaderCallback();
	};

	class PhongShaderCallback:public video::IShaderConstantsCallback
	{
	protected:
		virtual void setUniform(video::IGPUShaderProgram*shader,GCPtr<video::GPUUniform> u,bool VertexU);
	public:
		PhongShaderCallback();
		virtual ~PhongShaderCallback();
	};


}
}


#endif //___EaglesShaderCallbacks___
