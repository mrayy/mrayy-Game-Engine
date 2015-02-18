

/********************************************************************
	created:	2010/10/21
	created:	21:10:2010   16:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IParticleRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IParticleRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IParticleRenderer_h__
#define IParticleRenderer_h__


namespace mray
{
namespace scene
{
	class IMeshBuffer;
	class IParticle;
	class IViewNode;

class IParticleRenderer
{
private:
protected:
public:
	IParticleRenderer(){}
	virtual ~IParticleRenderer(){}

	virtual void reset(IViewNode*vnode)=0;
	virtual void addParticle(IParticle* p)=0;
	virtual void done()=0;

	virtual IMeshBuffer* getMeshBuffer()=0;
};

}
}
#endif // IParticleRenderer_h__
