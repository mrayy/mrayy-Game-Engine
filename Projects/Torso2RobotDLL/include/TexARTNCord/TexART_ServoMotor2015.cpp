

/*=============================================================================

              Member functions of the class TexART_ServoMotor2015.

                        file name : TexART_ServoMotor2015.cpp
                     first author : I.Kawabuchi(info@kawabuchi-lab.comp)
            prototype publication : 2002.10/8(Tue)
                      version 1.1 : 2003.8/30(Mon)
                      version 1.2 : 2005.4/5(Tue)
                      version 1.3 : 2012.8/10(Fri) // Start adaptation to TexART_NCode system

=============================================================================*/
#include "stdafx.h"
#include "TexART_ServoMotor2015.h"



///-----[TexART_ServoMotor2015::TexART_ServoMotor2015]---------------------------
TexART_ServoMotor2015::TexART_ServoMotor2015(void){}

/*
///-----[TexART_ServoMotor2015::~TexART_ServoMotor2015]--------------------------
TexART_ServoMotor2015::~TexART_ServoMotor2015(){
//  delete[] AngularDisplacement_RingBuffer;
//  delete[] Time_RingBuffer;
}
*/
///-----[TexART_ServoMotor2015::Initialize]-------------------------------------
void TexART_ServoMotor2015::Initialize(
ifstream&                 ParameterFile,
Class_DABoard             *_DADeviceHandle,
Class_CounterBoard        *_CNTDeviceHandle,
Class_ADBoard             *_ADDeviceHandle,
TexART_NCord000_Interface *_NCordHandle)
{
  // Set default values into each parameter.
  IDNumber                     = 0;
  AngularDisplacement          = 0.0;
  AngularVelocity              = 0.0;
  AngularAcceleration          = 0.0;
  Torque                       = 0.0;

  SpeedConstant                = 1.0;
  TorqueConstant               = 1.0;
  TerminalResistance           = 1.0;
  AmpType                      = 0;
  AmpFactor                    = 1000.0;
  AmpOffset                    = 0.0;
  DirectionMatching_motor      = 1;
  AuxType                      = 0;

  Resolution                   = 1000;
  ResolutionMulti              = 1;
  DirectionMatching_encoder    = 1;
  CounterCapacity              = 0xFFFFF; // Default is 20bit counter.
  AngularDisplacement_encoder_offset  = 0.0;
  AngularDisplacement_potentio_offset = 0.0;

  PGain_D                      = 0.0;
  PGain_V                      = 0.0;
  IGain_D                      = 0.0;
  IGain_V                      = 0.0;
  DGain_D                      = 0.0;
  DGain_V                      = 0.0;
  Reference_D                  = 0.0;
  Reference_V                  = 0.0;
  Reference_T                  = 0.0;

  CommandVoltage               = 0.0;
  LimitCommandVoltage_p        = 10.0;
  LimitCommandVoltage_n        = 0.0;
  CommandDigital               = 0x0000; // Forced idling.
  LimitCommandDigital_p        = 0xffff; // Default is 16bit AMP.
  LimitCommandDigital_n        = 0x0001;

  LimitTorque                  = 0.0;
  LimitAngularVelocity         = 0.0;
  LimitAngularDisplacement_p   = 1.0;
  LimitAngularDisplacement_n   = 0.0;

  DeadendAngularDisplacement_p = 1.0;
  DeadendAngularDisplacement_n = 0.0;
  DeadendPotentioValue_p       = 0xffff;
  DeadendPotentioValue_n       = 0x0000;

  DAChannel                    = 0;
  CNTChannel                   = 0;
  ADChannel                    = 0;
  TerminalNumber_M             = 0;
  TerminalNumber_E             = 0;
  TerminalNumber_P             = 0;
  MotorDRVNumber               = 0;
  EncoderCNTNumber             = 0;
  PotentioADNumber             = 0;

  SampleNumber                 = 3;
  PresentNumber                = 0;
  ErrorSum_D                   = 0.0;
  ErrorSum_V                   = 0.0;
  LimitErrorSum_D              = 0.0;
  LimitErrorSum_V              = 0.0;

  VersatileParameter_01        = 0.0;
  VersatileParameter_02        = 0.0;
  VersatileParameter_03        = 0.0;

  // Overload values from the parameter file.
  OverloadProperty(ParameterFile);

  // Initialize the ring buffers.
  if (SampleNumber<3) SampleNumber = 3;
  AngularDisplacement_encoder_RingBuffer  = new double[SampleNumber];
  AngularVelocity_encoder_RingBuffer      = new double[SampleNumber];
  AngularAcceleration_encoder_RingBuffer  = new double[SampleNumber];
  AngularDisplacement_potentio_RingBuffer = new double[SampleNumber];
  AngularVelocity_potentio_RingBuffer     = new double[SampleNumber];
  AngularAcceleration_potentio_RingBuffer = new double[SampleNumber];
  for (int i=0; i<SampleNumber; i++){
    AngularDisplacement_encoder_RingBuffer[i]  = 0.0;
    AngularVelocity_encoder_RingBuffer[i]      = 0.0;
    AngularAcceleration_encoder_RingBuffer[i]  = 0.0;
    AngularDisplacement_potentio_RingBuffer[i] = 0.0;
    AngularVelocity_potentio_RingBuffer[i]     = 0.0;
    AngularAcceleration_potentio_RingBuffer[i] = 0.0;
  }

  // Initialize backups.
  PGain_D_backup = PGain_D;
  PGain_V_backup = PGain_V;
  IGain_D_backup = IGain_D;
  IGain_V_backup = IGain_V;
  DGain_D_backup = DGain_D;
  DGain_V_backup = DGain_V;
  LimitAngularDisplacement_p_backup = LimitAngularDisplacement_p;
  LimitAngularDisplacement_n_backup = LimitAngularDisplacement_n;

  // Set device handles.
  DADeviceHandle  = _DADeviceHandle;
  CNTDeviceHandle = _CNTDeviceHandle;
  ADDeviceHandle  = _ADDeviceHandle;
  NCordHandle     = _NCordHandle;

  // Initialize AMP condition.
  if(DADeviceHandle != 0)
    DADeviceHandle->DAOut(DAChannel, 0.0 + AmpOffset);
  if(NCordHandle != 0){
    NCordHandle->Terminal[TerminalNumber_M].Drv_Command[MotorDRVNumber] = 0x0000; // Froced idling.
    NCordHandle->Contact();
  }

  // Initialize the EncoderCount and the Counter board.
  EncoderCount = CountMedian = (unsigned long)(CounterCapacity / 2);
  if(CNTDeviceHandle != 0)
    CNTDeviceHandle->SetCount(CNTChannel, /*(DWORD)*/CountMedian);
  if(NCordHandle != 0)
    NCordHandle->ResetCounter(TerminalNumber_E, EncoderCNTNumber, CountMedian);

  // Start the MothorCLK.
  MothorCLK.Acquire_Frequency(); // Acquire and set the MothorCLK_Frequency.
  MothorCLK.Start();
}

