

#ifndef ___GUIELEMENT___
#define ___GUIELEMENT___

#include "mString.h"


#include "rect.h"

#include "Event.h"
#include "IFont.h"
#include "IEventReciver.h"
#include "IObject.h"
#include "matrix3x3.h"
#include "FontAttributes.h"
#include "GUIEffector.h"
#include "TextureUnit.h"





namespace mray{
	namespace video
	{
		class IVideoDevice;
	}
namespace GUI{

class GUIManager;

MakeSharedPtrType(GUIElement)
class MRAY_DLL GUIElement:public IObject
{
	bool useDeviceTiming;//used to tell device that this object need to be rendered in real time
	
	DECLARE_RTTI;

protected:

	enum ETextType
	{
		ETT_Caption,
		ETT_Hint,
		ETT_Count
	};

	struct SRenderText
	{
		SRenderText():dirty(false)
		{}
		core::UTFString str;
		bool dirty;
	};

	GUIManager*  guiMngr;


	bool parentEnabled;

	GCPtr<GUIElement> activeElem;

	math::Point2df m_hintPos;

	const math::Point2df& getHintPos() { return m_hintPos; }
	void setHintPos(const math::Point2df &val) { m_hintPos = val; }

	EDock m_dock;

	math::Point2df parentPos;
	math::rectf	clipRect;

	math::matrix3x3 m_absTransformationMatrix;
	math::matrix3x3 m_relTransformationMatrix;

	bool m_bShowHint;

	float m_angle;
	float m_Alpha;

	video::SColor m_Color;
	int m_Id;
	bool m_Selectable;
	bool m_bAlphaEffect;
	float m_AlphaFade;
	float m_minAlpha,m_maxAlpha;
	bool m_isVisible;
	math::Point2df m_mainSize;
	math::Point2df m_mainPos;
	bool m_bShowCaption;
	bool m_useHint;
	core::UTFString m_hintStr;
	core::UTFString m_Caption;
	bool m_enableClip;
	bool m_isEnable;
	math::Point2df m_Pos;
	math::Point2df m_Size;
	math::vector2d m_origin;

	math::Point2df m_lastPos;
	float m_lastAngle;


	core::string m_name;

	GUIElement* parent;
	std::list<GCPtr<GUIElement> > Children;

	std::list<GUIEffectorPtr> effectors;


	IEventReciver* m_event;

	video::IVideoDevice* device;

	SRenderText m_renderText[ETT_Count];




	int anchor;
	core::string m_type;

	void setTypeStr(const mchar*type){
		m_type=type;
	}


	core::UTFString& _UpdateRenderText(SRenderText& t,const core::UTFString&str);


	virtual void fillProperties();
public: 

	rwProperty<GUIElement,math::vector2d> Origin;
	rwProperty<GUIElement,int> Id;
	rwProperty<GUIElement,core::string> Name;
	rwProperty<GUIElement,bool> Selectable;
	rwProperty<GUIElement,bool> AlphaEffect;	
	rwProperty<GUIElement,float> AlphaFade;
	rwProperty<GUIElement,float> MinAlpha;
	rwProperty<GUIElement,float> MaxAlpha;
	rwProperty<GUIElement,video::SColor> Color;
	rwProperty<GUIElement,float> Alpha;
	rwProperty<GUIElement,float> Angle;

	rwProperty<GUIElement,bool> Visible;
	rwProperty<GUIElement,bool> Enable;
	rwProperty<GUIElement,bool> EnableClip;
	rwProperty<GUIElement,core::UTFString> Caption;
	rwProperty<GUIElement,core::UTFString> HintStr;
	rwProperty<GUIElement,bool> UseHint;
	rwProperty<GUIElement,bool> ShowCaption;
	rwProperty<GUIElement,math::Point2df> MainPos;
	rwProperty<GUIElement,math::Point2df> MainSize;
	rwProperty<GUIElement,math::Point2df> HintPos;

	rwProperty<GUIElement,EDock> Dock;


	GUIElement(GUIManager* manager);
	GUIElement(GUIManager* manager,GUIElement* _parent,const math::Point2df& pos,const math::Point2df& size,
		IEventReciver* event,int id);
	virtual ~GUIElement();


