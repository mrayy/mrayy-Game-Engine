

/********************************************************************
	created:	2010/06/29
	created:	29:6:2010   16:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDGenerator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDGenerator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IDGenerator_h__
#define IDGenerator_h__



namespace mray
{

template <class T>
class IDGenerator
{
private:
protected:
	typedef std::map<uint,T> IDObjectMap;

	IDObjectMap m_objects;
	uint m_lastID;
public:
	IDGenerator():m_lastID(0)
	{
	}
	virtual ~IDGenerator()
	{
		Clear();
	}

	uint GetNextID()
	{
		return m_lastID+1;
	}

	//return the id been associated to the object,in success it will return the same id been passed
	uint AddObject(const T&obj,uint id)
	{
		IDObjectMap::iterator it=m_objects.find(id);
		if(it!=m_objects.end())
		{
			id=m_lastID+1;
		}
		m_objects[id]=obj;

		m_lastID=math::Max(m_lastID,id);

		return id;
	}
	void RemoveObject(uint id)
	{
		m_objects.erase(id);
	}

	bool GetObjectByID(uint id,T&out)
	{
		IDObjectMap::iterator it=m_objects.find(id);
		if(it!=m_objects.end()){
			out=it->second;
			return true;
		}
		return false;
	}

	void Clear()
	{
		m_objects.clear();
		m_lastID=0;
	}
	
};

}
#endif // IDGenerator_h__