///-----[TexART_ServoMotor2015::OverloadProperty]--------------------------------------
void TexART_ServoMotor2015::OverloadProperty(ifstream& ParameterFile)
{
  double double_IDNumber,
         double_AmpType,
         double_AuxType,
         double_DirectionMatching_motor,
         double_DirectionMatching_encoder,
         double_CounterCapacity,
         double_DAChannel,
         double_CNTChannel,
         double_ADChannel,
         double_TerminalNumber_M,
         double_TerminalNumber_E,
         double_TerminalNumber_P,
         double_MotorDRVNumber,
         double_EncoderCNTNumber,
         double_PotentioADNumber,
         double_SampleNumber;

  // Set default values into each parameter.
  double_IDNumber                   = (double)IDNumber;
  double_AmpType                    = (double)AmpType;
  double_AuxType                    = (double)AuxType;
  double_DirectionMatching_motor    = (double)DirectionMatching_motor;
  double_DirectionMatching_encoder  = (double)DirectionMatching_encoder;
  double_CounterCapacity            = (double)CounterCapacity;
  double_DAChannel                  = (double)DAChannel;
  double_CNTChannel                 = (double)CNTChannel;
  double_ADChannel                  = (double)ADChannel;
  double_TerminalNumber_M           = (double)TerminalNumber_M;
  double_TerminalNumber_E           = (double)TerminalNumber_E;
  double_TerminalNumber_P           = (double)TerminalNumber_P;
  double_MotorDRVNumber             = (double)MotorDRVNumber;
  double_EncoderCNTNumber           = (double)EncoderCNTNumber;
  double_PotentioADNumber           = (double)PotentioADNumber;
  double_SampleNumber               = (double)SampleNumber;

  LoadData(ParameterFile, '\0',
           "IDNumber",                            &double_IDNumber,
           "SpeedConstant",                       &SpeedConstant,
           "TorqueConstant",                      &TorqueConstant,
           "TerminalResistance",                  &TerminalResistance,
           "AmpType",                             &double_AmpType,
           "AmpFactor",                           &AmpFactor,
           "AmpOffset(HEX)",                      &AmpOffset,               // [Note] 2 styles available
           "AmpOffset(hex)",                      &AmpOffset,
           "AmpOffset",                           &AmpOffset,
           "DirectionMatching_motor",             &double_DirectionMatching_motor,
           "AuxType",                             &double_AuxType,

           "ResolutionMulti",                     &ResolutionMulti,
           "Resolution",                          &Resolution,
           "DirectionMatching_encoder",           &double_DirectionMatching_encoder,
           "CounterCapacity(HEX)",                &double_CounterCapacity,  // [Note] 2 styles available
           "CounterCapacity(hex)",                &double_CounterCapacity,
           "CounterCapacity",                     &double_CounterCapacity,

           "PGain_D",                             &PGain_D,
           "PGain_V",                             &PGain_V,
           "IGain_D",                             &IGain_D,
           "IGain_V",                             &IGain_V,
           "DGain_D",                             &DGain_D,
           "DGain_V",                             &DGain_V,
           "LimitCommandVoltage_p",               &LimitCommandVoltage_p,
           "LimitCommandVoltage_n",               &LimitCommandVoltage_n,
           "LimitCommandDigital_p(HEX)",          &LimitCommandDigital_p,  // [Note] Only Haxadecimal
           "LimitCommandDigital_p(hex)",          &LimitCommandDigital_p,
           "LimitCommandDigital_n(HEX)",          &LimitCommandDigital_n,  // [Note] Only Haxadecimal
           "LimitCommandDigital_n(hex)",          &LimitCommandDigital_n,

           "LimitTorque",                         &LimitTorque,
           "LimitAngularVelocity",                &LimitAngularVelocity,
           "LimitAngularDisplacement_p",          &LimitAngularDisplacement_p,
           "LimitAngularDisplacement_n",          &LimitAngularDisplacement_n,

           "DeadendAngularDisplacement_p",        &DeadendAngularDisplacement_p,
           "DeadendAngularDisplacement_n",        &DeadendAngularDisplacement_n,
           "DeadendPotentioValue_p(HEX)",         &DeadendPotentioValue_p, // [Note] 2 styles available
           "DeadendPotentioValue_p(hex)",         &DeadendPotentioValue_p,
           "DeadendPotentioValue_p",              &DeadendPotentioValue_p,
           "DeadendPotentioValue_n(HEX)",         &DeadendPotentioValue_n, // [Note] 2 styles available
           "DeadendPotentioValue_n(hex)",         &DeadendPotentioValue_n,
           "DeadendPotentioValue_n",              &DeadendPotentioValue_n,

           "DAChannel",                           &double_DAChannel,
           "CNTChannel",                          &double_CNTChannel,
           "ADChannel",                           &double_ADChannel,
           "TerminalNumber_M",                    &double_TerminalNumber_M,
           "TerminalNumber_E",                    &double_TerminalNumber_E,
           "TerminalNumber_P",                    &double_TerminalNumber_P,
           "MotorDRVNumber",                      &double_MotorDRVNumber,
           "EncoderCNTNumber",                    &double_EncoderCNTNumber,
           "PotentioADNumber",                    &double_PotentioADNumber,

           "SampleNumber",                        &double_SampleNumber,
           "LimitErrorSum_D",                     &LimitErrorSum_D,
           "LimitErrorSum_V",                     &LimitErrorSum_V,
           "-end-");                              // Terminator.

  IDNumber                   = (int)double_IDNumber;
  AmpType                    = (int)double_AmpType;
  AuxType                    = (int)double_AuxType;
  DirectionMatching_motor    = (int)double_DirectionMatching_motor;
  DirectionMatching_encoder  = (int)double_DirectionMatching_encoder;
  CounterCapacity            = (unsigned long)double_CounterCapacity;
  DAChannel                  = (int)double_DAChannel;
  CNTChannel                 = (int)double_CNTChannel;
  ADChannel                  = (int)double_ADChannel;
  TerminalNumber_M           = (int)double_TerminalNumber_M,
  TerminalNumber_E           = (int)double_TerminalNumber_E,
  TerminalNumber_P           = (int)double_TerminalNumber_P,
  MotorDRVNumber             = (int)double_MotorDRVNumber,
  EncoderCNTNumber           = (int)double_EncoderCNTNumber,
  PotentioADNumber           = (int)double_PotentioADNumber,
  SampleNumber               = (int)double_SampleNumber;

  // Calculate necessary values.
  SyntheticResolution_encoder  = ResolutionMulti * Resolution / (2.0 * PI);
  CountMedian                  = (unsigned long)(CounterCapacity / 2);
  SyntheticResolution_potentio = (DeadendPotentioValue_p - DeadendPotentioValue_n)
                               / (DeadendAngularDisplacement_p - DeadendAngularDisplacement_n);
  AngularDisplacement_potentio_offset = DeadendAngularDisplacement_p
                                      - DeadendPotentioValue_p / SyntheticResolution_potentio;

  // Initialize backups.
  PGain_D_backup = PGain_D;
  PGain_V_backup = PGain_V;
  IGain_D_backup = IGain_D;
  IGain_V_backup = IGain_V;
  DGain_D_backup = DGain_D;
  DGain_V_backup = DGain_V;
  LimitAngularDisplacement_p_backup = LimitAngularDisplacement_p;
  LimitAngularDisplacement_n_backup = LimitAngularDisplacement_n;
}

///-----[TexART_ServoMotor2015::SetAngularDisplacement]--------------------------------
void TexART_ServoMotor2015::SetAngularDisplacement(double _AngularDisplacement)
{
  // Check existence of encoder or potentio.
  if(AuxType == 0){
    cout << "ID=" << IDNumber << " SetAngularDisplacement : This motor has no encoder or potentio. \n";
    return;
  }

  // Set the AngularDisplacement, and its offset factor.
  AngularDisplacement = _AngularDisplacement;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10){
    AngularDisplacement_encoder = AngularDisplacement_encoder_offset = AngularDisplacement;
    EncoderCount = CountMedian;
    if(CNTDeviceHandle != 0)
      CNTDeviceHandle->SetCount(CNTChannel, /*(DWORD)*/CountMedian);
    for (int i=0; i<SampleNumber; i++) {
      AngularDisplacement_encoder_RingBuffer[i] = AngularDisplacement;
      AngularVelocity_encoder_RingBuffer[i]     = 0.0;
      AngularAcceleration_encoder_RingBuffer[i] = 0.0;
    }
  }

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4){
    AngularDisplacement_potentio = AngularDisplacement;
    if(ADDeviceHandle != 0){
      PotentioValue = ADDeviceHandle->ADIn(ADChannel);
      AngularDisplacement_potentio_offset = AngularDisplacement - PotentioValue / SyntheticResolution_potentio;
    }
    for (int i=0; i<SampleNumber; i++) {
      AngularDisplacement_potentio_RingBuffer[i] = AngularDisplacement;
      AngularVelocity_potentio_RingBuffer[i]     = 0.0;
      AngularAcceleration_potentio_RingBuffer[i] = 0.0;
    }
  }

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8){
    AngularDisplacement_encoder = AngularDisplacement_encoder_offset = AngularDisplacement;
    EncoderCount = CountMedian;
    if(NCordHandle != 0)
      NCordHandle->ResetCounter(TerminalNumber_E, EncoderCNTNumber, CountMedian);
    for (int i=0; i<SampleNumber; i++) {
      AngularDisplacement_encoder_RingBuffer[i] = AngularDisplacement;
      AngularVelocity_encoder_RingBuffer[i]     = 0.0;
      AngularAcceleration_encoder_RingBuffer[i] = 0.0;
    }
  }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10){
    AngularDisplacement_potentio = AngularDisplacement;
    if(NCordHandle != 0){
      NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
      AngularDisplacement_potentio_offset = AngularDisplacement - PotentioValue / SyntheticResolution_potentio;
    }
    for (int i=0; i<SampleNumber; i++) {
      AngularDisplacement_potentio_RingBuffer[i] = AngularDisplacement;
      AngularVelocity_potentio_RingBuffer[i]     = 0.0;
      AngularAcceleration_potentio_RingBuffer[i] = 0.0;
    }
  }
}

