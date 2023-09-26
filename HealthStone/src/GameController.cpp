#include "GameController.hpp"

GameController* GameController::gcInstance = new GameController();
std::vector<bool> UnitManager::unitIdContainer(true);

void GameController::Start()
{
   heroManager[GC_PLAYER_1] = new HeroManager();
   heroManager[GC_PLAYER_1]->Generate();
   heroManager[GC_PLAYER_2] = new HeroManager();
   heroManager[GC_PLAYER_2]->Generate();
}

UnitManager* GameController::GetUnitInformation(uint8_t id,UnitInfoType* unitInfo)
{
   UnitInfoType unitInfo;
   for(auto hero : heroManager)
   {
      if(true == hero->GetInformationById(id, unitInfo))
      {
         return hero;
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

   switch(beAttackedInfo.type)
   {
      case UnitManager::UM_TYPE_HERO:
         beAttackedManager->OnAttacked(beAttackedId, damage);
         break;
      case UnitManager::UM_TYPE_CARD:
         break;
   }

}
