
#include "stdafx.h"
#include "SceneComponentGI.h"
#include "StreamWriter.h"
#include "StreamReader.h"
#include "SceneComponent.h"


namespace mray
{
namespace game
{

SceneComponentGI::SceneComponentGI(SceneComponent*owner):GameComponentGI(m_owner)
{
}
SceneComponentGI::~SceneComponentGI()
{
}

void SceneComponentGI::CreateWrite(OS::StreamWriter* stream)
{
	const scene::ISceneNodeDesc& desc=((SceneComponent*)m_owner)->GetNodeDesc();
	(*stream)<<desc.name<<desc.position<<desc.orintation<<desc.scale<<desc.cullType<<desc.viewDist<<desc.visible;
}
void SceneComponentGI::CreateRead(OS::StreamReader* stream)
{
	scene::ISceneNodeDesc desc;
	(*stream)>>desc.name>>desc.position>>desc.orintation>>desc.scale>>desc.cullType>>desc.viewDist>>desc.visible;
}

void SceneComponentGI::UpdateWrite(OS::StreamWriter* stream)
{
}
void SceneComponentGI::UpdateRead(OS::StreamReader* stream)
{
}


}
}
