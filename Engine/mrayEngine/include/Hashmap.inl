
#include "mraySystem.h"

namespace mray{
namespace core{


template<class TKey,class T>
HashMap<TKey,T>::HashMap(){
	m_map.resize(1024);
	for(int i=0;i<m_map.size();++i){
		m_map[i]=0;
	}
	m_size=0;
	userHashFunction=0;
}
template<class TKey,class T>
HashMap<TKey,T>::HashMap(uint size){
	m_map.resize(size);
	for(int i=0;i<m_map.size();++i){
		m_map[i]=0;
	}
	m_size=0;
	userHashFunction=0;
}
template<class TKey,class T>
HashMap<TKey,T>::~HashMap(){
	m_map.clear();
}

template<class TKey,class T>
int HashMap<TKey,T>::size(){
	return m_size;
}
template<class TKey,class T>
bool HashMap<TKey,T>::insert(const value_type&t){

	int i=HashFunction(t.first);

	ListNode<value_type>* node=m_map[i];
	if(!node){
		m_map[i]=new ListNode<value_type>(t);
		m_size++;
		return true;
	}
	while(node){
		if(node->Elem.first==t.first)
			return false;
		node=node->Next;
	}

	node=new ListNode<value_type>(t);
	m_map[i]->Prev=node;
	node->Next=m_map[i];
	m_map[i]=node;
	m_size++;
	return true;
}
template<class TKey,class T>
bool HashMap<TKey,T>::insert(const TKey&k,const T&v){
	return insert(value_type(k,v));
}


template<class TKey,class T>
bool HashMap<TKey,T>::remove(const TKey &k){
	int i=HashFunction(k);

	ListNode<value_type>* node=m_map[i];
	while(node){
		if(node->Elem.first==k){
			if(node==m_map[i]){
				m_map[i]=node->Next;
			}else {
				node->Prev=node->Next;
			}
			if(node->Next)
				node->Next->Prev=node->prev;
			delete node;
			m_size--;
			return true;
		}
		node=node->Next;
	}

	return false
}

template<class TKey,class T>
void HashMap<TKey,T>::clear(){
	for(int i=0;i<m_map.size();++i){
		ListNode<value_type>* node=m_map[i];
		while(node)
		{
			ListNode<value_type> *Next=node->Next;
			delete node;
			node =Next;
		}
	}
}


template<class TKey,class T>
int HashMap<TKey,T>::HashFunction(const TKey&k)const{

	if(userHashFunction){
		return userHashFunction(k);
	}
	
    // default hash function
    static double HashMultiplier = 0.5*(sqrt(5.0)-1.0);
    unsigned int uiKey;
    uiKey = k%m_map.size();
    double dFraction = fmod(HashMultiplier*uiKey,1.0);
    return (int)floor(m_map.size()*dFraction);
}


}
}

