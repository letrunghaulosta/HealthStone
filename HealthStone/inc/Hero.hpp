#ifndef _HERO_HPP_INC_
#define _HERO_HPP_INC_

#include <string>
#include <cstdlib>

#include <iostream>

class Hero
{
public:
   Hero(std::string n,uint16_t h,uint16_t d):name(n),health(h),damage(d){}

   Hero(){};
   ~Hero(){};
   std::string getName(void) {return name;}
   uint16_t getHealth(void) {return health;}
   uint16_t getDamage(void) {return damage;}
   void OnAttacked(uint16_t val) { health = (val > health) ? (health - val): 0;}
private:
   std::string name;
   uint16_t    health;
   uint16_t    damage;
};

class Slark : public Hero
{
public:
   Slark():Hero("Slark",72,10)
   {
      std::cout << "Slark Constructor" << std::endl;
      std::cout << getName() << std::endl;
      std::cout << getHealth() << std::endl;
      std::cout << getDamage() << std::endl;
   }
};

class Butcher : public Hero
{
public:
   Butcher():Hero("Butcher",160,5)
   {
      std::cout << "Butcher Constructor" << std::endl;
      std::cout << getName() << std::endl;
      std::cout << getHealth() << std::endl;
      std::cout << getDamage() << std::endl;
   }
};
#endif