
#include "stdafx.h"
#include "CFood.h"



namespace mray
{
namespace GameMod
{

CFood::CFood()
:Amount(mT("Amount"),100),Type(mT("Type"),EFT_Meat)
{

	m_entity=0;
// 	Amount=new rwProperty<CFood,float>(this,&CFood::SetAmount,&CFood::GetAmount,EPT_FLOAT,mT("Amount"));
// 	Type=new rwProperty<CFood,EFoodType>(this,&CFood::SetFoodType,&CFood::GetFoodType,EPT_INT,mT("Type"));
}
CFood::~CFood()
{
// 	delete Amount;
// 	delete Type;
}

void CFood::SetEntity(game::GameEntity* ent)
{
	m_entity=ent;
}
game::GameEntity* CFood::GetEntity()
{
	return m_entity;
}

void CFood::SetAmount(const float& v)
{
	Amount.value=v;
}
const float& CFood::GetAmount()
{
	return Amount.value;
}

void CFood::SetFoodType(const EFoodType& t)
{
	Type.value=t;
}
const EFoodType& CFood::GetFoodType()
{
	return (EFoodType)Type.value;
}



}
}

