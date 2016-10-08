
/********************************************************************
	created:	2009/03/10
	created:	10:3:2009   22:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\Fractal.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	Fractal
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Fractal___
#define ___Fractal___


namespace mray{
namespace math{

template <class Noise,typename T>
class Fractal:public Noise
{
public:
	///FBM: The standard Fractal Brownian Motion summation
	float FBM(T vect_in, int octaves , float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f);

	///Turbulence: this should be used for gases, clouds, lava, etc, generates an inconsistent bubbly pattern
	float Turbulence(T vect_in, int octaves = 1, float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f);

	///A basic function that makes a pattern of lines in the x direction that could be interpreted as a marble look
	///try to make the output of this swirl, for added floatism
	float Marble(T vect_in, int octaves = 1, float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f);

	///This one I made myself which basically just scales the output exponentially to make an island look
	float IslandFractal(T vect_in, int octaves = 4, float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f, float oscarity = 2.37f);

	//this is a n F. Kenton Musgrave function taken and modified from Texturing and Modeling: A Procedural Approach
	//it attempts to simulate smooth valleys and sea floors with jagged peaks
	float RigidMultiFractal(T vect_in, int octaves = 2, float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f, float offset = 0.5f, float gain = 2.0f);

	//another one of F. Kenton Musgrave's from Texturing and Modeling: A Procedural Approach
	//this is like the one above but the smoother areas are only at the lower levels and it doesnt specifically create ridges
	float HeteroFractal(T vect_in, int octaves = 2, float freq = 1.0f, float persistance = 0.5f, float lacunarity = 2.0f, float offset = 0.5f);

};


template <class Noise, class T>
float Fractal<Noise,T>::FBM(T vect_in, int octaves, float freq, float persistance, float lacunarity)
{
	float total = 0.0f;
	float amplitude = 1.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += Noise(vect_in * freq) * amplitude;
		freq *= lacunarity;
		amplitude *= persistance;
	}


	return total;
}


template <class Noise, class T>
float Fractal<Noise,T>::Turbulence(T vect_in, int octaves, float freq, float persistance, float lacunarity)
{
	float total = 0.0f;
	float amplitude = 1.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += amplitude * std::abs(Noise(vect_in * freq));
		freq *= lacunarity;
		amplitude *= persistance;
	}

	return total;
}


template <class Noise, class T>
float Fractal<Noise,T>::Marble(T vect_in, int octaves, float freq, float persistance, float lacunarity)
{
	return sin(vect_in[0] + Turbulence(vect_in, octaves, freq, persistance, lacunarity));
}




template <class Noise, class T>
float Fractal<Noise,T>::RigidMultiFractal(T vect_in, int octaves, float freq, float persistance, float lacunarity, float offset, float gain)
{

	float total = 0.0f;
	float amplitude = 1.0f;
	float signal = 0.0f;
	float weight = 1.0;

	for (int i = 0; i < octaves; i++)
	{
		signal = weight * (offset - std::abs(Noise(vect_in)));
		weight = signal * gain;

		total += signal * amplitude;

		vect_in *= freq;
		freq *= lacunarity;
		amplitude *= persistance;
	}


	return total;

}

template <class Noise,class T>
float Fractal<Noise,T>::HeteroFractal(T vect_in, int octaves, float freq, float persistance, float lacunarity, float offset)
{

	float total = 1.0f;
	float amplitude = 1.0f;
	float signal = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		signal = amplitude * (Noise(vect_in) + offset);

		signal *= total;
		total += signal;

		vect_in *= freq;
		freq *= lacunarity;
		amplitude *= persistance;
	}


	return (total - 2.0f);

}


template <class Noise,class T>
float Fractal<Noise,T>::IslandFractal(T vect_in, int octaves /* = 4 */, float freq /* = 2.0f */, float persistance /* = 0.5f */, float lacunarity /* = 2.21731f */, float oscarity /* = 2.37f */)
{
	float total = 0.0f;
	float pers = 1.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += Noise(vect_in * freq) * pers;

		freq *= lacunarity;
		pers *= persistance;
	}

	total *= (std::abs(total * persistance) + (total * total * oscarity));

	return std::abs(total);
}


}
}


#endif //___Fractal___
