#ifndef __torsoController__
#define __torsoController__

#include <string>
#include "IRobotController.h"
#include "TexARTNCord\TexART_NCord000_Interface.h"
#include "console.h"
#include "DummyClass ADBoard.h"
#include "TexARTNCord\TexART_ServoMotor2015.h"

#include "movingAverage.h"
#include "RobotCapabilities.h"

class torsoControllerImpl;
class torsoController :public IRobotController
{
protected:

	char cCurrentPath[256];
	double targetRotMat[16];

	MovAvg *m_posAvg[3];
	MovAvg *m_rotAvg[3];
	
	torsoControllerImpl* m_impl;
	HANDLE hThread;
	ERobotControllerStatus robotState; 
	mray::TBee::RobotCapabilities m_caps;
	float torsoHeadPos[3];
	float torsoHeadOri[3];
	float robotJointData[12];
	bool torsoInitializatiion = false; 
	bool torsoInitialized = false; 

	IRobotStatusProvider* m_robotStatusProvider;

	static DWORD WINAPI timerThread1(torsoController *robot, LPVOID pdata);
	static DWORD WINAPI timerThread2(torsoController *robot, LPVOID pdata);

	std::string ScanePorts();

	void _setupCaps();
	void _processData();
public:

	TexART_NCord000_Interface  *Torso_02;   // Interface object for the machine hardware.
	TexART_ServoMotor2015      *ServoMotor; // Interface object for each servo motor.

	int ControlMode = 0,				    
		// 0: Keyboard JointSpace Simulate
		// 1: Keyboard CoordinateSpace Simulate
		// 2: fade connect
		// 3: realtime mode
		// 4: fade disconnect

		LoopCount = 0,
		Display = 0,                   // Display data on the console or not (0 or 1).
		ret;
	
	bool manualMode = false; 

	double DesiredCoordinate_Head[6],       // Head 6D coordinate.                        [rad or m]
		StepCoordinate_Head[6],          // Control step of Head 6D coordinate.           [rad or m]
		GearRatio_J[6],                  // Gear ratio of each joint.
		DesiredDisplacement_J[6],        // Angular or Linear displacement of each joint. [rad or m]
		LimitDisplacement_p_J[6],        // Positive limit of Joint Displacement.         [rad or m]
		LimitDisplacement_n_J[6],        // Negative limit of Joint Displacement.         [rad or m]
		StepDisplacement_J[6],           // Control step in each joint displacement.      [rad or m]
		AngularDisplacement_start_M[6],  // Status of motor starting.                     [rad]
		AngularDisplacement_goal_M[6],   // Status of motor goal.                         [rad]
		Time_start,                      // Status of motor starting.
		gamma = 0.0;					// Virtual turning displacement.                 [rad]
	int    quadrant = 0;                    // Code presenting quadrant of J6 rotaiton.

	TexART_MothorCLK MothorCLK;


	//torsoController class variables
	torsoController();
	virtual~torsoController();

	virtual void InitializeRobot(IRobotStatusProvider* robotStatusProvider) ;
	void SetListener(ITelubeeRobotListener* l);
	int InitializeTorsoRobot(bool debug);
	int ReInitializeTorsoRobot(void);
	void ConnectRobot();
	void ManualControlRobot(); 
	void DisconnectRobot();
	//bool IsConnected();
	void UpdateRobotStatus(const RobotStatus& st);

	ERobotControllerStatus GetRobotStatus();
	void ShutdownRobot();
	bool GetJointValues(std::vector<float>& values);
	virtual const mray::TBee::RobotCapabilities* GetRobotCaps()const { return &m_caps; }


	virtual std::string ExecCommand(const std::string& cmd, const std::string& args){ return ""; }

	double CalcJointDisplacement(TexART_ServoMotor2015 *ServoMotor, double LimitDisplacement_p_J, double LimitDisplacement_n_J); 
	double CalcMotorDisplacement(TexART_ServoMotor2015 *ServoMotor, double JointDisplacement, double LimitDisplacement_p_J, double LimitDisplacement_n_J);
	void AdaptComplexLimitation(double *AngularDisplacement_1, double *AngularDisplacement_2, double AngularDisplacement_limit); 
	
	int printRealtimeInfo(); 
	int controlStateMachine(); 
	int motorSafeShutdown();
	int motorForceShutdown();

	void InverseKinematicsTORSO_RPY(
		double G_X,           // X-coordinate of point Glabella [m]
		double G_Y,           // Y-coordinate of point Glabella [m]
		double G_Z,           // Z-Coordinate of point Glabella [m]
		double Roll,          // Roll  of head                  [rad]
		double Pitch,         // Pitch of head                  [rad]
		double Yaw,           // Yaw   of head                  [rad]
		int    *quadrant,     // Code presenting quadrant of J6 rotaiton
		double gamma,         // Virtual turning displacement   [rad]
		double *Theta_1,      // AngularDisplacement of J1      [rad]
		double *Theta_2,      // AngularDisplacement of J2      [rad]
		double *d_3,          // LinearDisplacement  of J3      [m]
		double *Theta_4,      // AngularDisplacement of J4      [rad]
		double *Theta_5,      // AngularDisplacement of J5      [rad]
		double *Theta_6);      // AngularDisplacement of J6      [rad]
};


#endif