///-----[TexART_ServoMotor2015::CaliAngularDisplacementByPotentio]----------
void TexART_ServoMotor2015::CaliAngularDisplacementByPotentio(void)
{
  // Check existence of potentio.
  if(AuxType == 0 || AuxType == 1 || AuxType == 5){
    cout << "ID=" << IDNumber << " CaliAngularDisplacementByPotentio : This motor has no potentio. \n";
    return;
 }

  // Get PotentioValue.
  switch(AuxType){
    case 2: case 3: case 4: // Using outer AD board.
      if(ADDeviceHandle != 0)
        PotentioValue = ADDeviceHandle->ADIn(ADChannel);
      break;

    case 6: case 7: case 8: case 9: case 10: // Using NCord AD.
      if(NCordHandle != 0){
        NCordHandle->Contact();
        PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
      }
  }

  // Calculate present AngularDisplacement_potentio.
  AngularDisplacement_potentio = (PotentioValue - DeadendPotentioValue_n) / SyntheticResolution_potentio
                               + DeadendAngularDisplacement_n;

  // Set AngularDisplacement.
  SetAngularDisplacement(AngularDisplacement_potentio);
}

///-----[TexART_ServoMotor2015::ResetReferences]-----------------------------------------
void TexART_ServoMotor2015::ResetReferences(void)
{
  // Contact to NCord system to get present data.
  if(NCordHandle != 0) NCordHandle->Contact();

  // Update AngularDisplacement_encoder & AngularDisplacement_potentio.
  AngularDisplacement_encoder  = 0.0;
  AngularDisplacement_potentio = 0.0;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10){
    if(CNTDeviceHandle != 0)
      EncoderCount = CNTDeviceHandle->Get(CNTChannel);
      AngularDisplacement_encoder = DirectionMatching_encoder
                                  * ((double)EncoderCount - (double)CountMedian) / SyntheticResolution_encoder
                                  + AngularDisplacement_encoder_offset;
  }

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4){
    if(ADDeviceHandle != 0)
      PotentioValue = ADDeviceHandle->ADIn(ADChannel);
      AngularDisplacement_potentio = PotentioValue / SyntheticResolution_potentio
                                   + AngularDisplacement_potentio_offset;
  }


  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8){
    if(NCordHandle != 0){
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
      AngularDisplacement_encoder = DirectionMatching_encoder
                                  * ((double)EncoderCount - (double)CountMedian) / SyntheticResolution_encoder
                                  + AngularDisplacement_encoder_offset;
    }
  }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10){
    if(NCordHandle != 0){
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
      AngularDisplacement_potentio = PotentioValue / SyntheticResolution_potentio
                                   + AngularDisplacement_potentio_offset;
    }
  }

  // Reset each RingBuffer.
  for (int i=0; i<SampleNumber; i++){
    AngularDisplacement_encoder_RingBuffer[i]  = AngularDisplacement_encoder;
    AngularVelocity_encoder_RingBuffer[i]      = 0.0;
    AngularAcceleration_encoder_RingBuffer[i]  = 0.0;
    AngularDisplacement_potentio_RingBuffer[i] = AngularDisplacement_potentio;
    AngularVelocity_potentio_RingBuffer[i]     = 0.0;
    AngularAcceleration_potentio_RingBuffer[i] = 0.0;
  }
  Time_last = MothorCLK.Elapsed();

  // Update AngularDisplacement.
  switch(AuxType){
    // Encoder is the referenced main displacement sensor.
    case 1: case 3: case 5: case 7: case 9:
      AngularDisplacement = AngularDisplacement_encoder;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      break;

   // Potentio is the referenced main displacement sensor.
    case 2: case 4: case 6: case 8: case 10:
      AngularDisplacement = AngularDisplacement_potentio;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      break;

    // No referenced main displacement sensor.
    default:
      AngularDisplacement = 0.0;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      ClearAllGain();
  }

  // Reset references.
  Reference_D = AngularDisplacement;
  Reference_V = 0.0;
  Reference_T = 0.0;
  ErrorSum_D  = 0.0;
  ErrorSum_V  = 0.0;

  // Clear versatile parameters.
  VersatileParameter_01 = 0.0;
  VersatileParameter_02 = 0.0;
  VersatileParameter_03 = 0.0;
}

///-----[TexART_ServoMotor2015::TorqueCut]--------------------------------------------------------
void TexART_ServoMotor2015::TorqueCut(void){

  // Set command to rest the motor AMP.
  switch(AmpType){
    // Using outer DA board.
    case 0: case 1: case 2:
      if(DADeviceHandle != 0)
        DADeviceHandle->DAOut(DAChannel, 0.0 + AmpOffset);
      break;

    // Using NCord AMP.
    case 10:
    if(NCordHandle != 0)
      NCordHandle->Terminal[TerminalNumber_M].Drv_Command[MotorDRVNumber] = 0x0000; // Froced idling.
  }

}

///-----[TexART_ServoMotor2015::Free]--------------------------------------------------------
void TexART_ServoMotor2015::Free(void){

  // Cut the motor torque.
  TorqueCut();

  // Reset all references.
  ResetReferences();
}

///-----[TexART_ServoMotor2015::SetReference_*]---------------------------------------------
void TexART_ServoMotor2015::SetReference_D(double _Reference_D){ Reference_D = _Reference_D; }
void TexART_ServoMotor2015::SetReference_V(double _Reference_V){ Reference_V = _Reference_V; }
void TexART_ServoMotor2015::SetReference_T(double _Reference_T){ Reference_T = _Reference_T; }

