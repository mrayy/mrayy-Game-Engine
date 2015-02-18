#include "stdafx.h"

#include "FuzzySystemGA.h"

#include "FuzzyModule.h"
#include "FuzzyAndOpt.h"
#include "FuzzySetProxy.h"


#include "GAModule.h"
#include "GAChromOperation.h"
#include "GAPopulation.h"
#include "GACrossover.h"
#include "GAMutation.h"
#include "GAFitnessComparator.h"
#include "GAChromosomeSelector.h"
#include "GASetChromosome.h"
#include "GAMultiLimitsValues.h"

#include "Randomizer.h"
#include "mmath.h"

namespace mray
{
namespace AI
{


class GAFuzzyRuleChromGenerator:public GAChromosomeGenerator
{

public:

	int m_rulesCount;
	int m_varsCount;
	int m_indCount;
	uint* m_indString;

	int m_maxInd;

	GAAlgorithmParameters* m_params;

	GAFuzzyRuleChromGenerator(GAAlgorithmParameters*p):m_rulesCount(0),m_varsCount(0),m_indCount(0),m_indString(0),m_params(p)
	{
	}
	virtual~GAFuzzyRuleChromGenerator()
	{
		delete [] m_indString;
	}


	void Init(int ruleCnt,int vars,int attrs)
	{

		m_rulesCount=ruleCnt;
		m_varsCount=vars;
		m_maxInd=attrs;
		m_indCount=m_rulesCount*m_varsCount*2;

		delete [] m_indString;
		m_indString=new uint[m_indCount];
	}

	int GetMembersCount()
	{
		return m_indCount;
	}

