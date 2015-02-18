

/********************************************************************
	created:	2010/08/25
	created:	25:8:2010   23:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CurveManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CurveManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CurveManager_h__
#define CurveManager_h__


#include "mString.h"
#include "ISingleton.h"

namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace OS
{
	class IStream;
}
namespace scene
{

	class CurveNode;

class CurveManager:public ISingleton<CurveManager>
{
private:
protected:
	typedef std::map<core::string,CurveNode*> CurvesMap;
	CurvesMap m_curves;
public:
	CurveManager();
	virtual ~CurveManager();
	
	void AddCurve(CurveNode*c);
	CurveNode* GetCurve(const core::string&name);
	void RemoveCurve(const core::string&name);

	void Clear();

	void LoadCurves(xml::XMLElement*elem);
	void LoadCurves(OS::IStream*file);
};

}
}


#endif // CurveManager_h__

