
/********************************************************************
	created:	2010/03/25
	created:	25:3:2010   14:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\SemanticTableVariable.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	SemanticTableVariable
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SemanticTableVariable___
#define ___SemanticTableVariable___



namespace mray{
namespace video{

template <class T>
class SemanticTableVariable
{
private:
protected:
	typedef std::vector<T> VariableArray;
	VariableArray m_vars;
public:
	SemanticTableVariable(){}
	virtual~SemanticTableVariable(){}

	T*operator[](int idx)
	{
		return GetVariable(idx);
	}

	void AddVariableBack(const T&v)
	{
		m_vars.push_back(v);
	}

	void AddVariableFront(const T&v)
	{
		if(m_vars.size())
		{
			m_vars.insert(m_vars.end()-1,v);
		}else
			m_vars.push_back(v);
	}

	void SetVariable(int idx,const T&v)
	{
		if(idx>=m_vars.size())
			m_vars.resize(idx+1);
		m_vars[idx]=v;
	}
	T* GetVariable(int idx)
	{
		if(idx>=m_vars.size())
			return 0;
		return &m_vars[idx];
	}
	int GetVariablesCount()
	{
		return m_vars.size();
	}
	void Clear()
	{
		m_vars.resize(0);
	}

};

}
}


#endif //___SemanticTableVariable___
