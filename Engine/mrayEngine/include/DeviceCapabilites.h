

/********************************************************************
	created:	2009/05/21
	created:	21:5:2009   21:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\DeviceCapabilites.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	DeviceCapabilites
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DeviceCapabilites___
#define ___DeviceCapabilites___

#include "ILogDevice.h"

#include "mString.h"
#include "EDeviceFeature.h"

namespace mray{
namespace video{

	enum EGPU_Vendor
	{
		EGPU_Unkown,
		EGPU_NVidia,
		EGPU_ATI,
		EGPU_Intel,
		EGPU_Matrox,
		EGPU_3DLabs,
		EGPU_S3,
		EGPU_Count
	};

class MRAY_DLL DeviceCapabilites
{
private:
protected:

	static std::vector<core::string> m_vendors;
	static bool m_inited;
	static void initVendors();

	bool m_features[EDF_COUNT];

	EGPU_Vendor m_vendor;

	core::string m_deviceName;

	int m_MaxTextureUnits;
	int m_MaxWorldMatUnits;
	int m_stencilBufferBits;
	int m_numVertexBlendMat;
	int m_numMultiRenderTargets;
	int m_maxLights;
	int m_MaxAnisotropy;

	std::vector<core::string> m_ShaderProfiles;

public:
	DeviceCapabilites();
	virtual~DeviceCapabilites();

	void setFeature(EDeviceFeature feature,bool support);
	bool isFeatureSupported(EDeviceFeature feature)const ;

	void setDeviceName(const core::string&name){m_deviceName=name;}
	const core::string& getDeviceName()const{return m_deviceName;}

	EGPU_Vendor getVendor()const {return m_vendor;}
	void setVendor(EGPU_Vendor v){m_vendor=v;}

	void setVendorFromString(const core::string& v){
		setVendor(getVendorFromString(v));
	}

	static core::string getVendorString(EGPU_Vendor v);
	static EGPU_Vendor getVendorFromString(const core::string& v);

	void setMaxLightsUnits(int c){m_maxLights=c;}
	int getMaxLightsUnits()const {return m_maxLights;}

	void setMaxAnisotropy(int c){m_MaxAnisotropy=c;}
	int getMaxAnisotropy()const {return m_MaxAnisotropy;}

	void setMaxTextureUnits(int c){m_MaxTextureUnits=c;}
	int getMaxTextureUnits()const {return m_MaxTextureUnits;}

	void setMaxWorldMatrixUnits(int c){m_stencilBufferBits=c;}
	int getMaxWorldMatrixUnits()const {return m_stencilBufferBits;}

	void setStencilBufferBits(int c){m_stencilBufferBits=c;}
	int getStencilBufferBits()const {return m_stencilBufferBits;}

	void setNumVertexBlendMat(int c){m_numVertexBlendMat=c;}
	int getNumVertexBlendMat()const {return m_numVertexBlendMat;}

	void setNumMultiRenderTargets(int c){m_numMultiRenderTargets=c;}
	int getNumMultiRenderTargets()const {return m_numMultiRenderTargets;}

	void addShaderProfile(const core::string&p){m_ShaderProfiles.push_back(p);}
	bool isShaderProfileSupported(const core::string&p)const ;
	const std::vector<core::string>& getShaderProfiles()const;

	void logInfo(ILogDevice*dev);

};

}
}


#endif //___DeviceCapabilites___
