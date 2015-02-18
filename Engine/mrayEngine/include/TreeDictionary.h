

/********************************************************************
	created:	2009/05/31
	created:	31:5:2009   19:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\TreeDictionary.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	TreeDictionary
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TreeDictionary___
#define ___TreeDictionary___

#include "mraySystem.h"
#include "CMemory.h"

namespace mray{
namespace core{

template <class T,class V>
class TreeDictionary
{
private:
protected:

	static const int m_tableSize=1<<(sizeof(T)*8);

	struct SNode
	{
	public:
		V value;

		SNode** children;

		SNode(){
			value=(V)0;
			children=0;
		}
		SNode(V v){
			value=v;
			children=0;
		}
		~SNode(){
			if(children){
				for (int i=0;i<m_tableSize;i++)
				{
					delete children[i];
				}
				delete [] children;
			}
		}

		void alloc(){
			if(!children){
				children=new SNode*[m_tableSize];
				mraySystem::memSet(children,0,sizeof(SNode*)*m_tableSize);
			}
		}

		void addNode(T n,V v){
			alloc();
			if(!children[n])
				children[n]=new SNode(v);
			else{
				children[n]->addNode(n,v);
			}
		}
		void addNode(T* n,int len,V v){
			if(len==0)
				return;
			alloc();
			if(!children[n[0]])
				children[n[0]]=new SNode(v);
			if(len==1)
				return;
			children[n[0]]->addNode(&n[1],len-1,v);
		}

		V* getValue(T* n,int len){
			if(!children)
				return 0;
			if(len>1)
				return children[n[0]]->getValue(&n[1],len-1);
			if (len==1)
			{
				return &children[n[0]]->value;
			}
			return 0;
		}
	};
	SNode** m_nodes;
public:
	TreeDictionary(){
		m_nodes=new SNode*[m_tableSize];
		mraySystem::memSet(m_nodes,0,sizeof(SNode*)*m_tableSize);
	}
	virtual~TreeDictionary(){
		for (int i=0;i<m_tableSize;i++)
		{
			delete m_nodes[i];
		}
		delete [] m_nodes;
	}
	void clear(){
		for (int i=0;i<m_tableSize;i++)
		{
			delete m_nodes[i];
			m_nodes[i]=0;
		}
	}
	void Add(T n,V v){
		if(!m_nodes[n])
			m_nodes[n]=new SNode(v);
		else{
			m_nodes[n]->value=v;
		}
	}
	void Add(T* n,int len,V v){
		if(len==0)
			return;
		if(!m_nodes[n[0]])
			m_nodes[n[0]]=new SNode(v);
		if(len==1)
			return;

		m_nodes[n[0]]->addNode(&n[1],len-1,v);
	}

	// n is an array of T
	V* getValue(T* n,int len){
		if(len>1)
			return m_nodes[n[0]]->getValue(&n[1],len-1);
		if (len==1)
		{
			return &m_nodes[n[0]]->value;
		}
		return 0;
	}

};

}
}


#endif //___TreeDictionary___
