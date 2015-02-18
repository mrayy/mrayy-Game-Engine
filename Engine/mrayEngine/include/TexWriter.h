


#ifndef ___TEXWRITER___
#define ___TEXWRITER___


#include "ITexWriter.h"

namespace mray{
namespace video{
	class IVideoDevice;
	class ITexture;
}
namespace loaders{


class MRAY_DLL TexWriter
{
protected:
	typedef std::map<core::string,ITexWriter*> WritersMap;
	WritersMap m_writers;
public:
	TexWriter();
	virtual ~TexWriter();
	void addWriter(ITexWriter*writer);
	void removeWriter(const core::string& ext);
	ITexWriter* getWriter(const core::string&ext);

	bool write(video::ITexture* tex,OS::IStream* stream,const core::string&ext);
};
};//loaders
};//mray

#endif