///-----[TexART_ServoMotor2015::CalcPIDControl]---------------------------------------------
void TexART_ServoMotor2015::CalcPIDControl(void)
{
  int i_1;   // = PresentNumber - 1;
  double Time_present;
  Time_present = MothorCLK.Elapsed();

  // Change PresentNumber to shift the present ring buffer cell.
  PresentNumber++;
    if(PresentNumber >= SampleNumber) PresentNumber -= SampleNumber;
  i_1 = PresentNumber - 1;
    if(i_1 < 0) i_1 += SampleNumber;

  // Get the present values for the ring buffers.
  /* Except using TexART_NCord system, each acquired value should be collected in one time connection to each PC interface board
     every the main control loop. This way should be consisted out of this function.
     How to make the way, and see a sample program on the end of this file. */

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];


  AngularDisplacement_encoder_RingBuffer[PresentNumber]  = DirectionMatching_encoder
                                                         * ((double)EncoderCount - (double)CountMedian) / SyntheticResolution_encoder
                                                         + AngularDisplacement_encoder_offset;
  AngularVelocity_encoder_RingBuffer[PresentNumber]      = (AngularDisplacement_encoder_RingBuffer[PresentNumber] - AngularDisplacement_encoder_RingBuffer[i_1])
                                                         / (Time_present - Time_last);
  AngularAcceleration_encoder_RingBuffer[PresentNumber]  = (AngularVelocity_encoder_RingBuffer[PresentNumber] - AngularVelocity_encoder_RingBuffer[i_1])
                                                         / (Time_present - Time_last);
  AngularDisplacement_potentio_RingBuffer[PresentNumber] = PotentioValue / SyntheticResolution_potentio
                                                         + AngularDisplacement_potentio_offset;
  AngularVelocity_potentio_RingBuffer[PresentNumber]     = (AngularDisplacement_potentio_RingBuffer[PresentNumber] - AngularDisplacement_potentio_RingBuffer[i_1])
                                                         / (Time_present - Time_last);
  AngularAcceleration_potentio_RingBuffer[PresentNumber] = (AngularVelocity_potentio_RingBuffer[PresentNumber] - AngularVelocity_potentio_RingBuffer[i_1])
                                                         / (Time_present - Time_last);

  // Select referenced sensor, and calculate the angular displacement, velocity and acceleration as moving averages.
  AngularDisplacement_encoder  = 0.0;
  AngularDisplacement_potentio = 0.0;

  for(int i=0; i<SampleNumber; i++) {
    AngularDisplacement_encoder  += AngularDisplacement_encoder_RingBuffer[i];
    AngularDisplacement_potentio += AngularDisplacement_potentio_RingBuffer[i];
  }
  AngularDisplacement_encoder  /= SampleNumber;
  AngularDisplacement_potentio /= SampleNumber;

  switch(AuxType){
    // Encoder is the referenced sensor.
    case 1: case 3: case 5: case 7: case 9:
      AngularDisplacement = AngularDisplacement_encoder;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      for(int i=0; i<SampleNumber; i++) {
        AngularVelocity     += AngularVelocity_encoder_RingBuffer[i];
        AngularAcceleration += AngularAcceleration_encoder_RingBuffer[i];
      }
      AngularVelocity     /= SampleNumber;
      AngularAcceleration /= SampleNumber;
    break;

    // Potentio is the referenced sensor.
    case 2: case 6:
      AngularDisplacement = AngularDisplacement_potentio;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      for(int i=0; i<SampleNumber; i++) {
        AngularVelocity     += AngularVelocity_potentio_RingBuffer[i];
        AngularAcceleration += AngularAcceleration_potentio_RingBuffer[i];
      }
      AngularVelocity     /= SampleNumber;
      AngularAcceleration /= SampleNumber;
    break;

    // Encoder and Potentio shares roles.
    case 4: case 8: case 10:
      AngularDisplacement = AngularDisplacement_potentio;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      for(int i=0; i<SampleNumber; i++) {
        AngularVelocity     += AngularVelocity_encoder_RingBuffer[i];
        AngularAcceleration += AngularAcceleration_encoder_RingBuffer[i];
      }
      AngularVelocity     /= SampleNumber;
      AngularAcceleration /= SampleNumber;
    break;

    // No referenced sensor.
    default:
      AngularDisplacement = 0.0;
      AngularVelocity     = 0.0;
      AngularAcceleration = 0.0;
      ClearAllGain();
  }

  // Calculate accumulations of error.
  if(IGain_D != 0.0) {
    ErrorSum_D += Reference_D - AngularDisplacement;
    if ((LimitErrorSum_D != 0.0) && (ErrorSum_D > LimitErrorSum_D)) {
      ErrorSum_D = LimitErrorSum_D;
    }
    else if ((LimitErrorSum_D != 0.0) && (ErrorSum_D < -LimitErrorSum_D)) {
      ErrorSum_D = -LimitErrorSum_D;
    }
  }
  if(IGain_V != 0.0) {
    ErrorSum_V += Reference_V - AngularVelocity ;
    if ((LimitErrorSum_V != 0.0) && (ErrorSum_V > LimitErrorSum_V)) {
      ErrorSum_V = LimitErrorSum_V;
    }
    else if ((LimitErrorSum_V != 0.0) && (ErrorSum_V < -LimitErrorSum_V)) {
      ErrorSum_V = -LimitErrorSum_V;
    }
  }

  // Avoid over shoot into the prohibited area.
  if(( LimitAngularDisplacement_p == 0.0 ) && ( LimitAngularDisplacement_n == 0.0 )); // Mode of no limitation by AngularDisplacement.
  else{
    if((Reference_D > LimitAngularDisplacement_p) || ((AngularDisplacement > LimitAngularDisplacement_p) && (Reference_V > 0))) {
       Reference_D = LimitAngularDisplacement_p;
       Reference_V = AngularVelocity = AngularAcceleration = ErrorSum_D = ErrorSum_V = 0.0;
    }
    if((Reference_D < LimitAngularDisplacement_n) || ((AngularDisplacement < LimitAngularDisplacement_n) && (Reference_V < 0))) {
       Reference_D = LimitAngularDisplacement_n;
       Reference_V = AngularVelocity = AngularAcceleration = ErrorSum_D = ErrorSum_V = 0.0;
    }
  }

  // Avoid over speed 1.
  if( LimitAngularVelocity > 0.0 ){
    if( Reference_V > LimitAngularVelocity ) {
      Reference_V = LimitAngularVelocity;
      ErrorSum_D = ErrorSum_V = 0.0;
    }
    else if( Reference_V < -LimitAngularVelocity ) {
      Reference_V = -LimitAngularVelocity;
      ErrorSum_D = ErrorSum_V = 0.0;
    }
  }

  // Calculate Torque which should be generated at the setvo-motor.
  Torque = PGain_D * ( Reference_D - AngularDisplacement  + IGain_D * ErrorSum_D )
         + DGain_D * (             - AngularVelocity )
         + PGain_V * ( Reference_V - AngularVelocity      + IGain_V * ErrorSum_V )
         + DGain_V * (             - AngularAcceleration )
         + Reference_T;

  // Avoid over speed 2.
  if( LimitAngularVelocity > 0.0 ){
    if( AngularVelocity > LimitAngularVelocity ) {
      cout << "ID=" << IDNumber << " CalcPIDControl : AngularVelocity = " << AngularVelocity << "[rad/2] > LimitAngularVelocity \n";
        Torque = 0.0;


//      if((1.0 - (AngularVelocity - LimitAngularVelocity)/LimitAngularVelocity*10.0) > 0)
//        Torque *= (1.0 - (AngularVelocity - LimitAngularVelocity)/LimitAngularVelocity*10.0);
//      else
//        Torque = 0.0;
      ErrorSum_D = ErrorSum_V = 0.0;
    }
    else if( AngularVelocity < -LimitAngularVelocity ) {
      cout << "ID=" << IDNumber << " CalcPIDControl : AngularVelocity = " << AngularVelocity << "[rad/2] < -LimitAngularVelocity \n";
/*      if((1.0 - (LimitAngularVelocity - AngularVelocity)/LimitAngularVelocity*10.0) > 0)
        Torque *= (1.0 - (LimitAngularVelocity - AngularVelocity)/LimitAngularVelocity*10.0);
      else*/
        Torque = 0.0;
      ErrorSum_D = ErrorSum_V = 0.0;
    }
  }

  // Avoid over torque.
  if( Torque > LimitTorque ){
    Torque = LimitTorque;
    ErrorSum_D = ErrorSum_V = 0.0;
  }
  else if( Torque < -LimitTorque ){
    Torque = -LimitTorque;
    ErrorSum_D = ErrorSum_V = 0.0;
  }

  // Calculate Command.
  switch(AmpType){
    case 0: // Analog voltage control type amplifier.
      CommandVoltage = DirectionMatching_motor
                     * ( Torque * TerminalResistance / TorqueConstant / AmpFactor
                       + AngularVelocity / SpeedConstant / AmpFactor ) + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 1: // Analog current control type amplifier.
      CommandVoltage = DirectionMatching_motor * Torque / TorqueConstant / AmpFactor + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 2: // Analog torque control type amplifier.
      CommandVoltage = DirectionMatching_motor * Torque / AmpFactor + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 10: // TexART_NCord system = Digital current control type amplifire.
      CommandDigital = DirectionMatching_motor * Torque / TorqueConstant / AmpFactor + AmpOffset;
      //-----Avoid overflow of DriverCommand.
      if     (CommandDigital > LimitCommandDigital_p) {CommandDigital = LimitCommandDigital_p; cout << "Overflow DriverCommand" << endl;}
      else if(CommandDigital < LimitCommandDigital_n) {CommandDigital = LimitCommandDigital_n; cout << "Overflow DriverCommand" << endl;}
      if(NCordHandle != 0)
        NCordHandle->Terminal[TerminalNumber_M].Drv_Command[MotorDRVNumber] = (int)CommandDigital;
  }

  // Set Time_last.
  Time_last = Time_present;

  // Show private data for check.
//   cout << "ID=" << IDNumber << " Torque = "              << Torque              << endl;
//   cout << "ID=" << IDNumber << " Reference_T = "         << Reference_T         << endl;
//   cout << "ID=" << IDNumber << " AngularDisplacement = " << AngularDisplacement << endl;
//   cout << "ID=" << IDNumber << " AngularVelocity = "     << AngularVelocity     << endl;
//   cout << "ID=" << IDNumber << " CommandVoltage = "      << CommandVoltage      << endl;
//   cout << "ID=" << IDNumber << " CommandDigital = "      << CommandDigital      << endl;
}

