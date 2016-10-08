

#ifndef __ILERPFUNCTION__
#define __ILERPFUNCTION__



namespace mray
{
namespace math
{

	//base class for advance lerp functions
	template<class T>
class ILerpFunction
{
protected:

public:
	ILerpFunction(){}
	virtual ~ILerpFunction(){}

	virtual void SetInitialValue(const T& v) = 0;
	virtual void SetGoalValue(const T& v) = 0;
	virtual bool IsDone(float err = 0.001f)=0;

	virtual const T& GetValue()const = 0;

	virtual const T& Update(float dt) = 0;
};

}
}


#endif
