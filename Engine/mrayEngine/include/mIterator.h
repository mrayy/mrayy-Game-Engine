
/********************************************************************
	created:	2009/04/25
	created:	25:4:2009   19:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\mIterator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	mIterator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___mIterator___
#define ___mIterator___


namespace mray{
namespace core{

template <class T>
class ListNode
{
public:
	ListNode():Next(0),Prev(0){};
	ListNode(const T&v):Next(0),Prev(0),Elem(v){};
	ListNode *Next;
	ListNode *Prev;
	T Elem;
};

template <class T>
class mIterator
{
private:
protected:
	ListNode<T> *current;
public:
	mIterator(ListNode<T>*node):current(node){};
	mIterator():current(0){};
	virtual~mIterator(){}

	mIterator& operator++(){current=current->Next;return *this;}
	mIterator& operator--(){current=current->Prev;return *this;}
	mIterator operator++(int){mIterator tmp=*this;current=current->Next;return tmp;}
	mIterator operator--(int){mIterator tmp=*this;current=current->Prev;return tmp;}

	mIterator operator+(int offset)const{
		mIterator tmp=*this;
		if(offset>=0)
			while(offset-- && tmp.current!=0)++tmp;
		else
			while(offset++ && tmp.current!=0)--tmp;
		return tmp;
	}
	mIterator&operator+=(int offset){
		if(offset>=0)
			while(offset-- && offset!=0)++(*this);
		else
			while(offset++ && offset!=0)--(*this);
		return (*this);
	}
	mIterator operator-(int offset)const{return *(this)+(-offset);}
	mIterator&operator-=(int offset){*(this)+=(-offset);return *(this);}

	bool operator==(const mIterator &other)const{return current==other.current;}
	bool operator!=(const mIterator &other)const{return current!=other.current;}

	ListNode<T> *getCurrent()const {return current;}

	T&operator*(){return current->Elem;}
	const T&operator*()const {return current->Elem;}
};

}
}


#endif //___mIterator___
