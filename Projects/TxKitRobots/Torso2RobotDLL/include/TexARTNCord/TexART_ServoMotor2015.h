/*=============================================================================

                  TexART utility calss [TexART_ServoMotor2015]

                        file name : TexART_ServoMotor2015.h
                     first author : I.Kawabuchi(info@kawabuchi-lab.com)
            prototype publication : 2002.10/8(Tue)
                      version 1.1 : 2003.8/30(Mon)
                      version 1.2 : 2005.4/5(Tue)
                      version 1.3 : 2012.8/10(Fri) // Start adaptation to TexART_NCode system

=============================================================================*/
#ifndef ___TexART_ServoMotor2015
#define ___TexART_ServoMotor2015


#define WIN32_LEAN_AND_MEAN


#include <windows.h> // for GetAsyncKeyState
#include <stdio.h>
#include <iostream> // Substituted for <iostream.h>.(2004/09/02)
#include <fstream>  // Substituted for <fstream.h>.(2004/09/02)

using namespace std;

#include "TexART_NCord000_Interface.h"
#include "DummyClass ADBoard.H"
#include "TexART_MothorCLK.h"
#include "LoadData.h"

#ifndef ___PI
  #define ___PI
  const double PI = 3.141592654;
  const double pi = 3.141592654;
#endif

///-----[TexART_ServoMotor2015]-------------------------------------------------
class TexART_ServoMotor2015 {
public:
  int    IDNumber;                            // Number for Identification that is defined by user's need. No affect ot the program.

  double AngularDisplacement;                 // Angular displacement the servo-motor is producing. [rad]
  double AngularVelocity;                     // Angular velocity the servo-motor is producing. [rad/s]
  double AngularAcceleration;                 // Angular acceleration the servo-motor is producing. [rad/s^2]
  double Torque;                              // Total torque the servo-motor should produce. [Nm]

protected:
  // Motor affairs.
  double SpeedConstant;                       // A characteristic of the servo-motor. [rad/(s*V)] (necessary when AmpType = 0)
  double TorqueConstant;                      // A characteristic of the servo-motor. [Nm/A]      (necessary when AmpType = 0,1,10)
  double TerminalResistance;                  // A characteristic of the servo-motor. [Ohm]       (necessary when AmpType = 0)

  // Driver amplifire and auxiliaries affairs.
  int    AmpType;                             /* Types of the amplifier.
                                                  0: Analog Voltage control
                                                  1: Analog Current control
                                                  2: Analog Torque control
                                                 10: TexART_NCord system = Digital current control */

  double AmpFactor;                           // Coefficient of the amplification between command and output. [A/V] or [A/CommandDigital] etc.
  double AmpOffset;                           /* Offset CommandVoltage or CommandDigital at setting the output 0. [V] or [CommandDigital]
                                                 When it is NCord Amp, this is 0x8000. */
  int    DirectionMatching_motor;             /* +1: Command_Voltage is posotive, when the joint rotates in posotive direction.
                                                 -1: Command_Voltage is negative, when the joint rotates in posotive direction. */

  int    AuxType;                             /* Type of the configuration of auxiliaries for PC interface.
                                                   No. | CNT board | AD board | NCord CNT | NCord AD
                                                    0:       -           -          -          -
                                                    1:     -DVA          -          -          -
                                                    2:       -         CDVA         -          -
                                                    3:     -DVA        C---         -          -
                                                    4:     --VA        CD--         -          -
                                                    5:       -           -        -DVA         -
                                                    6:       -           -          -        CDVA
                                                    7:       -           -        -DVA       C---
                                                    8:       -           -        --VA       CD--
                                                    9:     -DVA          -          -        C--- *When NCord board contains no CNT*
                                                   10:     --VA          -          -        CD-- *When NCord board contains no CNT*
                                                 Code of controll type.
                                                    C:Calibration of starting displacement
                                                    D:Displacement ctrl.
                                                    V:Velocity ctrl.
                                                    A:Acceleration ctrl.                                   */

