#ifndef _capDeviceInput
#define _capDeviceInput

/////////////////////////////////////////////////////////

#pragma comment(lib,"Strmiids.lib") 

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <string>
#include <vector>

//this is for TryEnterCriticalSection
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x501
#endif
#include <windows.h>

#define VI_VERSION	 0.200
#define VI_MAX_CAMERAS  20
#define VI_NUM_TYPES    19 //DON'T TOUCH
#define VI_NUM_FORMATS  18 //DON'T TOUCH

//allows us to directShow classes here with the includes in the cpp
struct ICaptureGraphBuilder2;
struct IGraphBuilder;
struct IBaseFilter;
struct IAMCrossbar;
struct IMediaControl;
struct ISampleGrabber;
struct IMediaEventEx;
struct IAMStreamConfig;
struct _AMMediaType;
class SampleGrabberCallback;
typedef _AMMediaType AM_MEDIA_TYPE;

//keeps track of how many instances of VI are being used
//don't touch
static int comInitCount = 0;


////////////////////////////////////////   VIDEO DEVICE   ///////////////////////////////////

class capDevice{


public:

	capDevice();
	void setSize(int w, int h);
	void NukeDownstream(IBaseFilter *pBF);
	void destroyGraph();
	~capDevice();


	ICaptureGraphBuilder2 *pCaptureGraph;	// Capture graph builder object
	IGraphBuilder *pGraph;					// Graph builder object
	IMediaControl *pControl;				// Media control object
	IBaseFilter *pcapDeviceInputFilter;  		// Video Capture filter
	IBaseFilter *pGrabberF;
	IBaseFilter * pDestFilter;
	IAMStreamConfig *streamConf;
	ISampleGrabber * pGrabber;    			// Grabs frame
	AM_MEDIA_TYPE * pAmMediaType;

	IMediaEventEx * pMediaEvent;

	GUID videoType;
	long formatType;

	SampleGrabberCallback * sgCallback;

	bool tryDiffSize;
	bool useCrossbar;
	bool readyToCapture;
	bool sizeSet;
	bool setupStarted;
	bool specificFormat;
	bool autoReconnect;
	int  nFramesForReconnect;
	unsigned long nFramesRunning;
	int  connection;
	int	 storeConn;
	int  myID;
	long requestedFrameTime; //ie fps

	char 	nDeviceName[255];
	WCHAR 	wDeviceName[255];


};




//////////////////////////////////////   VIDEO INPUT   /////////////////////////////////////



class capDeviceInput{

public:
	capDeviceInput();
	~capDeviceInput();

	//turns off console messages - default is to print messages
	static void setVerbose(bool _verbose);

	//this allows for multithreaded use of VI ( default is single threaded ).
	//call this before any capDeviceInput calls. 
	//note if your app has other COM calls then you should set VIs COM usage to match the other COM mode 
	static void setComMultiThreaded(bool bMulti);

	//Functions in rough order they should be used.
	static int listDevices(bool silent = false);
	static std::vector <std::string> getDeviceList();

	//needs to be called after listDevices - otherwise returns NULL
	static const char * getDeviceName(int deviceID);
	static int getDeviceIDFromName(const char * name);


	//Choose one of these four to setup your device
	bool setupDevice(int deviceID);

	bool isDeviceSetup(int deviceID);

	//Launches a pop up settings window
	//For some reason in GLUT you have to call it twice each time.
	void showSettingsWindow(int deviceID);

	//Manual control over settings thanks.....
	//These are experimental for now.
	bool setVideoSettingFilter(int deviceID, long Property, long lValue, long Flags = NULL, bool useDefaultValue = false);
	bool setVideoSettingFilterPct(int deviceID, long Property, float pctValue, long Flags = NULL);
	bool getVideoSettingFilter(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue);

	bool setVideoSettingCamera(int deviceID, long Property, long lValue, long Flags = NULL, bool useDefaultValue = false);
	bool setVideoSettingCameraPct(int deviceID, long Property, float pctValue, long Flags = NULL);
	bool getVideoSettingCamera(int deviceID, long Property, long &min, long &max, long &SteppingDelta, long &currentValue, long &flags, long &defaultValue);

	//completely stops and frees a device
	void stopDevice(int deviceID);

	//as above but then sets it up with same settings
	bool restartDevice(int deviceID);

	//number of devices available
	int  devicesFound;

	long propBrightness;
	long propContrast;
	long propHue;
	long propSaturation;
	long propSharpness;
	long propGamma;
	long propColorEnable;
	long propWhiteBalance;
	long propBacklightCompensation;
	long propGain;

	long propPan;
	long propTilt;
	long propRoll;
	long propZoom;
	long propExposure;
	long propIris;
	long propFocus;

private:

	void setPhyCon(int deviceID, int conn);
	bool setup(int deviceID);
	int  start(int deviceID, capDevice * VD);
	int  getDeviceCount();
	void getMediaSubtypeAsString(GUID type, char * typeAsString);

	HRESULT getDevice(IBaseFilter **pSrcFilter, int deviceID, WCHAR * wDeviceName, char * nDeviceName);
	static HRESULT ShowFilterPropertyPages(IBaseFilter *pFilter);
	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);
	HRESULT routeCrossbar(ICaptureGraphBuilder2 **ppBuild, IBaseFilter **pVidInFilter, int conType, GUID captureMode);

	//don't touch
	static bool comInit();
	static bool comUnInit();

	int  connection;
	int  callbackSetCount;
	bool bCallback;

	GUID CAPTURE_MODE;
	GUID requestedMediaSubType;

	//Extra video subtypes
	GUID MEDIASUBTYPE_Y800;
	GUID MEDIASUBTYPE_Y8;
	GUID MEDIASUBTYPE_GREY;

	capDevice * VDList[VI_MAX_CAMERAS];

	static void __cdecl basicThread(void * objPtr);

	static char deviceNames[VI_MAX_CAMERAS][255];

};

#endif