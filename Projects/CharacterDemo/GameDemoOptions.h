

/********************************************************************
	created:	2010/06/30
	created:	30:6:2010   12:18
	filename: 	i:\Programing\GameEngine\mrayEngine\CharacterDemo\GameDemoOptions.h
	file path:	i:\Programing\GameEngine\mrayEngine\CharacterDemo
	file base:	GameDemoOptions
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameDemoOptions_h__
#define GameDemoOptions_h__


namespace mray
{

class GameDemoOptions
{
private:
protected:
	core::string m_confingFileName;
public:
	GameDemoOptions(const core::string&confingFileName);
	virtual~GameDemoOptions();

	virtual void exportSettings(const core::string&confingFileName);;

public:

	float viewDistance;
	bool postEffect;

	float gameSoundVol;
	
};

}
#endif // GameDemoOptions_h__
