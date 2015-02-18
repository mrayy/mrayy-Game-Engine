

#include <stdlib.h>

//#include "mTypes.h"
#include "mMath.h"
#include "Randomizer.h"

namespace mray{
namespace math{

int Randomizer::_seed=0x0f0f0f0f;

void Randomizer::reset()
{
	_seed=0x0f0f0f0f;
}
void Randomizer::srand(int seed)
{
	_seed=seed;
	::srand(_seed);
}
int Randomizer::rand()
{/*
	const int m=2147483399;	// a non-Mersenne prime
	const int a = 40692;	// another spectral success story
	const int q = m/a;
	const int r = m%a;		// again less than q

	_seed = a * (_seed%q) - r* (_seed/q);
	if (_seed<0) _seed += m;

	return _seed;*/
	return ::rand();
}

int Randomizer::rand(int range)
{
	return rand()%range;
}
float Randomizer::rand(int range,int per)
{
	const float p=powf(10.0f,(float)per);
	const unsigned long rd=(unsigned long)(rand()*p);
	return float(unsigned long(rd)%((unsigned long)(range*p)))/(float)p;
}

float Randomizer::rand01(){
	return ((float)rand())/float(RAND_MAX);
}
float Randomizer::randRange(float low,float high){
	return low+(high-low)*rand01();
}

int Randomizer::RouletteWheelSelection(float* slices,int cnt,float maxRange){
	float r=rand((int)maxRange,4);

	float c=0;

	for(int i=0;i<cnt;++i){
		c+=slices[i];
		if(c>r)
			return i;
	}
	return 0;

}

}
}
