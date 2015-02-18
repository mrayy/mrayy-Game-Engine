
#include "stdafx.h"
#include "D3DDriverList.h"



namespace mray
{
namespace video
{

D3DDriverList::D3DDriverList():m_lpD3D(0)
{
}
D3DDriverList::~D3DDriverList()
{
	Clear();
}

void D3DDriverList::Clear()
{
	for(int i=0;i<size();++i)
		delete (*this)[i];
	clear();
}

void D3DDriverList::Enumerate(LPDIRECT3D9 lpD3D)
{
	m_lpD3D=lpD3D;
	Clear();

	for(int i=0;i<m_lpD3D->GetAdapterCount();++i)
	{
		D3DADAPTER_IDENTIFIER9 aID;
		D3DDISPLAYMODE dm;
		m_lpD3D->GetAdapterIdentifier(i,0,&aID);
		m_lpD3D->GetAdapterDisplayMode(i,&dm);

		push_back(new D3D9Driver(m_lpD3D,i,aID,dm));
	}
}


const D3D9Driver* D3DDriverList::GetDriver(const core::string& desc)const
{
	for(int i=0;i<size();++i)
	{
		if((*this)[i]->DriverDescription()==desc)
			return (*this)[i];
	}
	return 0;
}
D3D9Driver* D3DDriverList::GetDriver(const core::string& desc)
{
	for(int i=0;i<size();++i)
	{
		if((*this)[i]->DriverDescription()==desc)
			return (*this)[i];
	}
	return 0;
}


}
}

