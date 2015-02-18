
#include "Noise1D.h"
#include "mMath.h"


namespace mray{
namespace math{

Noise1D::Noise1D(unsigned int seedVal){
	this->seed(seedVal);
}

Noise1D::~Noise1D(){
}


void Noise1D::seed(unsigned int seedVal){
	m_rand.srand(seedVal);
	buildTable();
}

float Noise1D::Noise(float v){
	buildCoefs(v);

	float gradVals[2];

	for (int i = 0; i < 2; i++)
	{
		gradVals[i] = (v - m_vCoef[i]) * m_Table[m_iCoef[i]];
	}

	float u = (float)interp(v - m_vCoef[0]);
	float sumX1 = math::lerp(gradVals[0], gradVals[1], u);

	return sumX1;
}


void Noise1D::buildTable(){
	for (int i = 0; i < TableSize; i++)
	{
		m_Table[i] = 1.0f - ( 2.0f * m_rand.rand01() );
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

void Noise1D::buildCoefs(float v){
	int x = int(floor(v));
	int x1 = (x + 1);

	m_vCoef[0] = (float)x;
	m_vCoef[1] = (float)x1;

	m_iCoef[0] = fold(m_vCoef[0]);
	m_iCoef[1] = fold(m_vCoef[1]);
}

int Noise1D::fold(float v){
	int x = int(v) & (TableSize - 1);
	return m_perm[x];
}

//Ken Perlin's new interpolation function
//n = 6t^5 - 15t^4 + 10t^3
double Noise1D::interp(double t){
	return (6.0 * pow(t, 5.0)) - (15.0 * pow(t, 4.0)) + (10.0 * pow(t, 3.0));
}

}
}