	const core::UTFString& getCaption() ;
	virtual void setCaption(const core::UTFString&  val) ;

	const core::string& getName() ;
	virtual void setName(const core::string&  val) ;

	const bool& getVisible() ;
	void setVisible(const bool& val) ;

	const bool& getEnable() ;
	void setEnable(const bool& val) ;

	const bool& getEnableClip() ;
	void setEnableClip(const bool& val) ;


	const core::UTFString &getHintStr() ;
	void setHintStr(const core::UTFString& val) ;

	const bool& getUseHint() ;
	void setUseHint(const bool& val) ;

	const bool& getShowCaption() ;
	void setShowCaption(const bool& val) ;

	const math::Point2df& getMainPos() ;
	void setMainPos(const math::Point2df&  val) ;

	const math::Point2df&  getMainSize() ;
	virtual void setMainSize(const math::Point2df&  val);



	const int& getId() ;
	void setId(const int& val) ;

	const bool& getSelectable() ;
	void setSelectable(const bool& val) ;

	const bool& getAlphaEffect() ;
	void setAlphaEffect(const bool& val) ;

	const float& getAlphaFade() ;
	void setAlphaFade(const float& val);

	const float& getMinAlpha();
	void setMinAlpha(const float& val) ;

	const float& getMaxAlpha();
	void setMaxAlpha(const float& val) ;

	const video::SColor& getColor() ;
	void setColor(const video::SColor& val) ;

	const float& getAlpha() ;
	void setAlpha(const float& val) ;

	const float& getAngle() ;
	void setAngle(const float& val) ;

	const math::vector2d& getOrigin() ;
	void setOrigin(const math::vector2d &val) ;

	const core::UTFString& getRenderString();

	void SetDevice(video::IVideoDevice* dev);
	video::IVideoDevice* getDevice();

	void SetManager(GUIManager* mngr);
	GUIManager* getManager();

	void addEffector(const GUIEffectorPtr&eff);
	void removeEffector(const GUIEffectorPtr&eff);
	std::list<GUIEffectorPtr> *getEffectors(){return &effectors;};
	
	math::rectf getClipRect(){return clipRect;}

	void setReciver(IEventReciver*event);


	virtual FontAttributes* getFontAttributes();

	//! using EAlignment and binary operation | (OR)
	void setAnchor(int a);


	const mray::GUI::EDock& getDock() { return m_dock; }
	void setDock(const mray::GUI::EDock &val) { m_dock = val; }

	std::list<GCPtr<GUIElement> > *getChildren(){return &Children;};

	

	bool bringToFront(GUIElement* elem);
	bool dropToBack(GUIElement* elem);


	void updateAnchor();
	void updateDock();

	void move(math::Point2df &abs);

	virtual void preDraw(float dt);
	virtual void draw(float dt);
	virtual void drawHint();

	virtual void onEvent(const EventPtr&  event);

	void removeFromParent();

	void removeChild(GUIElement* elem);

	virtual void addChild(GUIElement* elem);


	GUIElement* getElementFromPoint(math::Point2df pos);

	void updatePos();
	void clipParentPos();

	GUIElement *getElementById(int id,bool searchChildren=0);
	GUIElement *getElementByName(const core::string&name,bool searchChildren=0);

	bool isElementChild(GUIElement*  elem);

	GUIElement* getNextChild(GUIElement* elem);

	void removeAll();


	virtual GCPtr<GUIElement> getDuplicate();

	void setActiveElem(GUIElement* elem);
	GUIElement*  getActiveElem();


	float getWidth();
	float getHeight();

	GUIElement*  getParent();

	virtual void loadTextures();
	math::rectf getRect()const;
	
	bool hasChanged()const;

	bool updateRelTransformationMat();
	bool updateAbsTransformationMat(bool shouldUpdate=false);
	const math::matrix3x3 &getAbsTransformationMat()const;
	const math::matrix3x3 &getRelTransformationMat()const;
	math::vector2d getAbsPos()const;

	math::Point2df getParentPos()const;
	void setParentPos(math::Point2df p);

	const math::Point2df& getPos() const{ return m_Pos; }
	const math::Point2df& getSize()  const{ return m_Size; }
	
	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

	void showHint(bool s);

};




}
}

#endif