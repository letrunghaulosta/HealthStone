#ifndef _HEROMANAGER_HPP_INC_
#define _HEROMANAGER_HPP_INC_

#include "UnitManager.hpp"
#include "Hero.hpp"
#include "map"

class HeroManager : public UnitManager
{
public:
   ~HeroManager(){
      for(auto it = heroList.begin();it != heroList.end();it++)
      {
         if(it->second != nullptr)
         {
            delete it->second;
         }
      }
   }
   void Generate(void);
   void OnAttacked(uint8_t, uint8_t);
   bool GetInformationById(uint8_t, UnitInfoType*);
   void Attack(uint8_t){};
private:
   std::map<uint8_t,Hero*> heroList;
};

#endif