

/********************************************************************
	created:	2010/08/25
	created:	25:8:2010   23:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CurveNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CurveNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CurveNode_h__
#define CurveNode_h__

#include "mString.h"
#include "ICurve.h"

namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace scene
{

class CurveNode
{
private:
protected:
	core::string m_name;

	math::ICurve<math::vector3d> *m_curve;
public:
	CurveNode();
	virtual ~CurveNode();

	void SetName(const core::string&name);
	const core::string& GetName();

	void BuildCurve(math::vector3d *points,int count,int curveSubD=2);

	math::ICurve<math::vector3d>* GetCurve();
	
	void LoadXML(xml::XMLElement* elem);
};

}
}


#endif // CurveNode_h__
