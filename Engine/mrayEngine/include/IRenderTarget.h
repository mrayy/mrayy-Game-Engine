
/********************************************************************
	created:	2009/01/23
	created:	23:1:2009   12:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRenderTarget.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRenderTarget
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRenderTarget___
#define ___IRenderTarget___

#include "IRenderArea.h"



namespace mray{
namespace video{


	class IRenderTargetListener
	{
	public:
		virtual~IRenderTargetListener(){}
		virtual void OnBind(){}
		virtual void OnUnBind(){}
	};
class MRAY_DLL IRenderTarget:public IRenderArea
{
protected:
	std::vector<IRenderTargetListener*> m_raListeners;

	void _OnBind()
	{
		std::vector<IRenderTargetListener*>::iterator it=m_raListeners.begin();
		for(;it!=m_raListeners.end();++it)
		{
			(*it)->OnBind();
		}
	}
	void _OnUnBind()
	{
		std::vector<IRenderTargetListener*>::iterator it=m_raListeners.begin();
		for(;it!=m_raListeners.end();++it)
		{
			(*it)->OnUnBind();
		}
	}

public:
	IRenderTarget(){}
	virtual~IRenderTarget()
	{
		ClearListeners();
	}


	void AddListener(IRenderTargetListener*l)
	{
		m_raListeners.push_back(l);
	}
	void RemoveListener(IRenderTargetListener*l)
	{
		std::vector<IRenderTargetListener*>::iterator it=m_raListeners.begin();
		for(;it!=m_raListeners.end();++it)
		{
			if(l==*it)
			{
				m_raListeners.erase(it);
				return;
			}
		}
	}

	void ClearListeners()
	{
		
		std::vector<IRenderTargetListener*>::iterator it=m_raListeners.begin();
		for(;it!=m_raListeners.end();++it)
		{
			delete *it;
		}
		m_raListeners.clear();
	}
	virtual void clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer)=0;

	virtual void bind()=0;
	virtual void unbind()=0;

	virtual void attachRenderTarget(const ITexturePtr& tex,uint index=0)=0;
	virtual void deattachRenderTarget(const ITexturePtr& tex,uint index=0)=0;
	
	virtual const ITexturePtr& GetColorTexture(int i=0)=0;
	virtual const IHardwarePixelBufferPtr& GetDepthBuffer()=0;
	virtual const IHardwarePixelBufferPtr& GetStencilBuffer()=0;

	virtual int GetColorTextureCount()=0;

	virtual void Resize(int x,int y)=0;
};

MakeSharedPtrType(IRenderTarget)

}
}


#endif //___IRenderTarget___

