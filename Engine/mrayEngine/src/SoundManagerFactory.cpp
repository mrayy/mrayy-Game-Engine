#include "stdafx.h"

#include "SoundManagerFactory.h"


namespace mray{
namespace sound{

SoundManagerFactory::SoundManagerFactory()
{
}
SoundManagerFactory::~SoundManagerFactory()
{
}

void SoundManagerFactory::SetDefaultManager(const core::string& type)
{
	m_defaultManager=type;
}
ISoundManager* SoundManagerFactory::CreateDefaultManager()
{
	return CreateObject(m_defaultManager);
}


}
}


