/********************************************************************
	created:	2012/12/24
	created:	24:12:2012   13:12
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SoundRenderingManager.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	SoundRenderingManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SoundRenderingManager__
#define __SoundRenderingManager__




namespace mray
{
	namespace game
	{
		class GameEntityManager;
	}
namespace VT
{

	class SoundRenderingManagerImpl;
class SoundRenderingManager
{
protected:
	SoundRenderingManagerImpl* m_impl;
public:
	SoundRenderingManager();
	virtual~SoundRenderingManager();


	void Init(game::GameEntityManager* mngr);
};

}
}


#endif