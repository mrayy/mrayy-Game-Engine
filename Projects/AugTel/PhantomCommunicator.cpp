

#include "stdafx.h"
#include "PhantomCommunicator.h"
#include <windows.h>

extern "C" {
#include "hidsdi.h"
#include <setupapi.h>
}

namespace mray
{
class PhantomCommunicatorImpl
{
	//PCTx/Servo Controller USB device info
	static const unsigned int VendorID = 0x0925;
	static const unsigned int ProductID = 0x1299;

protected:

	HANDLE								DeviceHandle;
	HIDP_CAPS							Capabilities;
	BOOL								result;
	CHAR								OutputReport[20]; //Holds the data to be sent to the PCTx or Servo Controller
	DWORD								BytesWritten;
	DWORD								BytesRead;

	bool threadStart;

	HANDLE m_baseThread;
	bool isDone;

	float m_throttle; // controls up and down speed
	float m_pan;	  // controls the orientation of the phantom speed
	float m_side;	  // controls the side (left, right) speed 
	float m_front;	  // controls front/back motion speed
	float m_lastRotation;
public:

	ITelubeeRobotListener* listener;

public:
	PhantomCommunicatorImpl()
	{
		DeviceHandle = 0;
		listener = 0;
		isDone = false;
		m_baseThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadBase, this, NULL, NULL);

		m_throttle = 0;
		m_pan = 0;
		m_side = 0;
		m_front = 0;
		m_lastRotation = 0;
	}
	~PhantomCommunicatorImpl()
	{
		Disconnect();
		isDone = true;
		Sleep(100);
		TerminateThread(m_baseThread, 0);
	}

	bool SendControl(int V1, int V2, int V3, int V4)
	{
		int V5 = 512;
		int V6 = 1023;
		int V7 = 63;
		int V8 = 512;
		int V9 = 512;

		return Send(V1 % 256, V1 / 256, V2 % 256, V2 / 256, V3 % 256, V3 / 256, V4 % 256, V4 / 256,
			V5 % 256, V5 / 256, V6 % 256, V6 / 256, V7 % 256, V7 / 256, V8 % 256, V8 / 256, V9 % 256, V9 / 256);

	}

	bool Send(int delay1, int mul1, int delay2, int mul2, int delay3, int mul3, int delay4, int mul4, int delay5, int mul5, int delay6, int mul6,
		int delay7, int mul7, int delay8, int mul8, int delay9, int mul9)
	{
		if (DeviceHandle == 0)
			return false;
		OutputReport[0] = 0;  //do not remove, must be 0

		OutputReport[1] = delay1; //ch1
		OutputReport[2] = mul1;
		OutputReport[3] = delay2; //ch2
		OutputReport[4] = mul2;
		OutputReport[5] = delay3; //ch3
		OutputReport[6] = mul3;
		OutputReport[7] = delay4; //ch4
		OutputReport[8] = mul4;
		OutputReport[9] = delay5; //ch5
		OutputReport[10] = mul5;
		OutputReport[11] = delay6; //ch6
		OutputReport[12] = mul6;
		OutputReport[13] = delay7; //ch7
		OutputReport[14] = mul7;
		OutputReport[15] = delay8; //ch8
		OutputReport[16] = mul8;
		OutputReport[17] = delay9; //ch9
		OutputReport[18] = mul9;

		if (!WriteFile(DeviceHandle, OutputReport, Capabilities.OutputReportByteLength, &BytesWritten, NULL)) {
			CloseHandle(DeviceHandle);
			DeviceHandle = 0;
			return false;
		}

		return true;
	}
	bool Connect()
	{
		Disconnect();
		::GUID								HidGuid;
		HANDLE								hDevInfo;
		SP_DEVICE_INTERFACE_DATA			devInfoData;
		int									MemberIndex = 0;
		bool								MyDeviceDetected = FALSE;
		bool								LastDevice = FALSE;
		ULONG								Length;
		PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
		ULONG								Required;
		HIDD_ATTRIBUTES						Attributes;
		PHIDP_PREPARSED_DATA				PreparsedData;

		// Get the GUID for all system HIDs
		HidD_GetHidGuid(&HidGuid);

		// Get handle to device information set for all installed devices
		hDevInfo = SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

		// Set struct size before calling SetupDiEnumDeviceInterfaces
		devInfoData.cbSize = sizeof(devInfoData);

		do {
			MyDeviceDetected = FALSE;

			// Get handle to a SP_DEVICE_INTERFACE_DATA structure for a detected device
			result = SetupDiEnumDeviceInterfaces(hDevInfo, 0, &HidGuid, MemberIndex, &devInfoData);

			if (result) {
				result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &Length, NULL);

				//Allocate memory for the hDevInfo structure, using the returned Length.
				detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);

				//Set cbSize in the detailData structure.
				detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

				//Call the function again, this time passing it the returned buffer size.
				result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detailData, Length, &Required, NULL);

