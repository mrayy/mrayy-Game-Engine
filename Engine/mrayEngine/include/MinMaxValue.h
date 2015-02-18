


/********************************************************************
	created:	2010/10/22
	created:	22:10:2010   16:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MinMaxValue.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MinMaxValue
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MinMaxValue_h__
#define MinMaxValue_h__



namespace mray
{
namespace core
{

	template<class T>
class MinMaxValue
{
private:
protected:
	T m_min,m_max;
public:
	MinMaxValue(const T&minV,const T&maxV):m_min(minV),m_max(maxV)
	{
	}
	virtual ~MinMaxValue()
	{
	}

	T operator()const
	{
		float l=math::Randomizer::rand01();
		return m_min*l+m_max*(1-l);
	}

};

}
}

#endif // MinMaxValue_h__
