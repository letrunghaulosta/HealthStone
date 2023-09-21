#ifndef _UNITMANAGER_HPP_INC_
#define _UNITMANAGER_HPP_INC_

#include <string>
#include <cstdlib>
#include <list>

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
   virtual void generate(void) = 0;
   virtual void notify(void) = 0;
   virtual void onAttacked(uint8_t) = 0;
   virtual UnitInfoType getInformationById(uint8_t) = 0;
   virtual void attack(uint8_t) = 0;
private:
   std::list<bool>  unitIdContainer;
};

#endif