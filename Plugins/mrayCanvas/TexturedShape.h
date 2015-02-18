


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   14:43
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\TexturedShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	TexturedShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __TexturedShape__
#define __TexturedShape__

#include "ICShape.h"

#include "TextureUnit.h"

namespace mray
{
namespace canvas
{

class TexturedShape:public ICShape
{
private:
protected:
	video::SColor m_modulateColor;
	video::TextureUnitPtr m_textureUnit;
	std::vector<math::vector2d> m_pointTexcoords;
	math::rectf m_texCoord;
public:
	TexturedShape();
	virtual~TexturedShape();

	
	virtual void Draw(EMouseStatus status);

	virtual void SetShapePoints(const PointList &points);

	void SetTexture(video::ITextureCRef tex,const math::rectf& texCoords);
	video::TextureUnitCRef GetTextureUnit();

	const std::vector<math::vector2d>& GetPointTexcoords();
};

}
}

#endif

