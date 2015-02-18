
#include "stdafx.h"
#include "IGhostInterface.h"

#include "StreamWriter.h"
#include "StreamReader.h"


namespace mray
{
namespace game
{


	
void IGhostInterface::CreateWrite(OS::StreamWriter* stream)
{
	(*stream)<<GetGUID();
}

void IGhostInterface::UpdateWrite(OS::StreamWriter* stream)
{

}

}
}
