
#include "stdafx.h"
#include "D3D9Driver.h"
#include "D3DVideoMode.h"



namespace mray
{
namespace video
{

uint D3D9Driver::s_driverCount = 0;

D3D9Driver::D3D9Driver( LPDIRECT3D9 pD3D, uint adapterNumber, const D3DADAPTER_IDENTIFIER9& adapterIdentifer, const D3DDISPLAYMODE& desktopDisplayMode )
{
	m_driverID=++s_driverCount;
	m_pD3D = pD3D;
	m_pD3DDevice = NULL;
	m_AdapterNumber = adapterNumber;
	m_AdapterIdentifier = adapterIdentifer;
	m_DesktopDisplayMode = desktopDisplayMode;
	m_vmList=new D3DVideoModeList();
	m_vmList->Enumerate(this);
}
D3D9Driver::~D3D9Driver()
{
	--s_driverCount;
	delete m_vmList;
}

core::string D3D9Driver::DriverName() const
{
	return core::StringConverter::toString(m_AdapterIdentifier.Driver);
}
core::string D3D9Driver::DriverDescription() const
{
	return core::StringConverter::toString(m_AdapterIdentifier.Description);
}

const D3DVideoModeList& D3D9Driver::GetVideoModes()
{
	return *m_vmList;
}


}
}