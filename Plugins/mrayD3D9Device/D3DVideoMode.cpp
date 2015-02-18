

#include "stdafx.h"
#include "D3DVideoMode.h"

#include "D3D9Driver.h"


namespace mray
{
namespace video
{

	int D3DVideoMode::m_modeCount=0;

D3DVideoMode::D3DVideoMode()
{
	m_modeNumber=++m_modeCount;
	ZeroMemory( &m_dm, sizeof(D3DDISPLAYMODE) );
}
D3DVideoMode::D3DVideoMode(D3DDISPLAYMODE d3ddm)
{
	m_modeNumber=++m_modeCount;
	m_dm=d3ddm;
}
D3DVideoMode::D3DVideoMode(const D3DVideoMode&o)
{
	m_modeNumber=++m_modeCount;
	m_dm=o.m_dm;
}
D3DVideoMode::~D3DVideoMode()
{
	--m_modeCount;
}

uint D3DVideoMode::getColorDepth()const
{
	uint clrDepth=16;
	if( m_dm.Format==D3DFMT_X8R8G8B8 ||
		m_dm.Format == D3DFMT_A8R8G8B8 ||
		m_dm.Format == D3DFMT_R8G8B8 )
		clrDepth=32;

	return clrDepth;
}
core::string D3DVideoMode::GetDescription()const
{
	uint clrDepth=getColorDepth();

	return core::StringConverter::toString(m_dm.Width)+mT("x")+
		core::StringConverter::toString(m_dm.Height)+mT("@")+
		core::StringConverter::toString(getColorDepth());

}




D3DVideoModeList::D3DVideoModeList()
{
	m_driver=0;
}

void D3DVideoModeList::Enumerate(D3D9Driver* driver)
{
	m_driver=driver;
	clear();

	UINT iMode;
	LPDIRECT3D9 pD3D = m_driver->GetD3D();
	UINT adapter = m_driver->GetAdapterNumber();

	for( iMode=0; iMode < pD3D->GetAdapterModeCount( adapter, D3DFMT_R5G6B5 ); iMode++ )
	{
		D3DDISPLAYMODE displayMode;
		pD3D->EnumAdapterModes( adapter, D3DFMT_R5G6B5, iMode, &displayMode );

		// Filter out low-resolutions
		if( displayMode.Width < 640 || displayMode.Height < 400 )
			continue;

		// Check to see if it is already in the list (to filter out refresh rates)
		BOOL found = FALSE;
		for(int i=0;i<size();++i)
		{
			D3DVideoMode& mode=(*this)[i];
			D3DDISPLAYMODE oldDisp = mode.getDisplayMode();
			if( oldDisp.Width == displayMode.Width &&
				oldDisp.Height == displayMode.Height &&
				oldDisp.Format == displayMode.Format )
			{
				// Check refresh rate and favour higher if poss
				if (oldDisp.RefreshRate < displayMode.RefreshRate)
					mode.setRefreshRate(displayMode.RefreshRate);
				found = TRUE;
				break;
			}
		}

		if( !found )
			push_back( D3DVideoMode( displayMode ) );
	}
}

const D3DVideoMode* D3DVideoModeList::GetMode(const core::string& desc)const
{
	for(int i=0;i<size();++i)
	{
		if((*this)[i].GetDescription()==desc)
			return &(*this)[i];
	}
	return 0;
}
D3DVideoMode* D3DVideoModeList::GetMode(const core::string& desc)
{
	for(int i=0;i<size();++i)
	{
		if((*this)[i].GetDescription()==desc)
			return &(*this)[i];
	}
	return 0;
}

}
}

