

#ifndef ___SAnimatedSprite___
#define ___SAnimatedSprite___

#include "MeshRenderableNode.h"

namespace mray{
namespace scene{

class MRAY_DLL SAnimatedSprite:public MeshRenderableNode
{
	float frame;
	int rows;
	int cols;
	float speed;
	int maxFrames;
	bool autoAnimate;
	void StartUp();
public:
	SAnimatedSprite(const video::ITexturePtr& tex,int rowCnt,int colCnt);
	virtual ~SAnimatedSprite();

	void setTexture(const video::ITexturePtr&   tex);
	video::ITexture *getTexture();

	void setRowsCount(int rowCnt);
	int getRowsCount();
	void setColsCount(int colCnt);
	int getColsCount();

	void setFrame(float f);
	float getFrame();

	void setAutoAnimate(bool a);
	bool getAutoAnimate();

	void setSpeed(float s);
	float getSpeed();

	virtual void update(float dt);
	
	void setNumberOfFrames(int n);
	int getNumberOfFrames();
};

}
}


#endif









