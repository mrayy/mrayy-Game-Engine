/********************************************************************
	created:	2010/03/21
	created:	21:3:2010   0:12
	filename: 	d:\Character Animation\Solution\Project\Controlling\Controller.h
	file path:	d:\Character Animation\Solution\Project\Controlling
	file base:	Controller
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Controller_h__
#define Controller_h__

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class Character;
		class ControllingParams;

		class Controller
		{
		public:
			Controller(){};
			virtual~Controller(){};
			virtual void load(const core::string& descFile, Character* character)=0;
			virtual void setControllingParams(ControllingParams* params)=0;
			virtual void reset()=0;
			virtual void stop()=0;
			virtual bool isFinished()=0;
			virtual void update(float dt)=0;
			virtual const core::string& getName()=0;
		};
	}
}
#endif // Controller_h__