	virtual GAChromosome* GenerateChromosome()
	{
		for(int i=0;i<m_indCount;i+=2)
		{
			m_indString[i+0]=math::Randomizer::rand(3);
			m_indString[i+1]=math::Randomizer::rand(m_maxInd);
		}

		return new GASetChromosome(m_indString,m_indCount,m_maxInd,m_params);
	}
};

class GAFuzzyRuleChromFitness:public GAFitnessEvaluator
{
	std::vector<float> m_fitness;
public:
	GAFuzzyRuleChromFitness()
	{
	}
	void SetChromsCount(int c)
	{
		m_fitness.resize(c);
	}
	void SetFitness(int id,float v)
	{
		m_fitness[id]=v;
	}
	virtual float Evaluate(GAChromosome* c)
	{
		return m_fitness[c->GetID()];
	}
};

FuzzySystemGA::FuzzySystemGA():m_numberOfRules(0),m_fuzzyModule(0),m_GAAlgo(0),m_eval(0),m_gen(0),m_populationSize(50)
{
	m_GAAlgo=new GAModule();
	m_crossoverOP=new GAMultiPointCrossover(2);
	m_mutationOP=new GAFlipMutation(0.03);
	m_comp=new GAMaxFitnessComparator();
	m_selector=new GABestFitChromSelector(m_comp);
	m_gen=new GAFuzzyRuleChromGenerator(m_GAAlgo->GetParams());
	m_eval=new GAFuzzyRuleChromFitness();

	m_limitValues=new GAMultiLimitsValues<uint>();

}
FuzzySystemGA::~FuzzySystemGA()
{
	delete m_GAAlgo;
	delete m_crossoverOP;
	delete m_mutationOP;

	delete m_comp;
	delete m_gen;
	delete m_selector;
	delete m_eval;
	delete m_limitValues;
}


int FuzzySystemGA::GetVarID(const core::string&v)
{
	for(int i=0;i<m_varsNames.size();++i)
	{
		if(m_varsNames[i]==v)
			return i;
	}
	return -1;
}

void FuzzySystemGA::SetFitnessValue(int chrom,float fitness)
{
	((GAFuzzyRuleChromFitness*)m_eval)->SetFitness(chrom,fitness); 
}

void FuzzySystemGA::SetNumberOfRules(int c)
{
	m_numberOfRules=c;
}

void FuzzySystemGA::SetPopulationSize(int sz)
{
	m_populationSize=sz;
}
int  FuzzySystemGA::GetPopulationSize()
{
	return m_populationSize;
}

void FuzzySystemGA::SetFuzzyModule(FuzzyModule*module)
{
	m_fuzzyModule=module;
	if(!m_fuzzyModule)
		return;

	const FuzzyVariableMap& vars=m_fuzzyModule->GetVariables();
	FuzzyVariableMap::const_iterator it=vars.begin();
	for(;it!=vars.end();++it)
	{
		m_varsNames.push_back(it->second->GetName());
	}
}
/*
void FuzzySystemGA::SetFitnessEvaluator(GAFitnessEvaluator*eval)
{
	m_eval=eval;
}*/

GAPopulation* FuzzySystemGA::Initialize(const std::vector<core::string>& input,const std::vector<core::string> &output)
{
	if(/*m_numberOfRules==0 ||*/ !m_fuzzyModule)
		return 0;

	int maxAttrsCnt=0;

	GAAlgorithmParameters params;
	params.crossoverOp=m_crossoverOP;
	params.mutationOp=m_mutationOP;
	params.fitnessEval=m_eval;
	params.chromSelector=m_selector;
	params.chromosomeGen=m_gen;
	params.populationSize=m_populationSize;
	params.selectedParentsCnt=4;
	params.chromValues=m_limitValues;

	m_inputs.clear();
	m_outputs.clear();

	m_numberOfRules=1;

	std::vector<int> current_limit(input.size());
	

	for(int i=0;i<input.size();++i)
	{
		FuzzyVariable*var= m_fuzzyModule->GetVariable(input[i]);
		//int id=GetVarID(input[i]);
		if(!var)
			return 0;
		//inID.push_back(id);
		m_inputs.push_back(var);
		maxAttrsCnt=math::Max(maxAttrsCnt,var->GetSetsCount());
		current_limit[i]=0;
		m_numberOfRules*=var->GetSetsCount();
	}
	for(int i=0;i<output.size();++i)
	{
		FuzzyVariable*var= m_fuzzyModule->GetVariable(output[i]);
		if(!var)
			return 0;
		m_outputs.push_back(var);
		maxAttrsCnt=math::Max(maxAttrsCnt,var->GetSetsCount());
	}

	((GAFuzzyRuleChromGenerator*)m_gen)->Init(m_numberOfRules,input.size()+output.size(),maxAttrsCnt);
	((GAFuzzyRuleChromFitness*)m_eval)->SetChromsCount(m_populationSize);
	m_limitValues->SetMembersCount(((GAFuzzyRuleChromGenerator*)m_gen)->GetMembersCount());

	//set limit values
	int member=0;
	for (int r=0;r<m_numberOfRules;++r)
	{
		for (int i=0;i<m_inputs.size();++i)
		{
			if(current_limit[i]==m_inputs[i]->GetSetsCount())
			{
				if(i!=m_inputs.size()-1)
					current_limit[i+1]+=1;	
				current_limit[i]=0;
			}
			m_limitValues->SetLimits(member+0,0,3);
			m_limitValues->SetLimits(member+1,current_limit[i],current_limit[i]);//0,m_inputs[i]->GetSetsCount()-1);
			if(i==0)
				current_limit[i]++;
			member+=2;
		}
		for (int i=0;i<m_outputs.size();++i)
		{
			m_limitValues->SetLimits(member+0,0,3);
			m_limitValues->SetLimits(member+1,0,m_outputs[i]->GetSetsCount()-1);
			member+=2;
		}
	}

	return m_GAAlgo->Initialize(params);
}
GAPopulation* FuzzySystemGA::RunEpoch()
{
	return m_GAAlgo->RunEpoch();
}
GAModule* FuzzySystemGA::GetGAModule()
{
	return m_GAAlgo;
}


bool FuzzySystemGA::ConvertToRules(GAChromosome*chrom,std::vector<FuzzyRule*>&rules)
{
	rules.clear();
	GASetChromosome*c=(GASetChromosome*)chrom;
	GAFuzzyRuleChromGenerator*gen=(GAFuzzyRuleChromGenerator*) m_gen;
	int n=0;
	for(int i=0;i<gen->m_rulesCount;++i)
	{
		FuzzyAndOpt *ant=new FuzzyAndOpt();
		for(int j=0;j<m_inputs.size();++j)
		{
			int hedge=c->GetAt(n);
			int set=c->GetAt(n+1);
			n+=2;

			FuzzyVariable*v=m_inputs[j];

			//only accept hedge values : 0,1,2
			if(hedge==3 || set>=v->GetSetsCount())
			{
				delete ant;
				return 0;
			}
			ant->AddTerm(new FuzzySetProxy(v->GetName(),v->GetSet(set)));
		}

		FuzzyAndOpt *con=new FuzzyAndOpt();
		for(int j=0;j<m_outputs.size();++j)
		{
			int hedge=c->GetAt(n);
			int set=c->GetAt(n+1);
			n+=2;

			FuzzyVariable*v=m_outputs[j];

			//only accept hedge values : 0,1,2
			if(hedge==3 || set>=v->GetSetsCount())
			{
				delete ant;
				delete con;
				return 0;
			}
			con->AddTerm(new FuzzySetProxy(v->GetName(),v->GetSet(set)));
		}

		rules.push_back(new FuzzyRule(ant,con));

	}

	return 1;
}

}
}
