#ifndef ___ICycles___
#define ___ICycles___



namespace mray{
namespace scene{


class SCycle
{

public:
	SCycle():totalCycles(0),
		CurrCycle(0)
	{}
	virtual~SCycle()
	{
		clear();
	}
	void clear()
	{
		for(int i=0;i<CycleProp.size();++i)
			CycleProp[i].cycleIndex.clear();
		CycleProp.clear();
		
	}
	SCycle*  getDuplicate(){
		SCycle*  dup=new SCycle();
		dup->totalCycles=totalCycles;
		dup->startFrame=startFrame;
		dup->endFrame=endFrame;
		dup->CurrCycle=CurrCycle;
/*		dup->CycleProp.resize(CycleProp.size());
		for(int i=0;i<CycleProp.size();++i){
			dup->CycleProp[i].count=CycleProp[i].count;
			dup->CycleProp[i].start=CycleProp[i].start;
			dup->CycleProp[i].end=CycleProp[i].end;
			dup->CycleProp[i].name=CycleProp[i].name;
			dup->CycleProp[i].cycleIndex.resize(CycleProp[i].cycleIndex.size());
			for(int j=0;j<CycleProp[i].cycleIndex.size();j++){
				dup->CycleProp[i].cycleIndex[j]=CycleProp[i].cycleIndex[j];
			}
		}*/

		return dup;
	}


	int totalCycles;
	struct SCycleProp
	{
		int count;
		int start;
		int end;
		std::vector<int> cycleIndex;
		core::string name;
	};


	std::vector<SCycleProp>CycleProp;

	float CurrFrame;
	float blendAmount;
	int startFrame;
	int endFrame;
	int blendFrame;
	int CurrCycle;
	int totalLocators;
};


};//scene
};//mray


#endif