///-----[TexART_ServoMotor2015::ExecuteServo]------------------------------------------
// [Note] This is unrecommendable for usage by a consumer.
//        This remains to be embedded in only several utility functions;
//        GoDeadEndPosition(), SearchDeadendPotentioValueAuto(), GoNextPositionOnTime() and Fix().
void TexART_ServoMotor2015::ExecuteServo(void)
{
  // Get the present values for the ring buffers.
  /* Values provided from TexART_NCord system are updated in function CalcPIDControl. */

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount  = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  // Set remaining procedure for the servo.
  CalcPIDControl();

  // Output CommandVoltage when using outer DA board.
  if(AmpType == 0 || AmpType == 1 || AmpType == 2)
    if(DADeviceHandle != 0) DADeviceHandle->DAOut(DAChannel, CommandVoltage);

  // NCord Contact.
  if(NCordHandle != 0) NCordHandle->Contact();
}

///-----[TexART_ServoMotor2015::Change?Gain_?]-----------------------------------------
void TexART_ServoMotor2015::ChangePGain_D(double _PGain_D){ PGain_D_backup = PGain_D = _PGain_D; }
void TexART_ServoMotor2015::ChangePGain_V(double _PGain_V){ PGain_V_backup = PGain_V = _PGain_V; }
void TexART_ServoMotor2015::ChangeIGain_D(double _IGain_D){ IGain_D_backup = IGain_D = _IGain_D; }
void TexART_ServoMotor2015::ChangeIGain_V(double _IGain_V){ IGain_V_backup = IGain_V = _IGain_V; }
void TexART_ServoMotor2015::ChangeDGain_D(double _DGain_D){ DGain_D_backup = DGain_D = _DGain_D; }
void TexART_ServoMotor2015::ChangeDGain_V(double _DGain_V){ DGain_V_backup = DGain_V = _DGain_V; }

///-----[TexART_ServoMotor2015::ZeroGain_?]--------------------------------------------
void TexART_ServoMotor2015::ZeroPGain_D(void){ PGain_D = 0.0; }
void TexART_ServoMotor2015::ZeroPGain_V(void){ PGain_V = 0.0; }
void TexART_ServoMotor2015::ZeroIGain_D(void){ IGain_D = 0.0; }
void TexART_ServoMotor2015::ZeroIGain_V(void){ IGain_V = 0.0; }
void TexART_ServoMotor2015::ZeroDGain_D(void){ DGain_D = 0.0; }
void TexART_ServoMotor2015::ZeroDGain_V(void){ DGain_V = 0.0; }

void TexART_ServoMotor2015::ZeroGain_D(void)
{
  PGain_D = 0.0;
  IGain_D = 0.0;
  DGain_D = 0.0;
}

void TexART_ServoMotor2015::ZeroGain_V(void)
{
  PGain_V = 0.0;
  IGain_V = 0.0;
  DGain_V = 0.0;
}

void TexART_ServoMotor2015::ZeroLimitAngularDisplacement(void)
{
  LimitAngularDisplacement_p = 0.0;
  LimitAngularDisplacement_n = 0.0;
}

///-----[TexART_ServoMotor2015::ReloadGain_?]------------------------------------------
void TexART_ServoMotor2015::ReloadGain_D(void)
{
  PGain_D = PGain_D_backup;
  IGain_D = IGain_D_backup;
  DGain_D = DGain_D_backup;
}

void TexART_ServoMotor2015::ReloadGain_V(void)
{
  PGain_V = PGain_V_backup;
  IGain_V = IGain_V_backup;
  DGain_V = DGain_V_backup;
}

void TexART_ServoMotor2015::ReloadLimitAngularDisplacement(void)
{
  LimitAngularDisplacement_p = LimitAngularDisplacement_p_backup;
  LimitAngularDisplacement_n = LimitAngularDisplacement_n_backup;
}

///-----[Change limit parameters.
void TexART_ServoMotor2015::ChangeLimitAngularDisplacement_p(double _LimitAngularDisplacement_p)
{ LimitAngularDisplacement_p  = _LimitAngularDisplacement_p; }
void TexART_ServoMotor2015::ChangeLimitAngularDisplacement_n(double _LimitAngularDisplacement_n)
{ LimitAngularDisplacement_n  = _LimitAngularDisplacement_n; }
void TexART_ServoMotor2015::ChangeLimitAngularVelocity      (double _LimitAngularVelocity)
{ LimitAngularVelocity        = _LimitAngularVelocity;       }
void TexART_ServoMotor2015::ChangeLimitTorque               (double _LimitTorque)
{ LimitTorque                 = _LimitTorque;                }

///-----[TexART_ServoMotor2015::ClearAllGain]------------------------------------------
void TexART_ServoMotor2015::ClearAllGain(void){ PGain_D = PGain_V = IGain_D = IGain_V = DGain_D = DGain_V = 0.0; }

///-----[TexART_ServoMotor2015::ClearErrorSum]-----------------------------------------
void TexART_ServoMotor2015::ClearErrorSum(void){ ErrorSum_D = ErrorSum_V = 0.0; }

///-----[TexART_ServoMotor2015::ClearVersatileParameters]------------------------------
void TexART_ServoMotor2015::ClearVersatileParameters(void){
  VersatileParameter_01 = 0.0;
  VersatileParameter_02 = 0.0;
  VersatileParameter_03 = 0.0;
}

///-----[TexART_ServoMotor2015::ShowPrivateData]---------------------------------------
void TexART_ServoMotor2015::ShowPrivateData(void)
{
  char c;
  cout << "IDNumber                   = " << IDNumber                   << endl;

  cout << "AngularDisplacement        = " << AngularDisplacement        << "[rad]      " << endl;
  cout << "AngularVelocity            = " << AngularVelocity            << "[rad/s]    " << endl;
  cout << "AngularAcceleration        = " << AngularAcceleration        << "[rad/s^2]  " << endl;
  cout << "Torque                     = " << Torque                     << "[N]        " << endl;

  cout << "TorqueConstant             = " << TorqueConstant             << "[Nm/A]     " << endl;
  cout << "SpeedConstant              = " << SpeedConstant              << "[rad/(s*V)]" << endl;
  cout << "TerminalResistance         = " << TerminalResistance         << "[Ohm]      " << endl;

  cout << "AmpType                    = " << AmpType                    << endl;
  cout << "AmpFactor                  = " << AmpFactor                  << endl;
  cout << "AmpOffset                  = " << AmpOffset                  << endl;
  cout << "DirectionMatching_motor    = " << DirectionMatching_motor    << endl;
  cout << "AuxType                    = " << AuxType                    << endl;

  cout << "Resolution                 = " << Resolution                 << endl;
  cout << "ResolutionMulti            = " << ResolutionMulti            << endl;
  cout << "DirectionMatching_encoder  = " << DirectionMatching_encoder  << endl;
  cout << "CounterCapacity            = " << CounterCapacity            << endl;
  cout << "CountMedian                = " << CountMedian                << endl;

  cout << "PGain_D                    = " << PGain_D                    << endl;
  cout << "PGain_V                    = " << PGain_V                    << endl;
  cout << "IGain_D                    = " << IGain_D                    << endl;
  cout << "IGain_V                    = " << IGain_V                    << endl;
  cout << "DGain_D                    = " << DGain_D                    << endl;
  cout << "DGain_V                    = " << DGain_V                    << endl;

  cout << "Reference_D                = " << Reference_D                << endl;
  cout << "Reference_V                = " << Reference_V                << endl;
  cout << "Reference_T                = " << Reference_T                << endl;
  cout << "CommandVoltage             = " << CommandVoltage             << endl;
  cout << "CommandDigital             = " << CommandDigital             << endl;

  cout << "LimitTorque                = " << LimitTorque                << "[Nm]       " << endl;
  cout << "LimitAngularVelocity       = " << LimitAngularVelocity       << "[rad/s]    " << endl;
  cout << "LimitAngularDisplacement_p = " << LimitAngularDisplacement_p << "[rad]      " << endl;
  cout << "LimitAngularDisplacement_n = " << LimitAngularDisplacement_n << "[rad]      " << endl;
  cout << "DeadendAngularDisplacement_p = " << DeadendAngularDisplacement_p << "[rad]  " << endl;
  cout << "DeadendAngularDisplacement_n = " << DeadendAngularDisplacement_n << "[rad]  " << endl;
  cout << "DeadendPotentioValue_p     = " << DeadendPotentioValue_p     << endl;
  cout << "DeadendPotentioValue_n     = " << DeadendPotentioValue_n     << endl;

  cout << "DAChannel                  = " << DAChannel                  << endl;
  cout << "CNTChannel                 = " << CNTChannel                 << endl;
  cout << "ADChannel                  = " << ADChannel                  << endl;

  cout << "TerminalNumber_M           = " << TerminalNumber_M           << endl;
  cout << "TerminalNumber_E           = " << TerminalNumber_E           << endl;
  cout << "TerminalNumber_P           = " << TerminalNumber_P           << endl;
  cout << "MotorDRVNumber             = " << MotorDRVNumber             << endl;
  cout << "EncoderCNTNumber           = " << EncoderCNTNumber           << endl;
  cout << "PotentioADNumber           = " << PotentioADNumber           << endl;

  cout << "SampleNumber               = " << SampleNumber               << endl;
  cout << "PresentNumber              = " << PresentNumber              << endl;
  cout << "ErrorSum_D                 = " << ErrorSum_D                 << endl;
  cout << "ErrorSum_V                 = " << ErrorSum_V                 << endl;

  cout << "Input any key to resume.";
  cin  >> c;
}

