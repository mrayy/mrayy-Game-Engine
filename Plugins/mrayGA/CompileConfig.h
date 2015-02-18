
#if 0
#ifdef MRAYGA_EXPORTS
#define MRAYGA_API __declspec(dllexport)
#else
#define MRAYGA_API __declspec(dllimport)
#endif
#else
#define MRAYGA_API 
#endif