				DeviceHandle = CreateFile
					(detailData->DevicePath,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					0,
					NULL);

				Attributes.Size = sizeof(Attributes);

				result = HidD_GetAttributes(DeviceHandle, &Attributes);

				if (Attributes.VendorID == VendorID) {
					if (Attributes.ProductID == ProductID) {
						//Both the Product and Vendor IDs match.
						MyDeviceDetected = TRUE;
					}
					else {
						CloseHandle(DeviceHandle);
					}
				}
				else {
					CloseHandle(DeviceHandle);
				}
				//Free the memory used by the detailData structure (no longer needed).
				free(detailData);
			}
			else {
				//SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
				LastDevice = TRUE;
			}
			//If we haven't found the device yet, and haven't tried every available device,
			//try the next one.
			MemberIndex++;
		} while ((LastDevice == FALSE) && (MyDeviceDetected == FALSE));

		if (MyDeviceDetected == FALSE) {
			SetupDiDestroyDeviceInfoList(hDevInfo);
			DeviceHandle = 0;
			printf(" Failed to connect to Phantom controller. \n");
			return false;
		}

		//Free the memory reserved for hDevInfo by SetupDiClassDevs.
		SetupDiDestroyDeviceInfoList(hDevInfo);

		HidD_GetPreparsedData(DeviceHandle, &PreparsedData);
		HidP_GetCaps(PreparsedData, &Capabilities);

		HidD_FreePreparsedData(PreparsedData);

		printf("Phantom controller connected successfully. \n");

		SendControl(1023, 1023, 1023, 0);
		Sleep(3000);
		
		return true;
	}
	bool IsConnected()
	{
		return DeviceHandle!=0;
	}
	void Disconnect()
	{
		SendControl(512, 512, 1023, 512);
		if (DeviceHandle!=0)
			CloseHandle(DeviceHandle);
		DeviceHandle = 0;
		printf("Phantom controller disconnected. \n");
	}

	bool TimerUpdate()
	{
		if (isDone)
			return false;

		if (threadStart){
			float pan = m_pan*3 ;
			int V1 = math::clamp<float>(m_side * 512+512, 0, 1023);
			int V2 = math::clamp<float>(m_front * 512 + 512, 0, 1023);
			int V3 = math::clamp<float>(m_throttle * 512 + 512, 0, 1023);
			int V4 = math::clamp<float>(pan * 512 + 512, 0, 1023);

			int V5 = 512;
			int V6 = 1023;
			int V7 = 63;
			int V8 = 512;
			int V9 = 512;

			//printf("S1:%f,F1:%f,T1:%f,P1:%f\n", m_side, m_front, m_throttle, pan);
			//printf("S:%d,F:%d,T:%d,P:%d\n", V1, V2, V3, V4);
			
			Send(V1 % 256, V1 / 256, V2 % 256, V2 / 256, V3 % 256, V3 / 256, V4 % 256, V4 / 256, 
				 V5 % 256, V5 / 256, V6 % 256, V6 / 256, V7 % 256, V7 / 256, V8 % 256, V8 / 256, V9 % 256, V9 / 256);
			//robot->omni_control(-robot->robot_vx, -robot->robot_vy, -robot->robot_rot, robot->baseConnected ? RUN : STOP);
			//printf("thread b: %d \r", count++);
			//robot->yamahaXY_control(robot->robotX, robot->robotY, RUN);
		}

		Sleep(30);
		return true;
	}

	static DWORD timerThreadBase(PhantomCommunicatorImpl *robot, LPVOID pdata){
		int count = 0;
		while (robot->TimerUpdate()){
		}
		return 0;
	}

	void OnUpdate(const RobotStatus& st)
	{
		if (IsConnected() && !st.connected)
		{
			Disconnect();
		}
		else if (!IsConnected() && st.connected)
		{
			Connect();
		}
		if (st.connected)
			threadStart = true;
		else
			threadStart = false;

		if (!IsConnected())
			return;

		m_pan = st.rotation ;
		m_front = st.speed[0];
		m_side = -st.speed[1];

	}
};
}
PhantomCommunicator::PhantomCommunicator()
{
	m_impl = new mray::PhantomCommunicatorImpl();
}
PhantomCommunicator::~PhantomCommunicator()
{
	delete m_impl;
}

void PhantomCommunicator::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}
void PhantomCommunicator::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{

}
void PhantomCommunicator::ConnectRobot()
{
	m_impl->Connect();
}
void PhantomCommunicator::DisconnectRobot()
{
	m_impl->Disconnect();

}
bool PhantomCommunicator::IsConnected()
{
	return m_impl->IsConnected();
}
void PhantomCommunicator::UpdateRobotStatus(const RobotStatus& st)
{

	m_impl->OnUpdate(st);


}

std::string PhantomCommunicator::ExecCommand(const std::string& cmd, const std::string& args)
{
	return "";
}