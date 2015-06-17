

/********************************************************************
	created:	2014/01/18
	created:	18:1:2014   20:38
	filename: 	C:\Development\mrayEngine\Projects\TelubeeLib\OptiTrackHeadController.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeLib
	file base:	OptiTrackHeadController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OptiTrackHeadController__
#define __OptiTrackHeadController__


#include "IHeadController.h"

namespace mray
{
namespace TBee
{
class OptiTrackHeadController:public IHeadController
{
protected:
	int m_headID;
	std::list<math::vector3d> m_posAvg;
	std::list<math::vector3d> m_oriAvg;
public:
	OptiTrackHeadController(int headId);
	virtual~OptiTrackHeadController();

	void SetHeadID(int id){ m_headID = id; }


	virtual bool GetHeadOrientation(math::quaternion& q, bool abs);
	virtual bool GetHeadPosition(math::vector3d& v, bool abs);
};

}
}


#endif
