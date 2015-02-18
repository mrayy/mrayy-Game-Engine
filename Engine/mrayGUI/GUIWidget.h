
#ifndef ___GUIWidget___
#define ___GUIWidget___

#include "rect.h"
#include "ITexture.h"
#include "mArray.h"

namespace mray{
namespace GUI{


class GUIWidget
{
protected:
	std::vector<math::rectf> m_coords;
	std::vector<video::ITexturePtr > m_texture;
	int m_currentState;
	
	void setCurrentState(int index);
	int getCurrentState();

	void insertTexCoord(const math::rectf&rc,int index=-1);
	video::ITexturePtr  insertTexure(video::ITexture* tex,int index=-1);

	math::rectf getCurrentTexCoord();
	math::rectf getCurrentTexure();
public:
	GUIWidget();
	virtual~GUIWidget();
};


}
}



#endif


