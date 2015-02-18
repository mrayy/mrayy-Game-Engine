
#include "Noise3D.h"
#include "mMath.h"


namespace mray{
namespace math{

Noise3D::Noise3D(unsigned int seedVal){
	this->seed(seedVal);
}

Noise3D::~Noise3D(){
}


void Noise3D::seed(unsigned int seedVal){
	m_rand.srand(seedVal);
	buildTable();
}

float Noise3D::Noise(const vector3d&v){
	buildCoefs(v);

	float gradVals[8];

	for (int i = 0; i < 8; i++)
	{
		gradVals[i] = (v - m_vCoef[i]) .dotProduct(m_Table[m_iCoef[i]]);
	}

	float u1 = (float)interp(v.x - m_vCoef[0].x);
	float v1 = (float)interp(v.y - m_vCoef[0].y);
	float w1 = (float)interp(v.z - m_vCoef[0].z);

	float sumX1 = math::lerp(gradVals[0], gradVals[1], u1);
	float sumX2 = math::lerp(gradVals[2], gradVals[3], u1);
	float sumX3 = math::lerp(gradVals[4], gradVals[5], u1);
	float sumX4 = math::lerp(gradVals[6], gradVals[7], u1);

	float sumY1=math::lerp(sumX1,sumX2,v1);
	float sumY2=math::lerp(sumX3,sumX4,v1);

	float sumZ1=math::lerp(sumY1,sumY2,w1);
	return sumZ1;
}


void Noise3D::buildTable(){
	for (int i = 0; i < TableSize; i++)
	{
		float x=1.0f - ( 2.0f * m_rand.rand01() );
		float y=1.0f - ( 2.0f * m_rand.rand01() );
		float z=1.0f - ( 2.0f * m_rand.rand01() );
		m_Table[i].set(x,y,z);
		if(m_Table[i].LengthSQ()>1)
			m_Table[i].Normalize();
	}


	//create a table of random permutations
	for (int j = 0; j < TableSize; ++j)
	{
		m_perm[j] = j;
	}

	for (int j = 0; j < TableSize; ++j)
	{
		int r = (int)m_rand.randRange(0, TableSize - 1);
		int temp = m_perm[j];
		m_perm[j] = m_perm[r];
		m_perm[r] = temp;
	}

	//duplicate this table for speed
	for (int k = TableSize; k < TableSize * 2; ++k)
	{
		m_perm[k] = m_perm[k - TableSize];
	}
}

void Noise3D::buildCoefs(const vector3d&v){
	float x = floor(v.x);
	float x1 = (x + 1);

	float y = floor(v.y);
	float y1 = (y + 1);

	float z = floor(v.z);
	float z1 = (z + 1);

	m_vCoef[0].set(x,y,z);
	m_vCoef[1].set(x1,y,z);
	m_vCoef[2].set(x,y1,z);
	m_vCoef[3].set(x1,y1,z);
	m_vCoef[4].set(x,y,z1);
	m_vCoef[5].set(x1,y,z1);
	m_vCoef[6].set(x,y1,z1);
	m_vCoef[7].set(x1,y1,z1);

	m_iCoef[0] = fold(m_vCoef[0]);
	m_iCoef[1] = fold(m_vCoef[1]);
	m_iCoef[2] = fold(m_vCoef[2]);
	m_iCoef[3] = fold(m_vCoef[3]);
	m_iCoef[4] = fold(m_vCoef[4]);
	m_iCoef[5] = fold(m_vCoef[5]);
	m_iCoef[6] = fold(m_vCoef[6]);
	m_iCoef[7] = fold(m_vCoef[7]);
}

int Noise3D::fold(const vector3d&v){
	int x = int(v.x) & (TableSize - 1);
	int y = int(v.y) & (TableSize - 1);
	int z = int(v.z) & (TableSize - 1);
	return m_perm[x+m_perm[y+m_perm[z]]];
}

//Ken Perlin's new interpolation function
//n = 6t^5 - 15t^4 + 10t^3
double Noise3D::interp(double t){
	return (6.0 * pow(t, 5.0)) - (15.0 * pow(t, 4.0)) + (10.0 * pow(t, 3.0));
}

}
}




