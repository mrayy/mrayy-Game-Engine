
/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   21:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLContext.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLContext
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLContext___
#define ___GLContext___


namespace mray{
namespace video{

class GLContext
{
private:
protected:
	bool m_inited;
public:
	GLContext():m_inited(false)
	{}
	virtual~GLContext(){};
	bool IsInited()const{return m_inited;}

	virtual void SetInited(){m_inited=true;}

	virtual void setCurrent()=0;
	virtual void endCurrent()=0;
	virtual void deleteContext()=0;
	virtual GLContext*duplicate()=0;

};

}
}


#endif //___GLContext___
