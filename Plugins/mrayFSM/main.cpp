

#include "StateMachine.h"
#include "InputTypes.h"
#include "ConditionCollection.h"

using namespace mray;

class TestState:public IState
{
public:
	TestState(const core::string&name):IState(name){}
	virtual~TestState(){}

	virtual void onEnter(IState*prev){

		printf("%s-onEnter()\n",m_name.c_str());
	}
	virtual void onUpdate(){
		printf("%s-onUpdate()\n",m_name.c_str());
	}
	virtual void onExit(){
		printf("%s-onExit()\n",m_name.c_str());
	}
};

int v1=0;
int v2=0;
void*getValueV1(){
	return &v1;
}

class TestInputClass
{
public:

	void*getValue(){
		return &v1;
	}
};

class Vector{
public:
	int x,y;
	Vector(int a,int b){
		x=a;
		y=b;
	}

	bool operator ==(const int& o)const{
		return x==o && y==o;
	}
	bool operator <(const int& o)const{
		return x<o && y<o;
	}
};

int main(){

	StateMachine*machine=new StateMachine();
	machine->addState(new TestState(mT("Idle")));
	machine->addState(new TestState(mT("Attack")));
	machine->addState(new TestState(mT("Run")));

	TestInputClass*c1=new TestInputClass();

	Vector vv(5,5);

	ICondition*cond1=0;
	AndCondition *andCondition=new AndCondition(mT("And1"));
	cond1=new TemplateCompareCondition<Vector,int>	(mT("Cond1"),new TemplateInputType<Vector>(&vv)	,ECT_Greater,new classFunctionInputType<TestInputClass,int>(c1,&TestInputClass::getValue));
	andCondition->addCondition(cond1);

	cond1=new TemplateCompareCondition<int,int>		(mT("Cond2"),new TemplateInputType<int>(&v1)	,ECT_Greater,new TemplateInputType<int>(&v2));
	andCondition->addCondition(cond1);

	machine->addCondition(andCondition);
	machine->addCondition(new TemplateCompareCondition<Vector,int>	(mT("Cond_2"),new TemplateInputType<Vector>(&vv)	,ECT_Less	,new FunctionInputType<int>(&getValueV1)));

	machine->addTransition(mT("Idle"),mT("Attack"),mT("Cond1"));

	machine->addTransition(mT("Attack"),mT("Idle"),mT("Cond2"));
	machine->addTransition(mT("Attack"),mT("Run"),mT("Cond1"));

	machine->addTransition(mT("Run"),mT("Idle"),mT("Cond1"));

	while(1){
		printf("V1: ");
		scanf_s("%d",&v1);
		printf("V2(x,y): ");
		scanf_s("%d,%d",&vv.x,&vv.y);
		if(v1==0)
			break;
		machine->onUpdate();
	}

	return 0;

}
