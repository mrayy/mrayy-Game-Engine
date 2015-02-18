
/********************************************************************
	created:	2009/02/16
	created:	16:2:2009   0:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\FreeImageWriter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	FreeImageWriter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FreeImageWriter___
#define ___FreeImageWriter___


#include "ITexWriter.h"
#include <FreeImage.h>

namespace mray{
namespace loaders{

class MRAY_DLL FreeImageWriter:public ITexWriter
{
protected:
	static bool m_inited;
	bool m_isDummy;

	FREE_IMAGE_FORMAT m_FTType;
	core::string m_ext;
	core::string m_extDesc;

	void Initialize();

	static unsigned DLL_CALLCONV FI_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle);
	static int DLL_CALLCONV FI_SeekProc(fi_handle handle, long offset, int origin);
	static long DLL_CALLCONV FI_TellProc(fi_handle handle);

public:
	FreeImageWriter();
	FreeImageWriter(const core::string& ext,FREE_IMAGE_FORMAT ftType);
	virtual~FreeImageWriter();

	virtual bool write(video::ImageInfo*tex,OS::IStream* stream,video::ETextureType target);
	virtual bool support(video::ETextureType type);

	virtual bool canWrite(const core::string& fileName);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
};

}
}

#endif //___FreeImageWriter___
