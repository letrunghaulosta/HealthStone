#include "GameController.hpp"
#include <iostream>

GameController* GameController::gcInstance = new GameController();
std::vector<bool> UnitManager::unitIdContainer(true);

void GameController::Start()
{
   heroManager.push_back(new HeroManager());
   heroManager[GC_PLAYER_1]->Generate();
   heroManager.push_back(new HeroManager());
   heroManager[GC_PLAYER_2]->Generate();
}

UnitManager* GameController::GetUnitInformation(uint8_t id,UnitInfoType* unitInfo)
{
   for(auto hero = heroManager.begin(); hero != heroManager.end();hero++)
   {
      if(true == (*hero)->GetInformationById(id,unitInfo))
      {
         return *hero;
      }
   }
   return NULL;
}

void GameController::Attack(uint8_t attackerId, uint8_t beAttackedId)
{
   
   UnitInfoType attackerInfo;
   UnitInfoType beAttackedInfo;
   UnitManager* attackerManager = GetUnitInformation(attackerId,&attackerInfo);
   UnitManager* beAttackedManager = GetUnitInformation(beAttackedId,&beAttackedInfo);

   uint8_t damage = attackerInfo.damage;
   std::cout << "attackerId" << (int)attackerId << std::endl;
   std::cout << "attackerInfoName " << attackerInfo.name << std::endl;
   std::cout << "attackerIdDamage" << attackerInfo.damage << std::endl;
   switch(beAttackedInfo.type)
   {
      case UnitManager::UM_TYPE_HERO:
         beAttackedManager->OnAttacked(beAttackedId, damage);
         break;
      case UnitManager::UM_TYPE_CARD:
         break;
   }
}

std::vector<uint8_t> GameController::GetHeroIdList(uint8_t player)
{
   return heroManager[player]->GetHeroIdList();
}