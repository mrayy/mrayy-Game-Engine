
#include "stdafx.h"
#include "FilledShape.h"
#include "ICanvasObject.h"
#include "CanvasManager.h"


namespace mray
{
namespace canvas
{

FilledShape::FilledShape():m_color(1,1,1,1)
{
}
FilledShape::~FilledShape()
{
}

void FilledShape::Draw(EMouseStatus status)
{
	if(!m_owner)
		return;
		
	m_owner->GetCanvasManager()->DrawShapeFilled(m_points,m_color);
	ICShape::Draw(status);
}

}
}