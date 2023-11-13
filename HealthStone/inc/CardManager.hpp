#ifndef _CARDMANAGER_HPP_INC_
#define _CARDMANAGER_HPP_INC_

#include "UnitManager.hpp"
#include "Card.hpp"
#include <map>
#include <list>

class CardManager : public UnitManager
{
public:
   ~CardManager(){
      for(auto it = cardList.begin();it != cardList.end();it++)
      {
         if(it->second != nullptr)
         {
            delete it->second;
         }
      }
   }
   void Generate(uint8_t);
   void OnAttacked(uint8_t, uint8_t,UM_OnDestroy_Callback);
   bool GetInformationById(uint8_t, Unit::UnitInfoType*);
   void DestroyById(uint8_t);
   void Attack(uint8_t){};
   void ActivateRunTimeEffect(uint8_t);
   std::list<uint8_t> GetIdList();
   uint8_t Spawn();
private:
   std::map<uint8_t,Card*> cardList;
};

#endif