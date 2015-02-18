

/********************************************************************
	created:	2009/05/10
	created:	10:5:2009   17:40
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\ImageController.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	ImageController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ImageController___
#define ___ImageController___

#include <GUIRotImage.h>
#include <GUIPercentAnchorEff.h>
#include <GUIFadeEffector.h>
#include <marray.h>

namespace mray{
namespace gameMod{

class ImageController
{
private:
protected:
	GCPtr<GUI::GUIFadeEffector> m_effector;
	GCPtr<GUI::GUIPercentAnchorEff> m_anchor;
	core::array<GCPtr<GUI::GUIRotImage>> m_images;
	int m_currentImage;
public:
	ImageController();
	virtual~ImageController();

	void addImage(GCPtr<GUI::GUIRotImage> image);

	void update(float dt);

	void shuffle();

};

}
}


#endif //___ImageController___
