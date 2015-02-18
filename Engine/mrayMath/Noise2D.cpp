
#include "Noise2D.h"
#include "mMath.h"


namespace mray{
namespace math{

Noise2D::Noise2D(unsigned int seedVal){
	this->seed(seedVal);
}

Noise2D::~Noise2D(){
}


void Noise2D::seed(unsigned int seedVal){
	m_rand.srand(seedVal);
	buildTable();
}

double Noise2D::Noise(const vector2d&v){
	buildCoefs(v);

	double gradVals[4];

	for (int i = 0; i < 4; i++)
	{
		gradVals[i] = (v - m_vCoef[i]) .dotProduct(m_Table[m_iCoef[i]]);
	}

	float u1 = (float)interp(v.x - m_vCoef[0].x);
	float v1 = (float)interp(v.y - m_vCoef[0].y);

	double sumX1 = math::lerp(gradVals[0], gradVals[1], u1);
	double sumX2 = math::lerp(gradVals[2], gradVals[3], u1);

	double sumY1=math::lerp(sumX1,sumX2,v1);
	return sumY1;
}


void Noise2D::buildTable(){
	for (int i = 0; i < TableSize; i++)
	{
		float x=1.0f - ( 2.0f * m_rand.rand01() );
		float y=1.0f - ( 2.0f * m_rand.rand01() );
		m_Table[i] = vector2d(x,y);
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

void Noise2D::buildCoefs(const vector2d&v){
	float x = (floor(v.x));
	float x1 = (x + 1);

	float y = (floor(v.y));
	float y1 = (y + 1);

	m_vCoef[0] .set(x,y);
	m_vCoef[1] .set(x1,y);
	m_vCoef[2] .set(x,y1);
	m_vCoef[3] .set(x1,y1);

	m_iCoef[0] = fold(m_vCoef[0]);
	m_iCoef[1] = fold(m_vCoef[1]);
	m_iCoef[2] = fold(m_vCoef[2]);
	m_iCoef[3] = fold(m_vCoef[3]);
}

int Noise2D::fold(const vector2d&v){
	int x = int(v.x) & (TableSize - 1);
	int y = int(v.y) & (TableSize - 1);
	return m_perm[x+m_perm[y]];
}

//Ken Perlin's new interpolation function
//n = 6t^5 - 15t^4 + 10t^3
double Noise2D::interp(double t){
	return (6.0 * pow(t, 5.0)) - (15.0 * pow(t, 4.0)) + (10.0 * pow(t, 3.0));
}

}
}




