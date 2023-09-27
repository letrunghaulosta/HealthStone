#include "GameController.hpp"
#include <iostream>
#include <functional>

GameController* GameController::gcInstance = new GameController();
std::vector<bool> UnitManager::unitIdContainer(true);

void GameController::Start()
{
   heroManager.push_back(new HeroManager());
   heroManager[GC_PLAYER_1]->Generate();
   heroManager.push_back(new HeroManager());
   heroManager[GC_PLAYER_2]->Generate();
   
   cardManager.push_back(new CardManager());
   cardManager[GC_PLAYER_1]->Generate();
   unitOnHand.push_back(cardManager[GC_PLAYER_1]->GetCardIdList());

   cardManager.push_back(new CardManager());
   cardManager[GC_PLAYER_2]->Generate();
   unitOnHand.push_back(cardManager[GC_PLAYER_2]->GetCardIdList());
}

UnitManager* GameController::GetUnitInformation(uint8_t id,Unit::UnitInfoType* unitInfo)
{
   for(auto hero = heroManager.begin(); hero != heroManager.end();hero++)
   {
      if(true == (*hero)->GetInformationById(id,unitInfo))
      {
         return *hero;
      }
   }
   
   for(auto card = cardManager.begin(); card != cardManager.end();card++)
   {
      if(true == (*card)->GetInformationById(id,unitInfo))
      {
         return *card;
      }
   }

   return NULL;
}

void GameController::Attack(uint8_t attackerId, uint8_t beAttackedId)
{
   
   Unit::UnitInfoType attackerInfo;
   Unit::UnitInfoType beAttackedInfo;
   UnitManager* attackerManager = GetUnitInformation(attackerId,&attackerInfo);
   UnitManager* beAttackedManager = GetUnitInformation(beAttackedId,&beAttackedInfo);

   uint8_t damage = attackerInfo.damage;
   std::cout << "attackerId" << (int)attackerId << std::endl;
   std::cout << "attackerInfoName " << attackerInfo.name << std::endl;
   std::cout << "attackerIdDamage" << attackerInfo.damage << std::endl;
   switch(beAttackedInfo.type)
   {
      case Unit::UNIT_TYPE_HERO:
         beAttackedManager->OnAttacked(beAttackedId, damage, std::bind(&GameController::RemoveUnitById,this, std::placeholders::_1));
         break;
      case Unit::UNIT_TYPE_MINION:
         break;
   }
}

std::vector<uint8_t> GameController::GetHeroIdList(uint8_t player)
{
   return heroManager[player]->GetHeroIdList();
}

void GameController::RemoveUnitById(uint8_t id)
{
   std::cout << "REMOVED UNIIT" << std::endl;
   Unit::UnitInfoType unitInfo;
   UnitManager* unitManager = GetUnitInformation(id,&unitInfo);

   unitInfo.type == Unit::UNIT_TYPE_HERO ?
   static_cast<HeroManager*>(unitManager)->DestroyById(id):
   (void)NULL;

}