  // Encoder affairs
  double Resolution;                          // Encoder resolution. [pulse/revolution]
  double ResolutionMulti;                     // Multiplication coefficient of the counter. Selected among 1,2 or 4.
  double SyntheticResolution_encoder;         // = ResolutionMulti * Resolution / (2.0 * PI); [count/rad]
  int    DirectionMatching_encoder;           /* +1: EncoderCount increases, when the joint rotates in posotive direction.
                                                 -1: EncoderCount idcreases, when the joint rotates in posotive direction. */
  unsigned long CounterCapacity;              // Capacity of a resister on the counter.
  unsigned long CountMedian;                  // Given automatically as (CounterCapacity / 2).

  unsigned long EncoderCount;                 // Values of the counter. [count]
  double AngularDisplacement_encoder;         // Angular displacemtnt of the motor axis measured by the encoder. [rad]
  double AngularDisplacement_encoder_offset;  // Angular displacemtnt when the EncoderCount is median of the CounterCapacity. [rad]

  // Potentio affairs
  double SyntheticResolution_potentio;        /* = (DeadendPotentioValue_p - DeadendPotentioValue_n)
                                                 / (DeadendAngularDisplacement_p - DeadendAngularDisplacement_n); [Value/rad]
                                                 [Note] Negative value is possible as alternated to DirectionMatching_poteentio. */
  double PotentioValue;                       // Values of the potentio. [AnalogVoltage or DigitalValue]
  double AngularDisplacement_potentio;        // Angular displacemtnt of the motor axis measured by the potentio. [rad]
  double AngularDisplacement_potentio_offset; // Angular displacemtnt when PotentioValue is 0. [rad]

  // Gain affairs.
  double PGain_D;                             // Proportional gain on the displacement control system. [Nm/rad]
  double PGain_V;                             // Proportional gain on the velocity control system. [Nm*s/rad]
  double IGain_D;                             // Integral gain on the displacement control system.
  double IGain_V;                             // Integral gain on the velocity control system.
  double DGain_D;                             // Differential gain on the displacement control system. [Nm*s/rad]
  double DGain_V;                             // Differential gain on the velocity control system. [Nm*s^2/rad]

  double PGain_D_backup;                      // Refuge used when canceled temporarily the gain is.
  double PGain_V_backup;
  double IGain_D_backup;
  double IGain_V_backup;
  double DGain_D_backup;
  double DGain_V_backup;

  // Command affairs.
  double Reference_D;                         // Referential angular displacent for the displacement control system. [rad]
  double Reference_V;                         // Referential angular velocity for the velocity control system. [rad/s]
  double Reference_T;                         // Referential torque for the torque control system. [Nm]
  double CommandVoltage;                      // Voltage given to the setvo-amplifier. [V]
  double LimitCommandVoltage_p;               // Positive limit of available CommandVoltage. [V]
  double LimitCommandVoltage_n;               // Negative limit of available CommandVoltage. [V]
  double CommandDigital;                      // Digital command given to the setvo-amplifier. [DRV_Command]
  double LimitCommandDigital_p;               // Positive limit of available CommandDigital. [DRV_Command]
  double LimitCommandDigital_n;               // Negative limit of available CommandDigital. [DRV_Command]

  // Limitter affairs.
  double LimitTorque;                         // Limit of available torque.[Nm]
  double LimitAngularVelocity;                // Limit of available velocity.[rad/s]
  double LimitAngularDisplacement_p;          // Positive limit of available rotational range.[rad]
  double LimitAngularDisplacement_n;          // Negative limit of available rotational range.[rad]
  double LimitAngularDisplacement_p_backup;   // Refuge used when unlimited temporarily the available rotational range is.
  double LimitAngularDisplacement_n_backup;

  double DeadendAngularDisplacement_p;        // Positive dead end of rotational range.[rad]
  double DeadendAngularDisplacement_n;        // Negative dead end of rotational range.[rad]
  double DeadendPotentioValue_p;              // Value of the potentio at DeadendAngularDisplacement_p. [AnalogVoltage or DigitalValue]
  double DeadendPotentioValue_n;              // Value of the potentio at DeadendAngularDisplacement_n. [AnalogVoltage or DigitalValue]

