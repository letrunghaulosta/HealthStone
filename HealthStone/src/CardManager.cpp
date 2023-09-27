#include "CardManager.hpp"
#include <iostream>
#include <ctime>

void CardManager::Generate()
{
   srand(std::time(NULL)^rand());
   for(int i=0;i<10;i++)
   {
      uint8_t id = AllocateID();
      int randomValue1 = rand() % 100;
      int randomValue2 = rand() % 3;
      switch(randomValue1%5)
      {
         case 0:
         case 1:
         case 2:
            cardList[id] = new Minion(randomValue2);
         break;
         case 3:
            cardList[id] = new Shaman(0);
         break;
         case 4:
            cardList[id] = new Spell(0);
         break;
      }
   }
}

std::vector<uint8_t> CardManager::GetCardIdList()
{
   std::vector<uint8_t> cardIdList;
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