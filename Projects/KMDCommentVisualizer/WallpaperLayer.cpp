
#include "stdafx.h"
#include "WallpaperLayer.h"
#include "WallpaperBin.h"
#include "GUIPictureWallpaper.h"
#include "IGUIManager.h"
#include "GUIElementRegion.h"



namespace mray
{
namespace kmd
{

WallpaperLayer::WallpaperLayer()
{
	m_wallBin = new WallpaperBin();
	m_nextImage = 0;
}

WallpaperLayer::~WallpaperLayer()
{
	delete m_wallBin;
}


void WallpaperLayer::InitLayer(GUI::IGUIPanelElement* panel)
{
	GenericRenderLayer::InitLayer(panel);
	m_wallBin->LoadPicturesFromDir( gFileSystem.getAppPath()+ "..\\Data\\KMDPlenary\\Pictures\\");
}

void WallpaperLayer::UpdateLayer(float dt)
{
	GenericRenderLayer::UpdateLayer(dt);

	m_nextImage -= dt;

	if (m_nextImage <= 0)
	{
		m_nextImage = math::Randomizer::rand01() * 10 + 5;// +5;
		video::ITexturePtr img = m_wallBin->GetNextRandomPicture();
		if (!img.isNull())
		{
			CWallpaperInfo ifo;
			ifo.picture = m_guiPanel->GetCreator()->CreateElement<GUI::GUIPictureWallpaper>();
			m_guiPanel->AddElement(ifo.picture);

			float r = (float)img->getSize().x / (float)img->getSize().y;

			math::vector2d v;
			float rand = math::Randomizer::rand01() * 150 + 250;
			v.x = rand*r;
			v.y = rand;

			ifo.picture->SetSize(v);

			v.x = (math::Randomizer::rand01() *0.6 + 0.2) * m_guiPanel->GetDefaultRegion()->GetRect().getSize().x;
			v.y = (math::Randomizer::rand01() *0.6 + 0.2) * m_guiPanel->GetDefaultRegion()->GetRect().getSize().y;
			ifo.picture->SetPosition(v);
			ifo.picture->SetAngle(math::Randomizer::randRange(-15, 15));
			ifo.picture->SetImage(img);
			ifo.picture->SetAlpha(0);

			m_pictures.push_back(ifo);
		}
	}
	for (int i = 0; i < m_pictures.size(); ++i)
	{
		math::vector2d s = m_pictures[i].picture->GetSize();
		s += m_pictures[i].direction  * dt;
		m_pictures[i].picture->SetSize(s);
		float angle= m_pictures[i].picture->GetAngle();
		angle += m_pictures[i].direction * 1.5f * dt;
		m_pictures[i].picture->SetAngle(angle);


		float a = m_pictures[i].picture->GetAlpha();
		switch (m_pictures[i].status)
		{
		case CWallpaperInfo::EFadeIn:
			a += 0.1f*dt;
			if (a >= 1)
			{
				a = 1;
				m_pictures[i].status = CWallpaperInfo::EIdle;
			}
			m_pictures[i].picture->SetAlpha(a);
			break;
		case CWallpaperInfo::EIdle:

			if ((m_pictures[i].timeOut+=dt)>5)
			{
				m_pictures[i].status = CWallpaperInfo::EFadeOut;
			}
			break;
		case CWallpaperInfo::EFadeOut:
			a -= 0.1f*dt;
			if (a <=0)
			{
				m_pictures.erase(m_pictures.begin()+i);
				i -= 1;
			}
			else{
				m_pictures[i].picture->SetAlpha(a);
			}
			break;
		default:
			break;
		}
	}
}


}
}