  // Device handle affairs.
  Class_DABoard *DADeviceHandle;              // Handle of the D/A board.
  int    DAChannel;                           // Number of the top channel is 0.
  Class_CounterBoard *CNTDeviceHandle;        // Handle of the counter board.
  int    CNTChannel;                          // Number of the top channel is 0.
  Class_ADBoard *ADDeviceHandle;              // Handle of the A/D board.
  int    ADChannel;                           // Number of the top channel is 0.

  TexART_NCord000_Interface *NCordHandle;     // Handle of the whole TexART_NCord system.
  int    TerminalNumber_M;                    // Number of the terminal controller connecting to Motor.
  int    TerminalNumber_E;                    // Number of the terminal controller connecting to Encorder.
  int    TerminalNumber_P;                    // Number of the terminal controller connecting to Potentio.
  int    MotorDRVNumber;                      // Motor Driver number in the terminal.
  int    EncoderCNTNumber;                    // Encorder Counter number in the terminal.
  int    PotentioADNumber;                    // Potentio AD converter number in the terminal.

  // Buffer affairs.
  double *AngularDisplacement_encoder_RingBuffer; // Ring buffer that saves motion log in each turn.
  double *AngularVelocity_encoder_RingBuffer;
  double *AngularAcceleration_encoder_RingBuffer;
  double *AngularDisplacement_potentio_RingBuffer;
  double *AngularVelocity_potentio_RingBuffer;
  double *AngularAcceleration_potentio_RingBuffer;
  double Time_last;                           // MothorCLK.Erapsed() at last turn.
  int    SampleNumber;                        // Number of saved values in the ring buffer.
  int    PresentNumber;                       // Number of a cell that saves the present value.
  double ErrorSum_D;                          // Accumulation of error in angular displacement from the starting. [rad]
  double ErrorSum_V;                          // Accumulation of error in angular velocity from the starting. [rad/s]
  double LimitErrorSum_D;                     // Limit of ErrorSum_D. [rad]
  double LimitErrorSum_V;                     // Limit of ErrorSum_V. [rad/s]

  // MothorCLK affairs.
  TexART_MothorCLK MothorCLK;                 // Class MothorCLK that manages the standard time.

  // Versatile parameters.
  double VersatileParameter_01;
  double VersatileParameter_02;
  double VersatileParameter_03;

public:
  // Default constructor.
  TexART_ServoMotor2015(void);

  // Destructor.
 // ~TexART_ServoMotor2015();

  // Initialize.
  void Initialize(ifstream&                 ParameterFile,
                  Class_DABoard             *_DADeviceHandle  = 0,
                  Class_CounterBoard        *_CNTDeviceHandle = 0,
                  Class_ADBoard             *_ADDeviceHandle  = 0,
                  TexART_NCord000_Interface *_NCordHandle     = 0);

  // Overload another parameter file.
  void OverloadProperty(ifstream& ParameterFile);

  // Set value of AngularDisplacement for the present position.
  void SetAngularDisplacement(double _AngularDisplacement);

  // Calibration AngularDisplacement automalically by the potentio.
  void CaliAngularDisplacementByPotentio(void);

  // Reset all references to stop and ready.
  void ResetReferences(void);

  // Cut the motor torque.
  void TorqueCut(void);

  // Release the motor torque and prepare to restart.
  void Free(void);

  // Set each reference value.
  void SetReference_D(double _Reference_D);
  void SetReference_V(double _Reference_V);
  void SetReference_T(double _Reference_T);

  // Calculate commands of PID control.
  void CalcPIDControl(void);

  // Execute servo.
  // [Note] This is unrecommendable for usage by a consumer.
  //        This remains to be embedded in only several utility functions;
  //        GoDeadEndPosition(), SearchDeadendPotentioValueAuto(), GoNextPositionOnTime() and Fix().
  void ExecuteServo(void);

  // Change each gain on demand.
  void ChangePGain_D(double _PGain_D);
  void ChangePGain_V(double _PGain_V);
  void ChangeIGain_D(double _IGain_D);
  void ChangeIGain_V(double _IGain_V);
  void ChangeDGain_D(double _DGain_D);
  void ChangeDGain_V(double _DGain_V);

