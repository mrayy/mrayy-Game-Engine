/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   19:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TPair.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TPair
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TPair___
#define ___TPair___


namespace mray{
namespace core{

template <class A,class B>
class TPair
{
private:
protected:
public:
	TPair(const A &a,const B&b){
		first=a;
		second=b;
	}
	virtual~TPair(){
	}

	A first;
	B second;

};



}
}

#endif //___TPair___

