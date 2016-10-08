
#ifndef INCLUDE_FASTTOURIERTRANSFORM
#define INCLUDE_FASTTOURIERTRANSFORM

namespace mray
{
namespace math
{

class FastFourierTransform
{
private:
	float* xre;
	float* xim;
	float* mag;
	float* fftSin;
	float* fftCos;
	int* fftBr;
	int ss;
	int ss2;
	int nu;
	int nu1;

	int BitRev(int j, int nu);
	void PrepareFFTTables();
public:
	FastFourierTransform(int pSampleSize);
	~FastFourierTransform(void);

	float* Calculate(float* pSample, size_t pSampleSize);
};

}
}

#endif
