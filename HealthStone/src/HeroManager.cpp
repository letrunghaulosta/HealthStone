
#include "HeroManager.hpp"
#include <ctime>
void HeroManager::Generate()
{
   uint8_t id = AllocateID();
   srand(std::time(NULL)^rand());
   int randomValue = rand() % 100;
   switch(randomValue%2)
   {
      case 0:
         heroList[id] = new Slark();
      break;
      case 1:
         heroList[id] = new Butcher();
      break;
   }
}

bool HeroManager::GetInformationById(uint8_t id, UnitInfoType* pUnitInfo)
{
   auto heroInfo = heroList.find(id);

   if(heroInfo != heroList.end())
   {
      pUnitInfo->name = heroInfo->second->getName();
      pUnitInfo->health = heroInfo->second->getHealth();
      pUnitInfo->damage = heroInfo->second->getDamage();
      pUnitInfo->type = UM_TYPE_HERO;
      return true;
   }
   return false;
}

void HeroManager::OnAttacked(uint8_t id, uint8_t damaged)
{
   auto heroInfo = heroList.find(id);

   if(heroInfo != heroList.end())
   {
      heroInfo->second->OnAttacked(damaged);
   }

   if(heroInfo->second->getHealth())
   {
      
   }
}