  // Zero Gain_D, Gain_V and LimitAngularDisplacement temporarily.
  void ZeroPGain_D(void);
  void ZeroPGain_V(void);
  void ZeroIGain_D(void);
  void ZeroIGain_V(void);
  void ZeroDGain_D(void);
  void ZeroDGain_V(void);
  void ZeroGain_D(void);
  void ZeroGain_V(void);
  void ZeroLimitAngularDisplacement(void); // Mode of no limitation by AngularDisplacement.

  // Reload Gain_D, Gain_V and LimitAngularDisplacement.
  void ReloadGain_D(void);
  void ReloadGain_V(void);
  void ReloadLimitAngularDisplacement(void);

  // Change limit parameters.
  void ChangeLimitAngularDisplacement_p(double _LimitAngularDisplacement_p);
  void ChangeLimitAngularDisplacement_n(double _LimitAngularDisplacement_n);
  void ChangeLimitAngularVelocity      (double _LimitAngularVelocity);
  void ChangeLimitTorque               (double _LimitTorque);

  // Clear all gains to 0 value.
  void ClearAllGain(void);

  // Clear ErrorSum to 0 value.
  void ClearErrorSum(void);

  // Clear versatile parameters to 0 value.
  void ClearVersatileParameters(void);


///-----[Utility Functions]----------------------------------------------------
  // Show private data.
  void ShowPrivateData(void);

  // Read out each gain.
  double ReadPGain_D(void);
  double ReadPGain_V(void);
  double ReadIGain_D(void);
  double ReadIGain_V(void);
  double ReadDGain_D(void);
  double ReadDGain_V(void);

  // Read out certain protected parameters.
  double ReadLimitAngularDisplacement_p  (void);
  double ReadLimitAngularDisplacement_n  (void);
  double ReadDeadendAngularDisplacement_p(void);
  double ReadDeadendAngularDisplacement_n(void);
  double ReadLimitAngularVelocity        (void);
  double ReadLimitTorque                 (void);
  double ReadReference_D                 (void);
  double ReadReference_V                 (void);
  double ReadReference_T                 (void);
  double ReadCommandVoltage              (void);
  double ReadCommandDigital              (void);
  double ReadAngularDisplacement_encoder (void);
  double ReadAngularDisplacement_potentio(void);
  unsigned long ReadEncoderCount         (void);
  double ReadPotentioValue               (void);

  // Search and set the DirectionMatching_encoder.
  void SearchDirectionMatching_encoder(void);

  // Search and set the DirectionMatching_motor.
  void SearchDirectionMatching_motor(double Torque_tmp);

  // Search and set the DeadendPotentioValue.
  void SearchDeadendPotentioValue(void);

  // Go to a dead-end position and reset AngularDisplacement_encoder.
  // [Note] This needs PGain_V.
  int GoDeadEndPosition(double AngularVelocity_tmp,
                        double StopAngularVelocity,
                        double StopTorque,
                        double AngularDisplacement_deadend,
                        double Time_start,
                        double Time_present);

  // Go to a dead-end position and show DeadendPotentioValue.
  // [Note] This needs PGain_V.
  int SearchDeadendPotentioValueAuto(double AngularVelocity_tmp,
                                     double StopAngularVelocity,
                                     double StopTorque,
                                     double Time_start,
                                     double Time_present);

  // Go to a next position during a given time in an uniform straigh-line movement.
  int GoNextPositionOnTime(double AngularDisplacement_start,
                           double AngularDisplacement_goal,
                           double TimeSpan,
                           double Time_start,
                           double Time_present);

  int GoNextPositionOnTime2(double AngularDisplacement_start,
                            double AngularDisplacement_goal,
                            double TimeSpan,
                            double Time_start,
                            double Time_present);

  // Go to a next position during a given time in a smooth velocity movement.
/* int GoNextPositionOnTimeSmooth(double AngularDisplacement_start,
                                double AngularDisplacement_goal,
                                double AngularVelocity_start,
                                double AngularVelocity_goal,
                                double Time_start,
                                double Time_present,
                                double TimeSpan,
                                double allowance);*/

  // Check the AngularDisplacement.
  double CheckAngularDisplacement(void);

  // Fix the angular displacement at a desirable stopping position.
  void Fix(void);
  void Fix(double Reference_D_tmp);
  void Fix2(double Reference_D_tmp);
};

#endif
