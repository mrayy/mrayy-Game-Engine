
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   18:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\HashMap.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	HashMap
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HashMap___
#define ___HashMap___


#include "CompileConfig.h"
#include "CMemory.h"
#include "mIterator.h"
#include "TPair.h"

namespace mray{
namespace core{

template<class TKey,class T>
class HashMap
{
public:


	template <class IP>
	class mapIterator;

	typedef TPair<TKey,T> value_type;
	typedef typename mapIterator<value_type> iterator ;

private:
protected:
	int HashFunction(const TKey&k)const;

	typedef std::vector<ListNode<value_type>*> MapArray;
	MapArray m_map;

	int m_size;
public:

	template <class IP>
	class mapIterator
	{
	private:
	protected:
		ListNode<IP> *current;
		MapArray* m_map;
		int m_row;
	public:
		mapIterator(ListNode<IP>*node,int row,MapArray* map):current(node),m_map(map),m_row(row)
		{};
		mapIterator():current(0),m_map(0),m_row(0){};
		virtual~mapIterator(){}

		mapIterator& operator++(){
			current=current->Next;
			if(!current && m_map){
				while(!current && m_row<m_map->size()){
					m_row++;
					current=(*m_map)[m_row];
				}
			}
			return *this;
		}
		mapIterator operator++(int){mapIterator tmp=*this;++(*this);return tmp;}

		mapIterator operator+(int offset)const{
			mapIterator tmp=*this;
			if(offset>=0)
				while(offset-- && tmp.current!=0)++tmp;
			else
				while(offset++ && tmp.current!=0)--tmp;
			return tmp;
		}
		mapIterator&operator+=(int offset){
			if(offset>=0)
				while(offset-- && offset!=0)++(*this);
			else
				while(offset++ && offset!=0)--(*this);
			return (*this);
		}
		bool operator==(const mapIterator &other)const{return current==other.current;}
		bool operator!=(const mapIterator &other)const{return current!=other.current;}

		T&operator*(){return current->Elem;}
	};

	int (*userHashFunction)(const TKey&k);


	HashMap();
	HashMap(uint size);
	virtual~HashMap();

	bool insert(const value_type&t);
	bool insert(const TKey&k,const T&v);
	iterator find(const TKey &k){
		int i=HashFunction(k);

		ListNode<value_type>* node=m_map[i];
		while(node){
			if(node->Elem.first==k)
				break;
			node=node->Next;
		}

		return iterator(node,i,&m_map);
	}
	bool remove(const TKey &k);

	void clear();

	int size();

	iterator begin(){
		return iterator(m_map[0],0,&m_map);
	}
	iterator end(){
		return iterator();
	}

};

}
}

#include "Hashmap.inl"


#endif //___HashMap___
