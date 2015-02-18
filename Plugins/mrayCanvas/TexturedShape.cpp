


#include "stdafx.h"
#include "TexturedShape.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"
#include "CanvasUtilities.h"
#include "DefaultColors.h"


namespace mray
{
namespace canvas
{

TexturedShape::TexturedShape()
:m_texCoord(0,0,1,1),m_modulateColor(1,1,1,1)
{
	m_textureUnit=new video::TextureUnit();
	m_modulateColor=video::DefaultColors::Gray;
}
TexturedShape::~TexturedShape()
{
}


void TexturedShape::Draw(EMouseStatus status)
{
	if(!m_owner)
		return;

	m_owner->GetCanvasManager()->DrawShapeTextured(m_points,m_modulateColor,&m_pointTexcoords[0],m_textureUnit);
	ICShape::Draw(status);
}

void TexturedShape::SetTexture(video::ITextureCRef tex,const math::rectf& texCoords)
{
	if(m_texCoord!=texCoords)
	{
		m_texCoord=texCoords;
		CanvasUtilities::CalculateTextureCoords(m_points,m_texCoord,m_pointTexcoords);
	}
	m_textureUnit->SetTexture(tex);
}
video::TextureUnitCRef TexturedShape::GetTextureUnit()
{
	return m_textureUnit;
}
void TexturedShape::SetShapePoints(const PointList &points)
{
	ICShape::SetShapePoints(points);
	CanvasUtilities::CalculateTextureCoords(points,m_texCoord,m_pointTexcoords);
}

const std::vector<math::vector2d>& TexturedShape::GetPointTexcoords()
{
	return m_pointTexcoords;
}

}
}