///-----[TexART_ServoMotor2015::Read?Gain_?]-------------------------------------------
double TexART_ServoMotor2015::ReadPGain_D(void){ return PGain_D; }
double TexART_ServoMotor2015::ReadPGain_V(void){ return PGain_V; }
double TexART_ServoMotor2015::ReadIGain_D(void){ return IGain_D; }
double TexART_ServoMotor2015::ReadIGain_V(void){ return IGain_V; }
double TexART_ServoMotor2015::ReadDGain_D(void){ return DGain_D; }
double TexART_ServoMotor2015::ReadDGain_V(void){ return DGain_V; }

double TexART_ServoMotor2015::ReadLimitAngularDisplacement_p  (void){ return LimitAngularDisplacement_p  ; }
double TexART_ServoMotor2015::ReadLimitAngularDisplacement_n  (void){ return LimitAngularDisplacement_n  ; }
double TexART_ServoMotor2015::ReadDeadendAngularDisplacement_p(void){ return DeadendAngularDisplacement_p; }
double TexART_ServoMotor2015::ReadDeadendAngularDisplacement_n(void){ return DeadendAngularDisplacement_n; }
double TexART_ServoMotor2015::ReadLimitAngularVelocity        (void){ return LimitAngularVelocity        ; }
double TexART_ServoMotor2015::ReadLimitTorque                 (void){ return LimitTorque                 ; }
double TexART_ServoMotor2015::ReadReference_D                 (void){ return Reference_D                 ; }
double TexART_ServoMotor2015::ReadReference_V                 (void){ return Reference_V                 ; }
double TexART_ServoMotor2015::ReadReference_T                 (void){ return Reference_T                 ; }
double TexART_ServoMotor2015::ReadCommandVoltage              (void){ return CommandVoltage              ; }
double TexART_ServoMotor2015::ReadCommandDigital              (void){ return CommandDigital              ; }
double TexART_ServoMotor2015::ReadAngularDisplacement_encoder (void){ return AngularDisplacement_encoder ; }
double TexART_ServoMotor2015::ReadAngularDisplacement_potentio(void){ return AngularDisplacement_potentio; }

