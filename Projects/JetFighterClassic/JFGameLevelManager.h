


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   14:43
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFGameLevelManager.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFGameLevelManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFGameLevelManager__
#define __JFGameLevelManager__


namespace mray
{
namespace game
{

	class JFObject;
	class JFIBullet;

class JFGameLevelManager
{
private:
protected:
	std::list<JFObject*> m_objects;
	std::list<JFIBullet*> m_bullets;


	std::vector<JFObject*> m_toRemoveObjects;
	std::vector<JFIBullet*> m_toRemoveBullets;

	template<class T>
	void _ProcessRemoveRequests(std::vector<T*>& req,
		std::list<T*>& lst)
	{
		for(int i=0;i<req.size();++i)
		{
			std::list<T*>::iterator it2;
			for(std::list<T*>::iterator it= lst.begin();
				it!=lst.end();)
			{
				if(*it==req[i])
				{
					it2=it;
					it2++;
					delete *it;
					lst.erase(it);
					it=it2;
				}else
					++it;
			}
			req.clear();
		}
	}
	void _ProcessRemoveRequests();
public:
	JFGameLevelManager();
	virtual~JFGameLevelManager();

	void AddObject(JFObject* obj);
	void RemoveObject(JFObject* obj);

	void AddBullet(JFIBullet* b);
	void RemoveBullet(JFIBullet* b);

	void Update(float dt);

};

}
}

#endif
