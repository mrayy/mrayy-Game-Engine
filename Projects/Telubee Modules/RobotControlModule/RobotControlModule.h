// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ROBOTCONTROLMODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ROBOTCONTROLMODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ROBOTCONTROLMODULE_EXPORTS
#define ROBOTCONTROLMODULE_API __declspec(dllexport)
#else
#define ROBOTCONTROLMODULE_API __declspec(dllimport)
#endif

// This class is exported from the RobotControlModule.dll
class ROBOTCONTROLMODULE_API CRobotControlModule {
public:
	CRobotControlModule(void);
	// TODO: add your methods here.
};

extern ROBOTCONTROLMODULE_API int nRobotControlModule;

ROBOTCONTROLMODULE_API int fnRobotControlModule(void);