unsigned long TexART_ServoMotor2015::ReadEncoderCount(void)
{
  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount  = CNTDeviceHandle->Get(CNTChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0){
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  return EncoderCount;
}

double TexART_ServoMotor2015::ReadPotentioValue(void)
{
  switch(AuxType){
    // Using outer AD board.
    case 2: case 3: case 4:
      if(ADDeviceHandle != 0)
        DeadendPotentioValue_p = ADDeviceHandle->ADIn(ADChannel);
      break;
    // Using NCord AD.
    case 6: case 7: case 8: case 9: case 10:
      if(NCordHandle != 0){
        //NCordHandle->Contact();
        DeadendPotentioValue_p = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
      }
  }

  return PotentioValue;
}

///-----[TexART_ServoMotor2015::SearchDirectionMatching_encoder]-----------------------
void TexART_ServoMotor2015::SearchDirectionMatching_encoder(void)
{
  unsigned long  EncoderCount1,  EncoderCount2;

  // Starting.
  cout << endl << "Start [SearchDirectionMatching_encoder]" << endl;
  cout << "Search and set the DirectionMatching_encoder. IDNumber = " << IDNumber << endl;

  // Get the starting values.
  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount1  = CNTDeviceHandle->Get(CNTChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0){
      NCordHandle->Contact();
      EncoderCount1 = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  // Evaluate DirectionMatching_encoder.
  cout << "Rotate the joint in a direction that you wish to be the posotive one." << endl;
  while(1){
    // Get the present values.
    /* Using outer CNT board. */
    if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
      if(CNTDeviceHandle != 0) EncoderCount2  = CNTDeviceHandle->Get(CNTChannel);

    /* Using NCord CNT. */
    if(AuxType == 5 || AuxType == 7 || AuxType == 8)
      if(NCordHandle != 0){
        NCordHandle->Contact();
        EncoderCount1 = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
      }

    // Evaluate change of EncoderCount.
    if(EncoderCount2 > EncoderCount1 + 0.1 * SyntheticResolution_encoder){
      DirectionMatching_encoder = 1;
      cout << "EncoderCount increase : DirectionMatching_encoder = 1" << endl;
      break;
    }
    else if(EncoderCount2 < EncoderCount1 - 0.1 * SyntheticResolution_encoder){
      DirectionMatching_encoder = -1;
      cout << "EncoderCount decrease : DirectionMatching_encoder = -1" << endl;
      break;
    }
  }

  // Ending.
  cout << "Finished. Thank you." << endl;
}

///-----[TexART_ServoMotor2015::SearchDirectionMatching_motor]-------------------------
void TexART_ServoMotor2015::SearchDirectionMatching_motor(double Torque_tmp = 0.0)
{
  char ctemp;

  // Starting.
  cout << endl << "Start [SearchDirectionMatching_motor]" << endl;
  cout << "Search and set the DirectionMatching_motor. IDNumber = " << IDNumber << endl;
  cout << "Please input any letter to start. ";
  cin >> ctemp;

  // Set temporal command Torque.
  cout << "Torque_tmp = " << Torque_tmp << " [Nm]. OK? [Y/N] ";
  cin >> ctemp;
  if (ctemp != 'y' && ctemp != 'Y'){
    cout << "Input Torque_tmp [Nm] = ";
    cin >> Torque_tmp;
  }

  // Avoid over torque.
  if( Torque_tmp > LimitTorque ) Torque_tmp = LimitTorque;
  else if( Torque_tmp < -LimitTorque ) Torque_tmp = -LimitTorque;

  // Calculate Command.
  switch(AmpType){
    case 0: // Analog voltage control type amplifier.
      CommandVoltage = Torque_tmp * TerminalResistance / TorqueConstant / AmpFactor + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 1: // Analog current control type amplifier.
      CommandVoltage = Torque_tmp / TorqueConstant / AmpFactor + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 2: // Analog torque control type amplifier.
      CommandVoltage = Torque_tmp / AmpFactor + AmpOffset;
      if     (CommandVoltage > LimitCommandVoltage_p) CommandVoltage = LimitCommandVoltage_p;
      else if(CommandVoltage < LimitCommandVoltage_n) CommandVoltage = LimitCommandVoltage_n;
    break;

    case 10: // TexART_NCord system = Digital current control type amplifire.
      CommandDigital = Torque_tmp / TorqueConstant / AmpFactor + AmpOffset;
      //-----Avoid overflow of DriverCommand.
      if     (CommandDigital > LimitCommandDigital_p) CommandDigital = LimitCommandDigital_p;
      else if(CommandDigital < LimitCommandDigital_n) CommandDigital = LimitCommandDigital_n;
  }

  // Output motor torque.
  switch(AmpType){
    // Using outer DA board.
    case 0: case 1: case 2:
      if(DADeviceHandle != 0)
        DADeviceHandle->DAOut(DAChannel, CommandVoltage);
      break;

    // Using NCord AMP.
    case 10:
      if(NCordHandle != 0)
        NCordHandle->Terminal[TerminalNumber_M].Drv_Command[MotorDRVNumber] = (unsigned int)CommandDigital;
  }

  // Evaluate DirectionMatching_motor.
  cout << "Please check the revolute direction of the motor." << endl;
  cout << "Please push a key according to your judge." << endl;
  cout << "[Y: Right direction, N: Opposite direction, <Esc>: Quit]\n";

  while(1){
    NCordHandle->Contact();

    if(GetAsyncKeyState('Y')&0x8000){
      DirectionMatching_motor = -1;
      cout << "Opposite direction : DirectionMatching_motor = -1 \n";
      break;
    }
    else if(GetAsyncKeyState('N')&0x8000){
      DirectionMatching_motor = 1;
      cout << "Right direction : DirectionMatching_motor = 1 \n";
      break;
    }
    else if(GetAsyncKeyState(0x1b)&0x8000) break; // Push <Esc> key to quit this loop.
  }

  // Ending.
  Free();
  cout << "Finished. Thank you." << endl;
}

///-----[TexART_ServoMotor2015::SearchDeadendPotentioValue]-------------------------------------
// [Note] This needs PGain_V.
void TexART_ServoMotor2015::SearchDeadendPotentioValue(void)
{
  // Starting.
  cout << endl << "Start [SearchDeadendPotentioValue]" << endl;
  cout << "Search and set the potentio properties. IDNumber = " << IDNumber << endl;

  // Get DeadendPotentioValue_p.
  cout << "Rotate the joint in the positive direction to deadend, and keep the position." << endl;
  cout << "Please push key [Y] after reaching the deadend. ";

  while(1){
    if(GetAsyncKeyState('Y')&0x8000){
      switch(AuxType){
        // Using outer AD board.
        case 2: case 3: case 4:
          if(ADDeviceHandle != 0)
            DeadendPotentioValue_p = ADDeviceHandle->ADIn(ADChannel);
          break;
        // Using NCord AD.
        case 6: case 7: case 8: case 9: case 10:
          if(NCordHandle != 0){
            NCordHandle->Contact();
            DeadendPotentioValue_p = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
          }
      }
      break;
    }
  }

  // Get DeadendPotentioValue_n.
  cout << "Rotate the joint in the negative direction to deadend, and keep the position." << endl;
  cout << "Please push key [U] after reaching the deadend. ";

  while(1){
    if(GetAsyncKeyState('U')&0x8000){
      switch(AuxType){
        // Using outer AD board.
        case 2: case 3: case 4:
          if(ADDeviceHandle != 0)
            DeadendPotentioValue_n = ADDeviceHandle->ADIn(ADChannel);
          break;
        // Using NCord AD.
        case 6: case 7: case 8: case 9: case 10:
          if(NCordHandle != 0){
            NCordHandle->Contact();
            DeadendPotentioValue_n = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
          }
      }
      break;
    }
  }

  // Calculate potentio properties.
  SyntheticResolution_potentio = (DeadendPotentioValue_p - DeadendPotentioValue_n)
                               / (DeadendAngularDisplacement_p - DeadendAngularDisplacement_n);
  AngularDisplacement_potentio_offset = DeadendAngularDisplacement_p - DeadendPotentioValue_p / SyntheticResolution_potentio;

  // Ending.
  cout << "Finished. Note below values." << endl;
  printf( "  DeadendPotentioValue_p(DEC)         = %8.4f\n",  DeadendPotentioValue_p);
  printf( "  DeadendPotentioValue_p(HEX)         = 0x%08x\n", DeadendPotentioValue_p);
  printf( "  DeadendPotentioValue_n(DEC)         = %8.4f\n",  DeadendPotentioValue_n);
  printf( "  DeadendPotentioValue_n(HEX)         = 0x%08x\n", DeadendPotentioValue_n);
  cout << "Thank you." << endl;
}

///-----[TexART_ServoMotor2015::GoDeadEndPosition](reset AngularDisplacement_encoder)-----
// [Note] This needs PGain_V.
int TexART_ServoMotor2015::GoDeadEndPosition(
double AngularVelocity_tmp,
double StopAngularVelocity,
double StopTorque,
double AngularDisplacement_deadend,
double Time_start,
double Time_present)
{
  double Time_elapsed;

  Time_elapsed = Time_present - Time_start;
  if(Time_elapsed < 0.0) Time_elapsed = 0.0;

  // Skip this routine.
  if(VersatileParameter_01 >= 200.0) // Terminating code.
    return(0);

  // Release the displacement and velocity controls temporarily.
  ZeroGain_D();
  ZeroLimitAngularDisplacement(); // Mode of no limitation by AngularDisplacement.
  DGain_V = 0.0;

  // Execute the velocity PI control.
  Reference_V = AngularVelocity_tmp;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  ExecuteServo();

  // Ending.
    // Reload the displacement and velocity controls.
    ReloadGain_D();
    ReloadLimitAngularDisplacement();
    DGain_V = DGain_V_backup;

  // Check limit value.
  if(Time_elapsed > 1.0) {
    if     (Torque >=  StopTorque) {
      VersatileParameter_01 += 1.0;
      //cout << "ID=" << IDNumber << " GoDeadEndPosition : Torque >=  StopTorque \n";
    }
    else if(Torque <= -StopTorque) {
      VersatileParameter_01 += 1.0;
      //cout << "ID=" << IDNumber << " GoDeadEndPosition : Torque <=  StopTorque \n";
    }
    if((AngularVelocity <= StopAngularVelocity) && (AngularVelocity >= -StopAngularVelocity)) {
      VersatileParameter_01 += 1.0;
      //cout << "ID=" << IDNumber << " GoDeadEndPosition : AngularVelocity <= StopAngularVelocity \n";
    }
  }

  // Terminating,
  if(VersatileParameter_01 >= 199.0){
    // Reset AngularDisplacement_encoder.
    AngularDisplacement_encoder = AngularDisplacement_encoder_offset = AngularDisplacement_deadend;
    EncoderCount = CountMedian;

    // Reset Counter.
    /* Using outer CNT board. */
    if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10){
      if(CNTDeviceHandle != 0)
        CNTDeviceHandle->SetCount(CNTChannel, /*(DWORD)*/CountMedian);
    }

    /* Using NCord CNT. */
    if(AuxType == 5 || AuxType == 7 || AuxType == 8){
      if(NCordHandle != 0)
        NCordHandle->ResetCounter(TerminalNumber_E, EncoderCNTNumber, CountMedian);
    }

    // Reset RingBuffers.
    for (int i=0; i<SampleNumber; i++) {
      AngularDisplacement_encoder_RingBuffer[i] = AngularDisplacement_encoder;
      AngularVelocity_encoder_RingBuffer[i]     = 0.0;
      AngularAcceleration_encoder_RingBuffer[i] = 0.0;
    }

    Free();
    VersatileParameter_01 = 100.0; // Set terminating code.
    ErrorSum_D            = 0.0;
    ErrorSum_V            = 0.0;
    return(0);
  }
  else return(1);
}

///-----[TexART_ServoMotor2015::SearchDeadendPotentioValueAuto](show DeadendPotentioValue)-----------
int TexART_ServoMotor2015::SearchDeadendPotentioValueAuto(
double AngularVelocity_tmp,
double StopAngularVelocity,
double StopTorque,
double Time_start,
double Time_present)
{
  double Time_elapsed;

  Time_elapsed = Time_present - Time_start;
  if(Time_elapsed < 0.0) Time_elapsed = 0.0;

  // Skip this routine.
  if(VersatileParameter_01 >= 100.0) // Terminating code.
    return(0);

  // Release the displacement and velocity controls temporarily.
  ZeroGain_D();
  ZeroLimitAngularDisplacement(); // Mode of no limitation by AngularDisplacement.
  DGain_V        = 0.0;

  // Execute the velocity PI control.
  Reference_V = AngularVelocity_tmp;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  ExecuteServo();

  // Ending.
    // Reload the displacement and velocity controls.
    ReloadGain_D();
    ReloadLimitAngularDisplacement();
    DGain_V = DGain_V_backup;

  // Check limit value.
  if(Time_elapsed > 1.0) {
    if((Torque >= StopTorque) || (Torque <= -StopTorque) ||
       ((AngularVelocity <= StopAngularVelocity) && (AngularVelocity >= -StopAngularVelocity))){
      VersatileParameter_01 += 1.0;
    }
  }

  // Terminating,
  if(VersatileParameter_01 >= 99.0){
    // Show DeadendPotentioValue.
    cout << "Finished. Note below values." << endl;
    printf( "  AngularVelocity_tmp       = %8.4f\n",  AngularVelocity_tmp);
    printf( "  DeadendPotentioValue(DEC) = %8.4f\n",  PotentioValue);
    printf( "  DeadendPotentioValue(HEX) = 0x%08x\n", PotentioValue);
    cout << "Thank you." << endl;

    Free();
    VersatileParameter_01 = 100.0; // Set terminating code.
    ErrorSum_D            = 0.0;
    ErrorSum_V            = 0.0;
    return(0);
  }
  else return(1);
}

///-----[TexART_ServoMotor2015::GoNextPositionOnTime]----------------------------------
int TexART_ServoMotor2015::GoNextPositionOnTime(
double AngularDisplacement_start,
double AngularDisplacement_goal,
double TimeSpan,
double Time_start,
double Time_present)
{
  double Time_elapsed;

  Time_elapsed = Time_present - Time_start;
  if(Time_elapsed < 0.0) Time_elapsed = 0.0;

  // When AngularDisplacement_goal is over range, return with no motion.
  if ((AngularDisplacement_goal > LimitAngularDisplacement_p) ||
	  (AngularDisplacement_goal < LimitAngularDisplacement_n)){
	  cout << "ID=" << IDNumber << " GoNextPositionOnTime : Error in AngularDisplacement_goal = " << AngularDisplacement_goal << "\n";
	  return(0);
  }
  // Release the velocity control temporarily.
  ZeroGain_V();

  // Execute the displacement PI control.
  if(Time_elapsed < TimeSpan)
    Reference_D = (AngularDisplacement_goal - AngularDisplacement_start)
                  * Time_elapsed / TimeSpan + AngularDisplacement_start;
  else
    Reference_D = AngularDisplacement_goal;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  ExecuteServo();

  // Ending.
    // Reload the velocity controls.
    ReloadGain_V();

    // Check progress.
//  Console::locate(0,15);
//  cout << Reference_D << "  " << AngularDisplacement << endl;

  if(Time_elapsed < TimeSpan) return(1);
  else{
    ErrorSum_D = 0.0;
    ErrorSum_V = 0.0;
    return(0);
  }
}

///-----[TexART_ServoMotor2015::GoNextPositionOnTime2]----------------------------------
int TexART_ServoMotor2015::GoNextPositionOnTime2(
double AngularDisplacement_start,
double AngularDisplacement_goal,
double TimeSpan,
double Time_start,
double Time_present)
{
  double Time_elapsed;

  Time_elapsed = Time_present - Time_start;
  if(Time_elapsed < 0.0) Time_elapsed = 0.0;

  // Release the velocity control temporarily.
  ZeroGain_V();

  // Calculate the displacement PI control.
  if(Time_elapsed < TimeSpan)
    Reference_D = (AngularDisplacement_goal - AngularDisplacement_start)
                  * Time_elapsed / TimeSpan + AngularDisplacement_start;
  else
    Reference_D = AngularDisplacement_goal;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  CalcPIDControl();

  // Ending.
    // Reload the velocity controls.
    ReloadGain_V();

    // Check progress.
//  Console::locate(0,15);
//  cout << Reference_D << "  " << AngularDisplacement << endl;

  if(Time_elapsed < TimeSpan) return(1);
  else{
    ErrorSum_D = 0.0;
    ErrorSum_V = 0.0;
    return(0);
  }
}


///-----[TexART_ServoMotor2015::CheckAngularDisplacement]--------------------------------
double TexART_ServoMotor2015::CheckAngularDisplacement(void)
{
  // Get present AngularDisplacement.
  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  // Calculate AngularDisplacement
  switch(AuxType){
    // Encoder is the referenced sensor for displacement ctrl.
    case 1: case 3: case 5: case 7: case 9:
    AngularDisplacement = DirectionMatching_encoder
                        * ((double)EncoderCount - (double)CountMedian) / SyntheticResolution_encoder
                        + AngularDisplacement_encoder_offset;
    break;

    // Potentio is the referenced sensor for displacement ctrl.
    case 2: case 4: case 6: case 8: case 10:
    AngularDisplacement = PotentioValue / SyntheticResolution_potentio
                        + AngularDisplacement_potentio_offset;
  }

  return AngularDisplacement;
}

///-----[TexART_ServoMotor2015::Fix]---------------------------------------------------
void TexART_ServoMotor2015::Fix(void){

  // Release the velocity control temporarily.
  ZeroGain_V();

  // Set reference.
  Reference_D = AngularDisplacement;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  ExecuteServo();

  // Reload the velocity control.
  ReloadGain_V();
}

void TexART_ServoMotor2015::Fix(double Reference_D_tmp){

  // Release the velocity control temporarily.
  ZeroGain_V();

  // Set reference.
  Reference_D = Reference_D_tmp;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  ExecuteServo();

  // Reload the velocity control.
  ReloadGain_V();
}

void TexART_ServoMotor2015::Fix2(double Reference_D_tmp){

  // Release the velocity control temporarily.
  ZeroGain_V();

  // Set reference.
  Reference_D = Reference_D_tmp;

  /* Using outer CNT board. */
  if(AuxType == 1 || AuxType == 3 || AuxType == 4 || AuxType == 9 || AuxType == 10)
    if(CNTDeviceHandle != 0) EncoderCount = CNTDeviceHandle->Get(CNTChannel);

  /* Using outer AD board. */
  if(AuxType == 2 || AuxType == 3 || AuxType == 4)
    if(ADDeviceHandle != 0) PotentioValue = ADDeviceHandle->ADIn(ADChannel);

  /* Using NCord CNT. */
  if(AuxType == 5 || AuxType == 7 || AuxType == 8)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      EncoderCount = NCordHandle->Terminal[TerminalNumber_E].Counter_Value[EncoderCNTNumber];
    }

  /* Using NCord AD. */
  if(AuxType == 6 || AuxType == 7 || AuxType == 8 || AuxType == 9 || AuxType == 10)
    if(NCordHandle != 0) {
      //NCordHandle->Contact();
      PotentioValue = NCordHandle->Terminal[TerminalNumber_P].AD_Value[PotentioADNumber];
    }

  CalcPIDControl();

  // Reload the velocity control.
  ReloadGain_V();
}


