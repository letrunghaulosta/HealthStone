#ifndef _UNITMANAGER_HPP_INC_
#define _UNITMANAGER_HPP_INC_

#include <string>
#include <cstdlib>
#include <vector>

struct UnitInfoType
{
   std::string name;
   uint16_t    health;
   uint16_t    damage;
   uint8_t     type;
};

class UnitManager
{
public:
   enum UnitType{
      UM_TYPE_INVALID = -1,
      UM_TYPE_HERO = 0,
      UM_TYPE_CARD = 1
   };

   virtual void Generate(void) = 0;
   virtual void OnAttacked(uint8_t, uint8_t) = 0;
   virtual bool GetInformationById(uint8_t, UnitInfoType*) = 0;
   virtual void Attack(uint8_t) = 0;
   uint8_t AllocateID() {
      for(auto it = unitIdContainer.begin(); it != unitIdContainer.end();it++)
      {
         if(false == *it)
         {
            *it = true;
            return (it - unitIdContainer.begin());
         }
      }
      unitIdContainer.push_back(true);
      return unitIdContainer.size()-1;
   };
   void EraseID(uint8_t id)
   {
      if(id > unitIdContainer.size())
      {
         return;
      }

      unitIdContainer[id] = false;
   }
private:
   static std::vector<bool> unitIdContainer;
};
#endif