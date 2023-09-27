#ifndef _HERO_HPP_INC_
#define _HERO_HPP_INC_

#include "Unit.hpp"
#include <string>
#include <cstdlib>
#include <iostream>

class Hero : public Unit
{
public:
   Hero(UnitInfoType c):heroInfo(c){}

   Hero(){};
   ~Hero(){};
   std::string getName(void) {return heroInfo.name;}
   uint16_t getHealth(void) {return heroInfo.health;}
   uint16_t getDamage(void) {return heroInfo.damage;}
   UnitType getType(void) {return heroInfo.type;}
   void OnAttacked(uint16_t val) { heroInfo.health = (val > heroInfo.health) ? 0:(heroInfo.health - val);}
private:
   UnitInfoType heroInfo;
};

class Slark : public Hero
{
public:
   Slark():Hero({"Slark",72,10,Unit::UNIT_TYPE_HERO})
   {
      std::cout << "Slark Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};

class Butcher : public Hero
{
public:
   Butcher():Hero({"Butcher",160,5,Unit::UNIT_TYPE_HERO})
   {
      std::cout << "Butcher Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};
#endif