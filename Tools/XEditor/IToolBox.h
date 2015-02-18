

/********************************************************************
	created:	2010/06/11
	created:	11:6:2010   16:49
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\Editor\IToolBox.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\Editor
	file base:	IToolBox
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IToolBox_h__
#define IToolBox_h__


namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

class IToolBox
{
private:
protected:
public:
	IToolBox(){}
	virtual ~IToolBox(){}
	
	virtual void OnRender(){}
	virtual void OnExportLevelXML(xml::XMLElement*e){}
	virtual bool OnImportLevelXML(xml::XMLElement*e){return false;}

	virtual void ShowToolBox(bool s)=0;
	virtual bool IsToolBoxVisible()=0;
};

}
#endif // IToolBox_h__

