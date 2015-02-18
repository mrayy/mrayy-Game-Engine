

/********************************************************************
	created:	2010/06/21
	created:	21:6:2010   22:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\DayTimeAttributes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	DayTimeAttributes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef DayTimeAttributes_h__
#define DayTimeAttributes_h__


namespace mray
{
namespace game
{

class DayTimeAttributes
{
private:
protected:

	float m_fogStart;
	float m_fogEnd;
	video::SColor m_fogColor;

	video::SColor m_ambientColor;
	video::SColor m_sunColor;
public:
	DayTimeAttributes();
	virtual ~DayTimeAttributes();
	
};

}
}

#endif // DayTimeAttributes_h__
