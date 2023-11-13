#include "CardManager.hpp"
#include <iostream>
#include <ctime>

void CardManager::Generate(uint8_t quantity = 0)
{
   srand(std::time(NULL)^rand());
   for(int i=0;i<quantity;i++)
   {
      Spawn();
   }
}

uint8_t CardManager::Spawn()
{
   uint8_t id = AllocateID();
   int randomValue1 = rand() % 100;
   int randomValue2 = rand() % 2;
   switch(randomValue1%4)
   {
      case 0:
      case 1:
         cardList[id] = new Minion(randomValue2);
      break;
      case 2:
         cardList[id] = new Shaman(0);
      break;
      case 3:
         cardList[id] = new Spell(0);
      break;
   }
   return id;
}

void CardManager::OnAttacked(uint8_t id, uint8_t damaged, UM_OnDestroy_Callback pfuncCbk)
{
   auto cardInfo = cardList.find(id);
   if(cardInfo != cardList.end())
   {
      cardInfo->second->OnAttacked(damaged);
   }

   if(0 == cardInfo->second->getHealth())
   {
      cardInfo->second->ActiveDestroyEffect(id);
      pfuncCbk(id);
   }
}

std::list<uint8_t> CardManager::GetIdList()
{
   std::list<uint8_t> cardIdList;
   for(auto card : cardList)
   {
      cardIdList.push_back(card.first);
   }
   return cardIdList;
}

bool CardManager::GetInformationById(uint8_t id, Unit::UnitInfoType* pUnitInfo)
{
   auto cardInfo = cardList.find(id);
   if(cardInfo != cardList.end())
   {
      pUnitInfo->name = cardInfo->second->getName();
      pUnitInfo->health = cardInfo->second->getHealth();
      pUnitInfo->damage = cardInfo->second->getDamage();
      pUnitInfo->type = cardInfo->second->getType();
      return true;
   }
   return false;
}

void CardManager::DestroyById(uint8_t id)
{
   auto cardInfo = cardList.find(id);

   if(cardInfo == cardList.end())
   {
      return;
   }

   EraseID(id);
   cardList.erase(cardInfo);
}

void CardManager::ActivateRunTimeEffect(uint8_t id)
{
   auto cardInfo = cardList.find(id);
   cardInfo->second->ActiveRunTimeEffect(id);
}
