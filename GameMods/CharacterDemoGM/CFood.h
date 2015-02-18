

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   13:34
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\Food.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	Food
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CFood_h__
#define CFood_h__

#include "CompileConfig.h"
#include "GameEntity.h"
#include "GenericValues.h"

namespace mray
{
namespace GameMod
{

	enum EFoodType
	{
		EFT_Meat,
		EFT_Vegetable
	};

class CHARACTERDEMOGM_API CFood
{
private:
protected:
//	EFoodType m_type;
//	float m_amount;

	game::GameEntity* m_entity;
public:

	FloatValue Amount;
	IntValue Type;

// 	rwProperty<CFood,float> *Amount;
// 	rwProperty<CFood,EFoodType> *Type;

	CFood();
	virtual ~CFood();

	void SetEntity(game::GameEntity* ent);
	game::GameEntity* GetEntity();

	void SetAmount(const float& v);
	const float& GetAmount();

	void SetFoodType(const EFoodType& t);
	const EFoodType& GetFoodType();
	
};

}
}


#endif // Food_h__
