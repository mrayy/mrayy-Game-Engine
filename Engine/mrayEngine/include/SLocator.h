
#ifndef ___SLocator___
#define ___SLocator___
#include "ISceneNode.h"

#include "mString.h"


#include "GCPtr.h"

namespace mray{
namespace scene{

class MRAY_DLL SLocator:public ISceneNode
{
	
public:
	class MRAY_DLL FramesAttr{
	public:
		std::vector<math::vector3d> position;
		std::vector<math::quaternion> orintation;
	//	std::vector<math::vector3d> rotate;
		std::vector<math::vector3d> scale;

		void clear(){
			position.clear();
			orintation.clear();
			scale.clear();
		}
		virtual ~FramesAttr(){
			clear();
		}
	};
	GCPtr<FramesAttr>frames;
	SLocator(const  mchar* name=mT(""),IMovable* parent=0);
	virtual ~SLocator(){
	}
	bool getFramePos(float frame,int sframe,int eframe,math::vector3d &outPos);
	bool getFrameOrintation(float frame,int sframe,int eframe,math::quaternion &outOrint);
	bool getFrameScale(float frame,int sframe,int eframe,math::vector3d &outScale);
	
	bool getFramePos(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
		math::vector3d &outPos);
	bool getFrameOrintation(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
		math::quaternion &outOrint);
	bool getFrameScale(float frame,int sframe,int eframe,float blendAmount,int blendFrame,
		math::vector3d &outScale);

	void setCurrPos(float frame,int sframe,int eframe);
	void setCurrOrintation(float frame,int sframe,int eframe);
	void setCurrScale(float frame,int sframe,int eframe);

	void setCurrPos(float frame,int sframe,int eframe,float blendAmount,int blendFrame);
	void setCurrOrintation(float frame,int sframe,int eframe,float blendAmount,int blendFrame);
	void setCurrScale(float frame,int sframe,int eframe,float blendAmount,int blendFrame);

	virtual void setCurrFrame(float frame,int sframe,int eframe);
	virtual void setCurrFrame(float frame,int sframe,int eframe,float blendAmount,int blendFrame);

	virtual const math::matrix4x4&getRelativeTransformation()const;
	virtual void updateAbsoluteTransformation();

	void getFrames(FramesAttr*other){
		frames=other;
	}
	
	virtual void render(IRenderPass*pass);
//	virtual void preRender();
//	virtual void update();

	GCPtr<SLocator>getDuplicate();
};

}
}


#endif