

#ifndef ___CKeyableAttr___
#define ___CKeyableAttr___



namespace mray{
namespace core{

template <class T>
class CKeyableAttr{
public:

	struct SKey
	{
		SKey(){
		}
		SKey(float t,const T& v):time(t),value(v)
		{
		}
		float time;
		T value;
		bool operator <(const SKey&o)const{
			return time<o.time;
		}
	};

	CKeyableAttr(){
	}
	virtual~CKeyableAttr(){
		clear();
	}

	int getKeysCount()const
	{
		return m_Keys.size();
	}

	void addKey(float t,const T& v)
	{
		m_Keys.push_back(SKey(t,v));
		std::sort(m_Keys.begin(),m_Keys.end());
		//m_Keys.sort();
	}
	void removeKey(int index){
		if(index>=m_Keys.size())
			return;
		std::vector<SKey>::iterator it=m_Keys.begin();
		std::advance(it,index);
		m_Keys.erase(it);
	}

	const SKey* getKeyAtIndex(int i)const
	{
		if(i<m_Keys.size())
			return &m_Keys[i];
		return 0;
	}

	void clear(){
		m_Keys.clear();
	}

	T getInterpolatedKey(float time)const
	{
		int last=-1;
		for(int i=0;i<m_Keys.size();++i){
			if(time<=m_Keys[i].time){
				if(last==-1)return m_Keys[i].value;
				
				float r=(time-m_Keys[last].time)/(m_Keys[i].time-m_Keys[last].time);
				return math::lerp(m_Keys[last].value,m_Keys[i].value,r);
			}else{
				last=i;
			}
		}
		if(m_Keys.size()>0)
			return m_Keys[m_Keys.size()-1].value;
		return T(0);
	}

private:
	
	std::vector<SKey> m_Keys;

};

}
}





#endif




