

#ifndef ___KEY_MAP___
#define ___KEY_MAP___

#include "KEYCode.h"

namespace mray{

enum EKEY_ACTION
{
	EKA_FORWORD=0,
	EKA_BACKWORD,
	EKA_LEFT,
	EKA_RIGHT,
	EKA_JUMP,
	EKA_CROWD,
	EKA_COUNT
};

struct KeyMap
{
	EKEY_ACTION action;
	EKEY_CODE key;
};

};//mray

#endif
