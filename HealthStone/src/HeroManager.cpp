#include "HeroManager.hpp"
#include <iostream>
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

bool HeroManager::GetInformationById(uint8_t id, Unit::UnitInfoType* pUnitInfo)
{
   auto heroInfo = heroList.find(id);
   if(heroInfo != heroList.end())
   {
      pUnitInfo->name = heroInfo->second->getName();
      pUnitInfo->health = heroInfo->second->getHealth();
      pUnitInfo->damage = heroInfo->second->getDamage();
       pUnitInfo->type = Unit::UNIT_TYPE_HERO;
      return true;
   }
   return false;
}

void HeroManager::OnAttacked(uint8_t id, uint8_t damaged, UM_OnDestroy_Callback pfuncCbk)
{
   auto heroInfo = heroList.find(id);

   if(heroInfo != heroList.end())
   {
      heroInfo->second->OnAttacked(damaged);
   }

   if(0 == heroInfo->second->getHealth())
   {
      pfuncCbk(id);
   }
}

std::vector<uint8_t> HeroManager::GetHeroIdList()
{
   std::vector<uint8_t> heroIdList;
   for(auto hero : heroList)
   {
      heroIdList.push_back(hero.first);
   }
   return heroIdList;
}

void HeroManager::DestroyById(uint8_t id)
{
   auto heroInfo = heroList.find(id);

   if(heroInfo == heroList.end())
   {
      return;
   }

   EraseID(id);
   heroList.erase(heroInfo);
}
