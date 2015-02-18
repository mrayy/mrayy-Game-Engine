

/********************************************************************
	created:	2012/09/27
	created:	27:9:2012   15:16
	filename: 	d:\Development\mrayEngine\Plugins\mrayVideoVFW\AVIVideoWriter.h
	file path:	d:\Development\mrayEngine\Plugins\mrayVideoVFW
	file base:	AVIVideoWriter
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___AVIVideoWriter___
#define ___AVIVideoWriter___


#include <mstring.h>


namespace mray
{
namespace video
{
	class ITexture;

	enum ECodecType
	{
		WMV2,
		WMV3,
		DIVX
	};

	struct AVIVideoWriterData;
class AVIVideoWriter
{
protected:

	AVIVideoWriterData* m_data;

	//uchar* m_buffer;
	int m_position;
	int m_stride;
	int m_sampleWritten;
	int m_bytesWritten;

	math::vector2d m_size;
	ECodecType m_codec;

	int m_rate;
	int m_quality;

public:
	AVIVideoWriter();
	virtual~AVIVideoWriter();
	

	bool AddFrame(video::ITexture* tex);

	bool Open(const core::string& path,const math::vector2d &size);
	
	void Close();
};

}
}

#endif
