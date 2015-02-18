

#ifndef ___IWaveLoader___
#define ___IWaveLoader___

#include <ISoundLoader.h>

#include "compileConfig.h"
namespace mray{
namespace loaders{


class MRAY_SOUND_DLL IWaveLoader:public ISoundLoader
{
	struct __declspec(align(1)) SWAVHeader 
   {  
        unsigned char    RIFF[4];
        unsigned long    Size;            // forget this                       
        unsigned char    WAVE[4];
        unsigned char    FMT[4];
        unsigned long    SizeFmt;
        unsigned short   FormatTag;       // 1=PCM     
        unsigned short   Channels;        // 1=Mono, 2=Stereo                   
        unsigned long    SampleRate;
        unsigned long    AvgBytesPerSec;  // usually same as Sp.Rate            
        unsigned short   nBlockAlign;     // too lazy to look it up :(         
        unsigned short   nBitsperSample;  // 8 or 16
        unsigned char    dat_[4];         // forget this     
        unsigned long    DataSize;        // size of the sample data
   };
public:

	virtual sound::ISoundStreamFrame* load(const mchar*filename);
	virtual bool canLoad(const mchar*filename);

};
};//loaders
};//mray


#endif