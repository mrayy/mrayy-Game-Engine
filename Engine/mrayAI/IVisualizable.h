

/********************************************************************
	created:	2010/06/29
	created:	29:6:2010   23:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IVisualizable.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IVisualizable
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IVisualizable_h__
#define IVisualizable_h__


namespace mray
{
namespace AI
{

	class IRenderDevice;

class IVisualizable
{
private:
protected:
public:
	IVisualizable(){}
	virtual ~IVisualizable(){}

	virtual void EnableVisualize(bool e)=0;
	virtual bool IsEnableVisualize()=0;
	virtual void Visualize(IRenderDevice*dbg)=0;

};

}
}


#endif // IVisualizable_h__
