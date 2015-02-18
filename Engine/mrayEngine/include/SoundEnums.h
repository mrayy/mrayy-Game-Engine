

/********************************************************************
	created:	2011/01/16
	created:	16:1:2011   21:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SoundEnums.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SoundEnums
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SoundEnums_h__
#define SoundEnums_h__


namespace mray
{
namespace sound
{

	enum ESoundType
	{
		ESNDT_2D,
		ESNDT_3D
	};

	enum ESampleFormat
	{
		ESF_8,
		ESF_16
	};
	enum EFFT_WindowType
	{
		FFT_WT_Rect,		/* w[n] = 1.0                                                                                            */
		FFT_WT_Triangle,	/* w[n] = TRI(2n/N)                                                                                      */
		FFT_WT_Hamming,		/* w[n] = 0.54 - (0.46 * COS(n/N) )                                                                      */
		FFT_WT_Hanning,		/* w[n] = 0.5 *  (1.0  - COS(n/N) )                                                                      */
		FFT_WT_Blackman,	/* w[n] = 0.42 - (0.5  * COS(n/N) ) + (0.08 * COS(2.0 * n/N) )                                           */
		FFT_WT_BlackmanHarris	/* w[n] = 0.35875 - (0.48829 * COS(1.0 * n/N)) + (0.14128 * COS(2.0 * n/N)) - (0.01168 * COS(3.0 * n/N)) */
	};
}
}
#endif // SoundEnums_h__

