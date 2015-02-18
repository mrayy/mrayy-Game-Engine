



/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   15:36
	filename: 	d:\Development\mrayEngine\Engine\mrayD3DDevice\D3D9Driver.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3DDevice
	file base:	D3D9Driver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3D9Driver__
#define __D3D9Driver__

#include "mTypes.h"
#include "mString.h"

namespace mray
{
namespace video
{
	class D3DVideoModeList;

class D3D9Driver
{
private:
protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	uint m_AdapterNumber;
	D3DADAPTER_IDENTIFIER9 m_AdapterIdentifier;
	D3DDISPLAYMODE m_DesktopDisplayMode;

	uint m_driverID;

	D3DVideoModeList* m_vmList;

	static uint s_driverCount;

public:
	D3D9Driver( LPDIRECT3D9 pD3D, uint adapterNumber, const D3DADAPTER_IDENTIFIER9& adapterIdentifer, const D3DDISPLAYMODE& desktopDisplayMode );
	virtual~D3D9Driver();

	core::string DriverName() const;
	core::string DriverDescription() const;

	LPDIRECT3D9 GetD3D() { return m_pD3D; }
	LPDIRECT3DDEVICE9 GetD3DDevice() { return m_pD3DDevice; }
	void SetD3DDevice(LPDIRECT3DDEVICE9 pD3DDevice) { m_pD3DDevice = pD3DDevice; }
	unsigned int GetAdapterNumber() const { return m_AdapterNumber; }
	const D3DADAPTER_IDENTIFIER9& GetAdapterIdentifier() const { return m_AdapterIdentifier; }
	const D3DDISPLAYMODE& GetDesktopMode() const { return m_DesktopDisplayMode; }

	const D3DVideoModeList& GetVideoModes();

};


}
}

#endif