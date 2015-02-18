#include "stdafx.h"

#include "DeviceCapabilites.h"
#include "mraySystem.h"
#include "ILogManager.h"
#include "StringConverter.h"


namespace mray{
namespace video{

std::vector<core::string> DeviceCapabilites::m_vendors;
bool DeviceCapabilites::m_inited=0;

DeviceCapabilites::	DeviceCapabilites(){
	mraySystem::memSet(m_features,0,sizeof(m_features));

	m_vendor=EGPU_Unkown;

	m_MaxTextureUnits=1;
	m_MaxWorldMatUnits=1;
	m_stencilBufferBits=0;
	m_numVertexBlendMat=0;
	m_numMultiRenderTargets=0;
	initVendors();
}
DeviceCapabilites::~DeviceCapabilites(){
}

void DeviceCapabilites::initVendors(){
	if(m_inited)
		return;

	m_vendors.resize(EGPU_Count);
	m_vendors[EGPU_Unkown]=mT("unkown");
	m_vendors[EGPU_NVidia]=mT("nvidia");
	m_vendors[EGPU_ATI]=mT("ati");
	m_vendors[EGPU_Intel]=mT("intel");
	m_vendors[EGPU_Matrox]=mT("matrox");
	m_vendors[EGPU_3DLabs]=mT("3dlabs");
	m_vendors[EGPU_S3]=mT("s3");

}

bool DeviceCapabilites::isFeatureSupported(EDeviceFeature feature)const {
	return m_features[feature];
}
void DeviceCapabilites::setFeature(EDeviceFeature feature,bool support){
	m_features[feature]=support;
}

core::string DeviceCapabilites::getVendorString(EGPU_Vendor v){
	if(!m_inited)
		initVendors();
	return m_vendors[v];
}
EGPU_Vendor DeviceCapabilites::getVendorFromString(const core::string& v){
	if(!m_inited)
		initVendors();
	core::string s=v;
	s.make_lower();
	for (int i=0;i<m_vendors.size();i++)
	{
		if(s==m_vendors[i])
			return (EGPU_Vendor)i;
	}
	return EGPU_Unkown;
}


bool DeviceCapabilites::isShaderProfileSupported(const core::string&p)const {
	for (int i=0;i<m_ShaderProfiles.size();i++)
	{
		if(p.equals_ignore_case(m_ShaderProfiles[i]))
			return true;
	}
	return false;
}
const std::vector<core::string>& DeviceCapabilites::getShaderProfiles()const{
	return m_ShaderProfiles;
}

void DeviceCapabilites::logInfo(ILogDevice*log){
	log->startSection(mT("Device Capabilities"));

	log->log(mT("Device Name : ")+m_deviceName,ELL_INFO);
	log->log(mT("Vendor Name : ")+getVendorString(m_vendor),ELL_INFO);


	log->log(mT("Max Textures Units		 : ")+core::StringConverter::toString(m_MaxTextureUnits),ELL_INFO);
	log->log(mT("Max World Matrix Units  : ")+core::StringConverter::toString(m_MaxWorldMatUnits),ELL_INFO);
	log->log(mT("Max Stencil Buffer Bits : ")+core::StringConverter::toString(m_stencilBufferBits),ELL_INFO);
	log->log(mT("Max Vertex Blend Matrix : ")+core::StringConverter::toString(m_numVertexBlendMat),ELL_INFO);
	log->log(mT("Max MultiRenderTarget Count : ")+core::StringConverter::toString(m_numMultiRenderTargets),ELL_INFO);
	log->log(mT("Max Lights Units		 : ")+core::StringConverter::toString(m_maxLights),ELL_INFO);
	log->log(mT("Max Ansitropy			 : ")+core::StringConverter::toString(m_MaxAnisotropy),ELL_INFO);

	log->log(mT("Features: "),ELL_INFO);
	log->log(mT("Render To Target : ")+core::StringConverter::toString(isFeatureSupported(EDF_RenderToTarget)),ELL_INFO);

	log->log(mT("Bilinear Filter: ")+core::StringConverter::toString(isFeatureSupported(EDF_BilinearFilter)),ELL_INFO);
	log->log(mT("Mipmaps: ")+core::StringConverter::toString(isFeatureSupported(EDF_MIPMAP)),ELL_INFO);
	log->log(mT("Mipmap auto update: ")+core::StringConverter::toString(isFeatureSupported(EDF_MIPMAP_AUTO_UPDATE)),ELL_INFO);
	log->log(mT("Auto Mipmap: ")+core::StringConverter::toString(isFeatureSupported(EDF_AutoMipmap)),ELL_INFO);
	log->log(mT("Stencil Buffer: ")+core::StringConverter::toString(isFeatureSupported(EDF_StencilBuffer)),ELL_INFO);
	log->log(mT("Vertex Program: ")+core::StringConverter::toString(isFeatureSupported(EDF_VertexProgram)),ELL_INFO);
	log->log(mT("Fragment Program: ")+core::StringConverter::toString(isFeatureSupported(EDF_FragmentProgram)),ELL_INFO);
	log->log(mT("Geometry Program: ")+core::StringConverter::toString(isFeatureSupported(EDF_GeometryProgram)),ELL_INFO);
	log->log(mT("Vertex Buffer Object: ")+core::StringConverter::toString(isFeatureSupported(EDF_VertexBufferObject)),ELL_INFO);
	log->log(mT("Textures Non Power of 2: ")+core::StringConverter::toString(isFeatureSupported(EDF_TextureNP2)),ELL_INFO);
	log->log(mT("Frame Buffer Object: ")+core::StringConverter::toString(isFeatureSupported(EDF_FrameBufferObject)),ELL_INFO);

	log->log(mT("Floating point textures: ")+core::StringConverter::toString(isFeatureSupported(EDF_FloatTexture)),ELL_INFO);
	log->log(mT("DXT Compression: ")+core::StringConverter::toString(isFeatureSupported(EDF_DXT_Compression)),ELL_INFO);
	log->log(mT("Packed Depth Stencil: ")+core::StringConverter::toString(isFeatureSupported(EDF_PackedDepthStencil)),ELL_INFO);
	log->log(mT("Seperate Stencil Buffer: ")+core::StringConverter::toString(isFeatureSupported(EDF_SeperateStencilBuffer)),ELL_INFO);
	log->log(mT("Multi Sample: ")+core::StringConverter::toString(isFeatureSupported(EDF_MultiSample)),ELL_INFO);
	log->log(mT("Seperate Specular Color: ")+core::StringConverter::toString(isFeatureSupported(EDF_SeparateSpecularColor)),ELL_INFO);
	log->log(mT("Cube Mapping: ")+core::StringConverter::toString(isFeatureSupported(EDF_CubeMapping)),ELL_INFO);

	log->log(mT("Scissor Test: ")+core::StringConverter::toString(isFeatureSupported(EDF_ScissorTest)),ELL_INFO);
	log->log(mT("3D Textures: ")+core::StringConverter::toString(isFeatureSupported(EDF_Texture3D)),ELL_INFO);
	log->log(mT("Point Sprites: ")+core::StringConverter::toString(isFeatureSupported(EDF_PointSprites)),ELL_INFO);
	log->log(mT("Anisotropy: ")+core::StringConverter::toString(isFeatureSupported(EDF_Anisotropy)),ELL_INFO);
	log->log(mT("Occlusion Query: ")+core::StringConverter::toString(isFeatureSupported(EDF_OcclusionQuery)),ELL_INFO);
	log->log(mT("DOT3: ")+core::StringConverter::toString(isFeatureSupported(EDF_DOT3)),ELL_INFO);
	log->log(mT("Blending: ")+core::StringConverter::toString(isFeatureSupported(EDF_Blending)),ELL_INFO);
	
	log->endSection(true);
}

}
}
