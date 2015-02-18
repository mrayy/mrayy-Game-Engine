



/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   19:25
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DDriverList.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DDriverList
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DDriverList__
#define __D3DDriverList__

#include "D3D9Driver.h"

namespace mray
{
namespace video
{


class D3DDriverList:public std::vector<D3D9Driver*>
{
private:
protected:
	LPDIRECT3D9 m_lpD3D;
public:
	D3DDriverList();
	virtual~D3DDriverList();

	void Enumerate(LPDIRECT3D9 lpD3D);

	void Clear();

	const D3D9Driver* GetDriver(const core::string& desc)const;
	D3D9Driver* GetDriver(const core::string& desc);

};

}
}

#endif
