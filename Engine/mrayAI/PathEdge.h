
/********************************************************************
	created:	2010/03/10
	created:	10:3:2010   15:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\PathEdge.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	PathEdge
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	the final path edge that's a path finder will make a list of
				it containst the begin,end and attributes about the link
*********************************************************************/

#ifndef ___PathEdge___
#define ___PathEdge___

#include "CompileConfig.h"
#include "Point3d.h"
#include <list>

namespace mray{
namespace AI{

class MRAY_AI_DLL PathEdge
{
private:
protected:
public:
	PathEdge():m_attributes(0),m_length(0)
	{}
	PathEdge(const math::vector3d&b,const math::vector3d&e):m_attributes(0)
	{
		SetEdge(b,e);
	}
	virtual~PathEdge()
	{}

	float GetLength()const
	{
		return m_length;
	}

	void SetEdge(const math::vector3d&b,const math::vector3d&e)
	{
		m_begin=b;
		m_end=e;
		m_length=(float)m_end.getDist(m_begin);
	}
	void SetBegin(const math::vector3d& b)
	{
		m_begin=b;
		m_length=(float)m_end.getDist(m_begin);
	}
	void SetEnd(const math::vector3d& e)
	{
		m_end=e;
		m_length=(float)m_end.getDist(m_begin);
	}
	const math::vector3d& GetBegin()const
	{
		return m_begin;
	}
	const math::vector3d& GetEnd()const
	{
		return m_end;
	}

	int GetAttribute()const
	{
		return m_attributes;
	}
	void SetAttribute(int a)
	{
		m_attributes=a;
	}
protected:
	float m_length;
	math::vector3d m_begin;
	math::vector3d m_end;
	int m_attributes;
};

typedef std::list<PathEdge> PathEdgeList;

}
}

#endif //___PathEdge___
