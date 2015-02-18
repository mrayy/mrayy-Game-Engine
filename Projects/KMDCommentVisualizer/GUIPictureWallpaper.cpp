

#include "stdafx.h"
#include "GUIPictureWallpaper.h"
#include "IGUIManager.h"
#include "IVideoDevice.h"
#include "IGUIRenderer.h"
#include "GUIElementRegion.h"


#include "ShaderResourceManager.h"


namespace mray
{

namespace GUI
{
const GUID GUIPictureWallpaper::ElementType = "GUIPictureWallpaper";
/*

const core::string WallpaperFrameShader =
	"half4 main_fp(float2 texCoord : TEXCOORD0, uniform float2 texSize , "
	"uniform sampler2D texA : register(s0),uniform float alpha) : COLOR "
	"{"
	"const half width=10;"
	"half4 clr=tex2D(texA,texCoord);"
	"float tsz=texCoord*texSize;"
	"float frame=(tsz.x<width || tsz.x>texSize.x)? 1:0;"
	"frame=(tsz.y<width || tsz.y>texSize.y || frame==1)? 1:0;"
	"return half4(mix(clr.rgb,half3(1,1,1),frame),alpha);"
	"return half4(clr.rgb,alpha);"
	"}";*/


const core::string WallpaperFrameShader =
"half4 main_fp(float2 texCoord : TEXCOORD0, uniform float2 texSize , "
"uniform sampler2D texA : register(s0),uniform half alpha,uniform half gray) : COLOR "
"{"
"const half width=10;"
"half4 clr=tex2D(texA,texCoord);"
"half gclr=dot(clr.rgb,half3(0.22, 0.707, 0.071));"
"return half4(mix(clr.rgb,half3(gclr,gclr,gclr),sqrt(gray)),alpha);"
"}";

GUIPictureWallpaper::GUIPictureWallpaper(IGUIManager* m)
	:GUIStaticImage(m)
{
	m_type = &ElementType;
	m_frameSize = 0;

	video::IGPUShaderProgramPtr shader = gShaderResourceManager.getResource("WallpaperFrame");
	if (!shader)
	{
		shader = gShaderResourceManager.loadShaderFromProgram("WallpaperFrame", WallpaperFrameShader, video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg"));
		shader->setResourceName("WallpaperFrame");
		gShaderResourceManager.addResource(shader, "WallpaperFrame");
	}
	m_textureUnit->SetEdgeColor(0);
	SetStretchMode(GUI::EImage_Zoom);

}
GUIPictureWallpaper::~GUIPictureWallpaper()
{

}

void GUIPictureWallpaper::Draw(const math::rectf*vp)
{

	if (!IsVisible())return;
	IGUIManager* creator = GetCreator();
	video::IVideoDevice*device = creator->GetDevice();
	video::IGPUShaderProgram *shader = (video::IGPUShaderProgram*)gShaderResourceManager.getResource("WallpaperFrame").pointer();
	if (shader)
	{
		device->setVertexShader(0);
		device->setFragmentShader(shader);
		float a = GetDerivedAlpha();

		math::vector2d ts=GetSize();
		shader->setConstant("texSize", &ts.x, 2);
		shader->setConstant("alpha", &a, 1);
		a = 1 - a;
		shader->setConstant("gray", &a, 1);
		shader->setConstant("width", &m_frameSize, 1);
	}
	GUIStaticImage::Draw(vp);
	if (shader)
		device->setFragmentShader(0);

	IGUIElement::Draw(vp);
}


void GUIPictureWallpaper::Update(float dt)
{
	m_frameSize += 5*dt;
	if (m_frameSize > 10)
		m_frameSize = 10;
}

IMPLEMENT_RTTI(GUIPictureWallpaper, GUIStaticImage);
IMPLEMENT_ELEMENT_FACTORY(GUIPictureWallpaper);

}
}


