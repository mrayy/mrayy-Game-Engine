

/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\IDefuzzifyOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	IDefuzzifyOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDefuzzifyOperation___
#define ___IDefuzzifyOperation___


namespace mray{
namespace AI{
	class FuzzyVariable;

class IDefuzzifyOperation
{
private:
protected:
public:
	IDefuzzifyOperation(){}
	virtual~IDefuzzifyOperation(){}

	virtual float Defuzzify(FuzzyVariable*var)=0;

};

}
}

#endif //___IDefuzzifyOperation___
