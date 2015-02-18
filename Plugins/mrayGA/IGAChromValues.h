

/********************************************************************
	created:	2010/05/12
	created:	12:5:2010   15:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\IGAChromValues.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	IGAChromValues
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGAChromValues_h__
#define IGAChromValues_h__


namespace mray
{
namespace AI
{

	class GAChromosome;

template <class T>
class IGAChromValues
{
private:
protected:
public:
	IGAChromValues(){}
	virtual ~IGAChromValues(){}

	//check if it's ok to change the [member] value to the new value [val] and return the new allowed value
	virtual const T& GetNearestVal(GAChromosome*chrom,uint member,const T& val)=0;
	
};

}
}

#endif // IGAChromValues_h__
