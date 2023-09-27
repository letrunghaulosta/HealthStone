#ifndef _UNITMANAGER_HPP_INC_
#define _UNITMANAGER_HPP_INC_

#include "Unit.hpp"
#include <string>
#include <cstdlib>
#include <vector>
#include <functional>

typedef std::function<void(int)> UM_OnDestroy_Callback;

class UnitManager
{
public:
   virtual void Generate(void) = 0;
   virtual void OnAttacked(uint8_t, uint8_t, UM_OnDestroy_Callback) = 0;
   virtual bool GetInformationById(uint8_t, Unit::UnitInfoType*) = 0;
   virtual void Attack(uint8_t) = 0;
   uint8_t AllocateID() {
      for(auto it = unitIdContainer.begin(); it != unitIdContainer.end();it++)
      {
         if(false == *it)
         {
            *it = true;
            return (it - unitIdContainer.begin() + 1);
         }
      }
      unitIdContainer.push_back(true);
      return unitIdContainer.size();
   };
   void EraseID(uint8_t id)
   {
      id--;
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