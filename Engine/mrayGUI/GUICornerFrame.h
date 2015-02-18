

#ifndef ___GUICornerFrame___
#define ___GUICornerFrame___

#include "GUIImage.h"

namespace mray{
namespace GUI{


class MRAY_DLL GUICornerFrame:public GUIImage
{
protected:
	class SCorner;
public:
	DECLARE_RTTI;
	enum ECorner{
		EC_TOP_LEFT,
		EC_TOP_RIGHT,
		EC_BOTTOM_LEFT,
		EC_BOTTOM_RIGHT,
		EC_COUNT
	};
	GUICornerFrame(GUIManager*mngr,math::Point2df pos,math::Point2df size,IEventReciver* event=0,GUIElement* parent=0,int id=0);
	virtual ~GUICornerFrame();

	void setCornerTexture(ECorner corner,const video::ITexturePtr  tex,bool front,math::vector2d size,video::SColor clr=video::SColor(255));
	SCorner*getCorner(ECorner corner){return Corners[corner];}
	
	virtual void draw(float dt);
	
	virtual void loadXMLSettings(xml::XMLElement* elem);


protected:

	class MRAY_DLL SCorner{
		GCPtr<video::TextureUnit> tex;
		bool front;
		video::SColor color;
		math::vector2d size;
	public:
		SCorner(const video::ITexturePtr&  t=0,bool f=0):tex(0),front(0),size(100),color(255)
		{
			tex=new video::TextureUnit();
			setTexture(t,f);
		};
		~SCorner(){
		}
		void setTexture(const video::TextureUnitPtr&  t,bool f){
			tex->SetTexture(t);
			front=f;
		}
		void setSize(math::vector2d sz){size=sz;}
		void setColor(video::SColor&clr){color=clr;}

		bool isFront(){return front;}
		const video::TextureUnitPtr& getTexture(){return tex;}
		math::vector2d getSize(){return size;}
		video::SColor getColor(){return color;}
	};

	GCPtr<SCorner>Corners[EC_COUNT];
};

}
}

#endif



