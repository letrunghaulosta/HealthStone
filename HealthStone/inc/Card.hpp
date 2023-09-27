#ifndef _CARD_HPP_INC_
#define _CARD_HPP_INC_

#include "Unit.hpp"
#include <string>
#include <cstdlib>
#include <iostream>

class Card : public Unit
{
public:
   Card(UnitInfoType c):cardInfo(c){}

   Card(){};
   ~Card(){};
   std::string getName(void) {return cardInfo.name;}
   uint16_t getHealth(void) {return cardInfo.health;}
   uint16_t getDamage(void) {return cardInfo.damage;}
   UnitType getType(void) {return cardInfo.type;}
   void OnAttacked(uint16_t val) { cardInfo.health = (val > cardInfo.health) ? 0:(cardInfo.health - val);}
private:
   UnitInfoType cardInfo;
};

const Card::UnitInfoType minionContainer[3] =
   {
      {
         .name = "Ragnaros the Firelord",
         .health = 1,
         .damage = 3,
         .type = Unit::UNIT_TYPE_MINION
      },
      {
         .name = "Bloodmage Thalnos",
         .health = 1,
         .damage = 1,
         .type = Unit::UNIT_TYPE_MINION
      },
      {
         .name = "Techies",
         .health = 2,
         .damage = 1,
         .type = Unit::UNIT_TYPE_MINION
      }
};
const Card::UnitInfoType shamanContainer[1] =
   {
      {
         .name = "Flametongue Totem",
         .health = 3,
         .damage = 0,
         .type = Unit::UNIT_TYPE_SHAMAN
      }
};
const Card::UnitInfoType spellContainer[1] =
   {
      {
         .name = "Brawl",
         .health = 0,
         .damage = 0,
         .type = Unit::UNIT_TYPE_SPELL
      }
};
class Minion : public Card
{
public:
   Minion(int i) : Card(minionContainer[i])
   {
      std::cout << "Minion Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};

class Shaman : public Card
{
public:
   Shaman(int i) : Card(shamanContainer[i])
   {
      std::cout << "Shaman Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};

class Spell : public Card
{
public:
   Spell(int i) : Card(spellContainer[i])
   {
      std::cout << "Spell Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};
#endif

