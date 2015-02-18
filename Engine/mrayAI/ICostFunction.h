
/********************************************************************
	created:	2010/03/10
	created:	10:3:2010   12:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ICostFunction.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ICostFunction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICostFunction___
#define ___ICostFunction___


namespace mray{
namespace AI{

	class IGraphNode;
	class IGraphLink;
class ICostFunction
{
private:
protected:
public:

	virtual float getHeuristic(IGraphNode*a,IGraphNode*b)=0;
	virtual float getWeight(IGraphLink*link)=0;
};

}
}


#endif //___ICostFunction___