/*
///-----[Contacting way]------------------------------------------------------
// 以下のプログラムはサンプルである．
// TexART_NCord システムのみを使う場合，モータアンプへの指令や，エンコーダやポテンショのデータ取得は，
// 制御ループ内において NCordHandle->ContactPeriodic() または NCordHandle->Contact() を1回実行することで済ませられる．
// それ以外を合わせて使う場合，すなわち一般的なインターフェイスも合わせて使う場合においては，
// 各ボードへの接続を，1ループあたり1回で済ませられる様に，記述しなくてはならない．
// そのサンプルとして，以下のプログラムを提示する．

  unsigned long *ArrayEncoderCount;
  double        *ArrayPotentioValue,
                *ArrayCommandVoltage;

  TexART_MotorCLK MothorCLK;
  double cycle_second = 0.1;

  ArrayEncoderCount   = new unsigned long[NumberOfMotors];
  ArrayPotentioValue  = new double[NumberOfMotors];
  ArrayCommandVoltage = new double[NumberOfMotors];

  // Get values of displacement sensors in one connection to each interface board.
  CNTDeviceHandle.Get(ArrayEncoderCount);
  ADDeviceHandle.ADIn(ArrayPotentioValue);

  // Allocating input data.
  for(i=0; i<NumberOfMotors; i++) {
    M[i].EncoderCount  = ArrayEncoderCount[i];
    M[i].PotentioValue = ArrayPotentioValue[i];
  }

  // Calcularing commands for PID control.
  for(i=0; i<NumberOfMotors; i++)
    M[i].CalcPIDControl();

  // Output CommandVoltage in one connection to the DA board.
  for(i=0; i<NumberOfMotors; i++)
    ArrayCommandVoltage[i] = M[i].CommandVoltage;
  DADeviceHandle.DAOut(ArrayCommandVoltage);

  // Contact to TexART_NCord system on periodic time.
  Robot.ContactPeriodic();

  // Waiting periodic, in case withount Robot.ContactPeriodic().
  MothorCLK.PeriodicWait(cycle_second);
*/





