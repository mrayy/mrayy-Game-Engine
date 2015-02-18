

/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   2:59
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\DepthVisualizer.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	DepthVisualizer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DepthVisualizer__
#define __DepthVisualizer__




namespace mray
{
namespace TBee
{
	class DepthFrame;
class DepthVisualizer
{
protected:
	DepthFrame* m_frame;

	video::ITexturePtr m_texDepth;
	video::ITexturePtr m_texNormals;
	video::ImageInfo m_depthData;
	video::ImageInfo m_normalData;

	bool m_viewNormals ;
public:
	DepthVisualizer();
	virtual~DepthVisualizer();

	void Init();

	void SetDepthFrame(DepthFrame* f){ m_frame = f; }
	DepthFrame* GetDepthFrame(){ return m_frame; }

	void SetViewNormals(bool v){ m_viewNormals = v; }
	bool IsViewNormals(){ return m_viewNormals; }

	void Update();

	video::ITexturePtr GetDepthTexture()
	{
		return m_texDepth;
	}

	video::ITexturePtr GetNormalsTexture()
	{
		return m_texNormals;
	}
};

}
}


#endif
