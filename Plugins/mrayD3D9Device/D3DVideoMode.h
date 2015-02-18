


/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   20:19
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DVideoMode.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DVideoMode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DVideoMode__
#define __D3DVideoMode__


namespace mray
{
namespace video
{
	class D3D9Driver;

class D3DVideoMode
{
private:
protected:
	static int m_modeCount;
	D3DDISPLAYMODE m_dm;
	uint m_modeNumber;
public:
	D3DVideoMode();
	D3DVideoMode(D3DDISPLAYMODE d3ddm);
	D3DVideoMode(const D3DVideoMode&o);
	virtual~D3DVideoMode();

	uint getWidth()const{return m_dm.Width;}
	uint getHeight()const{return m_dm.Height;}
	D3DFORMAT getFormat()const{return m_dm.Format;}
	uint getRefreshRate()const{return m_dm.RefreshRate;}
	uint getColorDepth()const;
	const D3DDISPLAYMODE& getDisplayMode()const{return m_dm;}
	void setRefreshRate(uint rt){m_dm.RefreshRate=rt;}
	core::string GetDescription()const;
};


class D3DVideoModeList:public std::vector<D3DVideoMode>
{
protected:
	D3D9Driver* m_driver;
public:
	D3DVideoModeList();

	void Enumerate(D3D9Driver* lpD3D);

	const D3DVideoMode* GetMode(const core::string& desc)const;
	D3DVideoMode* GetMode(const core::string& desc);
};

}
}

#endif
