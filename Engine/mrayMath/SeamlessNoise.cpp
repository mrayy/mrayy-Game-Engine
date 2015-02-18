
#include "SeamlessNoise.h"
#include "mMath.h"


namespace mray{
namespace math{

bool SeamlessNoise::s_init=false;

int SeamlessNoise::s_perm[];
int SeamlessNoise::s_p[];

SeamlessNoise::SeamlessNoise(unsigned int seedVal){
	s_init=false;
	this->seed(seedVal);
	m_repeat=1;
}
SeamlessNoise::SeamlessNoise(){
	if(!s_init){
		this->seed(1023058);
	}
	m_repeat=1;
}

SeamlessNoise::~SeamlessNoise(){
}


void SeamlessNoise::seed(unsigned int seedVal){
	m_rand.srand(seedVal);
	buildTable();
}

float SeamlessNoise::Noise(const vector3d&vec){
	return Noise(vec,m_repeat);
}
float SeamlessNoise::Noise(const vector3d&vec,int rep){

	float x = vec.x;
	float y = vec.y;
	float z = vec.z;


	int X = (int)floor(x) & 255,             
		Y = (int)floor(y) & 255,             
		Z = (int)floor(z) & 255;

	x -= floor(x);                       
	y -= floor(y);                        
	z -= floor(z);

	float  u = fade(x),                     
		v = fade(y),                       
		w = fade(z);


	int Xmod = (X+1) % rep;                   
	int Ymod = (Y+1) % rep;                   
	int Zmod = (Z+1) % rep;                   

	int A2 = (s_p[s_p[X]    + Y]   + Z ),
		A3 = (s_p[s_p[X]    + Y]   + Zmod),

		A5 = (s_p[s_p[X]    + Ymod]   + Z),
		A6 = (s_p[s_p[X]    + Ymod]   + Zmod),

		B2 = (s_p[s_p[Xmod] + Y]   + Z),
		B3 = (s_p[s_p[Xmod] + Y]   + Zmod),

		B5 = (s_p[s_p[Xmod] + Ymod]   + Z),
		B6 = (s_p[s_p[Xmod] + Ymod]   + Zmod);

#define LERP(t, a, b) ((a) + (t) * ((b) - (a)))

	return LERP(w,                             
		LERP(v,
		LERP(u, grad(s_p[A2], x, y,   z),   grad(s_p[B2], x-1, y,   z)),
		LERP(u, grad(s_p[A5], x, y-1, z),   grad(s_p[B5], x-1, y-1, z))),

		LERP(v,
		LERP(u, grad(s_p[A3], x, y,   z-1), grad(s_p[B3], x-1, y,   z-1)),
		LERP(u, grad(s_p[A6], x, y-1, z-1), grad(s_p[B6], x-1, y-1, z-1))));
}


void SeamlessNoise::buildTable(){
	int i, n, j;

	// Fill with zeros
	for(i=0; i<256; ++i)
		s_perm[i] = 0;

	// Fill randomly the array with number form 0 to 255
	for(i=0; i<255; ++i)
	{
		n = m_rand.rand(255);

		for(j=0; j<256; ++j)
		{

			if(s_perm[j] == n)
			{
				n = m_rand.rand(255);
				j=0;
			}
		}

		s_perm[i]  = n;
		s_p[256+i] = s_p[i] = s_perm[i];
	}
}

float SeamlessNoise::fade(float t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); 
}


float SeamlessNoise::grad(int hash,float x,float y,float z)
{
	int     h = hash & 15;      
	float  u = h < 8 ? x : y,  
		v = h < 4 ? y : h==12||h==14 ? x : z;
	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

}
}




