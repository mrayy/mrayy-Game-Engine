
#ifndef ___MRAY_DEV___
#define ___MRAY_DEV___

#include "IVideoDevice.h"


#include "SColor.h"




#include "mString.h"

//#include "EDeviceType.h"
#include "EDeviceFeature.h"
#include "GCPtr.h"
#include "KeyCode.h"
#include "RenderMaterial.h"
#include "videoCommon.h"
#include "IRenderTarget.h"
#include "IHardwareIndexBuffer.h"

#include "ITexture.h"

namespace mray
{

	class BenchmarkItem;
namespace OS{
	class IStream;
}
namespace scene{
	class S3DVertex;

	class IMeshBuffer;
	class SMesh;
	class LightNode;

	enum EVertexCoordType;

	class ViewPort;
}

namespace loaders{
	class TexLoader;
	class TexWriter;
}

namespace video{

class IShaderConstantsCallback;
class GPUShader;
class IHardwareStreamBuffer;
class StreamsData;
class IndexData;
class DeviceCapabilites;
class RenderWindow;

/*!
    Input status structure used to store inputs for keyboard and mouse
  */
/*
struct SInputStatus
{
	struct {
		EKEY_CODE lastKeyPressed;
		bool keys[256];
		bool bKeys[256];
		bool ctrl,shift,alt;
	}keyboard;

	struct {
		math::Point2df mouseSpeed;
		math::Point2df mousePos;
		bool leftPressed;
		bool rightPressed;
		bool middlePressed;
	}mouse;
};*/
class MRAY_DLL mrayDev:public IVideoDevice
{
protected:
	

	BenchmarkItem* m_benchmarkItem;

	EState m_RenderStates[RS_RenderStates_Count];
	bool m_dirtyRenderStates[RS_RenderStates_Count];

	scene::ViewPort* m_activeViewport;

	ERenderMode currentRenderMode;

	IGPUShaderProgram* m_geomShader;
	IGPUShaderProgram* m_vertexShader;
	IGPUShaderProgram* m_fragmentShader;

	std::vector<math::Plane> m_clipPlanes;

	bool m_clipPlanesDirty;

	int m_primitivesDrawn;
	int m_batchesDrawn;
	
	int currDrawingObjects;//this counter is used for the objects that needs
	//many frames to be finally finished


	std::vector<video::ITexture*>currTextures;

	bool m_enabledStreams[EMST_StreamsCount];

public:
	mrayDev();
	virtual ~mrayDev();
	
	virtual void mrayDev::setClearColor(const video::SColor &color);

	//! set matrix to certian matrix state
	/*!
	  \param transormation state enum 
	  \param matrix to set
	  \sa ETransformationState
	*/
	virtual void setTransformationState(ETransformationState state,const math::matrix4x4&mat);

	void resetStatistics()
	{
		m_primitivesDrawn=false;
		m_batchesDrawn=false;
	}
	
	/*!
	    return the number of primitives been drawn
	  */
	int getPrimitiveDrawnCount(){return m_primitivesDrawn;}
	int getBatchDrawnCount(){return m_batchesDrawn;}
	
	/*!
	    begin the scene
	  */
	virtual void begin(bool clearColorBuf=true,bool clearDepthBuf=true);

	
	virtual bool setRenderState(RenderStates flag,bool set);
	virtual bool getRenderState(RenderStates flag) ;


	void SetClipPlane(int i,const math::Plane&p);
	void AddClipPlane(const math::Plane&p);
	void RemoveClipPlane(int i);
	void ClearClipPlanes();

	//! set viewport 
	virtual void setViewport(scene::ViewPort* vp);
	//! get viewport 
	virtual scene::ViewPort* getViewport()
	{
		return m_activeViewport;
	}

	virtual bool useTexture(int stage,video::TextureUnit*tex);
	virtual void disableTexture(int stage);

	virtual bool useShader(GPUShader* shader);
	virtual bool setVertexShader(IGPUShaderProgram* shader);
	virtual bool setFragmentShader(IGPUShaderProgram* shader);
	virtual bool setGeometryShader(IGPUShaderProgram* shader);
	virtual IGPUShaderProgram* getVertexShader(){return m_vertexShader;}
	virtual IGPUShaderProgram* getFragmentShader(){return m_fragmentShader;}
	virtual IGPUShaderProgram* getGeometryShader(){return m_geomShader;}
	virtual void unuseShader();
	const GPUShaderPtr& getActiveShader();

	virtual void useRenderPass(video::RenderPass* p);

	//! add dynamic light
	virtual void addDynamicLight(scene::LightNode*light);
	virtual void disableAllLights();

	virtual bool setRenderTarget(IRenderTargetPtr rt,bool clearBackBuffer=true,bool clearZBuffer=true,const video::SColor &clearColor=video::SColor(0,0,0,0));
	

	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

	///////

	virtual void enableStream(EMeshStreamType stream,bool e);
	virtual bool isStreamEnabled(EMeshStreamType stream);

	virtual void drawElements(video::IndexData* ib,video::EMeshRenderType rtype);
	virtual void drawElementsUP(const void*indices,IHardwareIndexBuffer::EIndexType type,uint startIndex,int count,video::EMeshRenderType rtype);

};

}
}

#endif


