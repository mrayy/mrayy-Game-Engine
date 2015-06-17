
#include "stdafx.h"
#include "RenderingStateManager.h"
#include "XMLTree.h"
#include "StateMachine.h"
#include <IThreadManager.h>

#include "BaseRenderState.h"
#include "RenderBlender.h"
#include "AppData.h"


namespace mray
{

namespace TBee
{

	class COnDoneCondition:public ICondition
	{
	protected:
		int m_code;
		StateMachine* m_machine;
	public:
		COnDoneCondition(StateMachine*m,const core::string&name,int code):ICondition(name)
		{
			m_machine=m;
			m_code=code;
		}
		virtual ~COnDoneCondition(){

		}
		virtual bool checkCondition()
		{
			IRenderingState* s = dynamic_cast<IRenderingState*>(m_machine->getActiveState());
			if (s)
				return s->GetExitCode()==m_code;
			return false;
		}
	};


RenderingStateManager::RenderingStateManager()
{
	m_oldState=0;
	m_stateMachine=new StateMachine();
	m_stateMachine->AddListener(this);

	m_blender=new RenderBlender();

	m_blender->LoadSettings("RenderStateManager");


}
RenderingStateManager::~RenderingStateManager()
{
	delete m_stateMachine;
}

void RenderingStateManager::OnStateChanged(StateMachine*,IState* oldS,IState* newS)
{
	if(m_oldState)
		m_oldState->OnExit();
	m_oldState = dynamic_cast<IRenderingState*>(oldS);
	m_blender->Reset();

	FIRE_LISTENR_METHOD(OnStateChanged, (m_oldState, dynamic_cast<IRenderingState*>(newS)))
}

void RenderingStateManager::AddState(IRenderingState* st)
{
	if(!st)
		return;
	m_stateMachine->addState(st);

}

IRenderingState* RenderingStateManager::GetActiveState()
{
	BaseRenderState* s=(BaseRenderState*) m_stateMachine->getActiveState();
	if(!s)
		return 0;

	return dynamic_cast<IRenderingState*>(s);
}

bool RenderingStateManager::OnEvent(Event* event, const math::rectf& rc)
{
	IRenderingState* s = dynamic_cast<IRenderingState*>(m_stateMachine->getActiveState());
	if(!s)
		return false;

	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent* e=(KeyboardEvent*)event;
		if(e->press)
		{
			if(e->key==KEY_F8)
			{
				//s->ForceExit();
			}
		}
	}

	return s->OnEvent(event,rc);

}
void RenderingStateManager::OnEnter(IApplicationState* prev)
{
	m_blender->Reset();
	m_oldState=0;
	IRenderingState* s = dynamic_cast<IRenderingState*>(m_stateMachine->getActiveState());
	if (s)
		s->OnEnter(0);
}
void RenderingStateManager::OnExit()
{
	if(m_oldState)
	{
		m_oldState->OnExit();
		m_oldState=0;
	}
	IRenderingState* s = dynamic_cast<IRenderingState*>(m_stateMachine->getActiveState());
	if (s)
		s->OnExit();

	IApplicationState::OnExit();
}
void RenderingStateManager::OnUpdate(float dt)
{
	IRenderingState* s = dynamic_cast<IRenderingState*>(m_stateMachine->getActiveState());
	if (s)
		s->Update(dt);
	if(m_oldState)
	{
		m_oldState->Update(dt);
	}
	m_blender->Update(dt);
	if(m_oldState && m_blender->DoneBlending())
	{
		m_oldState->OnExit();
		m_oldState=0;
	}

	if(s)
	{
		bool canSleep=s->CanSleep();
		if(m_oldState)
			canSleep&=m_oldState->CanSleep();

		if (AppData::Instance()->stereoMode == ERenderStereoMode::StereoTV)
			canSleep = false;

		if(canSleep)
		{
			float sleep=(m_sleepTime - dt)*1000;
			if(sleep>0)
			{
				OS::IThreadManager::getInstance().sleep(sleep);
			}
		}
	}

	m_stateMachine->onUpdate();


}
void RenderingStateManager::OnDraw(const math::rectf& rc,video::IRenderTarget* rt,ETargetEye eye)
{
	IRenderingState* s = dynamic_cast<IRenderingState*>(m_stateMachine->getActiveState());
	if (!s)
		return;

	video::TextureUnit t;

	if(m_oldState)
		m_oldState->Render(rc,eye);
	s->Render(rc,eye);
	
	video::IRenderTarget* vrt=s->GetLastFrame(eye);
	if(vrt)
		t.SetTexture(vrt->GetColorTexture());
	if (!vrt)
		return;
	math::vector2d r=rc.getSize();
	if(rt)
	{
		Engine::getInstance().getDevice()->setRenderTarget(rt,true,true,video::SColor(0,0,0,1));
		r = rt->GetSize();
	}
	if(m_oldState && m_oldState->GetLastFrame(eye))
	{
		m_blender->BlendImages(m_oldState->GetLastFrame(eye)->GetColorTexture(),t.GetTexture(),math::rectf(0,r));

	}else
	{
		m_blender->BlendImages(0,t.GetTexture(),math::rectf(0,r));
	}
// 	if(rt)
// 		Engine::getInstance().getDevice()->setRenderTarget(0);

}

void RenderingStateManager::SetInitialState(IRenderingState* s)
{
	m_stateMachine->setActiveState(s);
}
void RenderingStateManager::AddTransition(IRenderingState* a, IRenderingState* b,int code)
{
	if (!a || !b)return;
	core::string name = mT("Trans@") + a->getName() + "_" + b->getName() + "#";
	name+=core::StringConverter::toString(code);
	COnDoneCondition*cond= new COnDoneCondition(m_stateMachine,name,code);
	if(!m_stateMachine->addCondition(cond))
		delete cond;
	m_stateMachine->addTransition(a, b, cond);
}

void RenderingStateManager::InitStates()
{

	const std::vector<IState*>& states = m_stateMachine->getStates();

	for (int i = 0; i < states.size(); ++i)
	{
		IRenderingState* s = dynamic_cast<IRenderingState*>(states[i]);
		if (s)
		{
			s->InitState();
		}
	}
}


void RenderingStateManager::LoadSettingsXML(xml::XMLElement* e)
{
	xml::XMLAttribute*attr=0;

	const std::vector<IState*>& states= m_stateMachine->getStates();

	for (int i = 0; i < states.size(); ++i)
	{
		IRenderingState* s = dynamic_cast<IRenderingState*>(states[i]);
		if (s)
		{
			xml::XMLElement* se = e->getSubElement(s->getName());
			if (se)
				s->LoadFromXML(se);
		}
	}
}
void RenderingStateManager::WriteSettingsXML(xml::XMLElement* e)
{

	const std::vector<IState*>& states = m_stateMachine->getStates();

	for (int i = 0; i < states.size(); ++i)
	{
		IRenderingState* s = dynamic_cast<IRenderingState*>(states[i]);
		if (s)
		{
			s->WriteToXML(e);
		}
	}
}

}
}


