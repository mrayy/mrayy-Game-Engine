
#include "StdAfx.h"
#include "JFMainMenuState.h"
#include "JFStateCodes.h"
#include "GUIManager.h"

#include "Application.h"
#include "JFEnvData.h"

#include "IGUIButton.h"
#include "IGUIPanel.h"


namespace mray
{
namespace game
{


const core::string JFMainMenuState::StateName(mT("MainMenu"));


JFMainMenuState::JFMainMenuState()
{
	m_exitCode=0;
	m_guiManager=new GUI::GUIManager(JFEnvData::App->getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());
	_initLayout();
}
JFMainMenuState::~JFMainMenuState()
{
}

void JFMainMenuState::_initLayout()
{
	GUI::IGUIPanelElement* root=new GUI::IGUIPanelElement(mray::GUID(""),m_guiManager);
	root->SetSize(JFEnvData::App->GetRenderWindow()->GetSize());

	GUI::IGUIButton* btn=(GUI::IGUIButton*)m_guiManager->CreateElement(GUID(mT("Button")));
	btn->SetPosition(math::vector2d(100,100));
	btn->SetSize(math::vector2d(200,50));

	btn->SetText(core::UTFString(mT("Start Game")));
	btn->OnClick+=newClassDelegate2(mT("StartGame"),this,&JFMainMenuState::OnStartGameBtn);

	root->AddElement(btn);
	m_guiManager->SetRootElement(root);
}

void JFMainMenuState::OnEvent(Event* e)
{
	m_guiManager->OnEvent(e);
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* evt=(KeyboardEvent*)e;
		if(evt->press && evt->key==KEY_N)
		{
			m_exitCode=EJF_StartGame;
		}
	}
}
void JFMainMenuState::OnStartGameBtn(const PVOID& sender,const PVOID& arg)
{
	m_exitCode=EJF_StartGame;
}
void JFMainMenuState::OnEnter()
{
	m_exitCode=0;

	JFEnvData::App->SetActiveGUIManager(m_guiManager);
}
void JFMainMenuState::Update(float dt)
{
	m_guiManager->Update(dt);
}
void JFMainMenuState::OnExit()
{
	JFEnvData::App->SetActiveGUIManager(0);

}

uint JFMainMenuState::GetExitCode()
{
	return m_exitCode;
}

}
}


