


#ifndef ___GUIHistogram___
#define ___GUIHistogram___


#include "GUIElement.h"
#include "SColor.h"



namespace mray{
namespace GUI{

class MRAY_DLL GUIHistogram:public GUIElement
{
	float m_maxVal;
	int m_maxSamples;

	std::vector<float> samples;

	double samplesSum;
	float average;
	int samplesCnt;
	float sampleInterval;
	float m_tm;
	FontAttributes m_FontAttributs;
	
	void fixSamples();

	virtual float getSample()=0;
	virtual void fillProperties();


	const int& getMaxSamples() { return m_maxSamples; }
	void setMaxSamples(const int &val) { m_maxSamples = val; }
public:
	DECLARE_RTTI;

	rwProperty<GUIHistogram,int> MaxSamples;

	GUIHistogram(GUIManager* manager,const  mchar*Caption,int maxSamples,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent);
	virtual ~GUIHistogram();

	void addSample(float val);

	void setSampleInterval(float interval);

	virtual void draw(float dt);

	float getMaxVal(){return m_maxVal;}
	float getAverage(){return average;}

	virtual FontAttributes* getFontAttributes();
};

}
}


#endif


