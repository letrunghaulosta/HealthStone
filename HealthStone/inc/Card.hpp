#ifndef _CARD_HPP_INC_
#define _CARD_HPP_INC_

#include "Unit.hpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <functional>

typedef std::function<void(int)> Card_FuncPtrType;

void TechiesOnDestroyed(uint8_t);
void BrawlOnDestroyed(uint8_t);
void ShamanOnRunTimeEffect(uint8_t);
void ShamanRemoveEffectOnDestroyed(uint8_t);

class Card : public Unit
{
public:
   Card(UnitInfoType c, Card_FuncPtrType fp1 = NULL, Card_FuncPtrType fp2 = NULL):
         cardInfo(c),   runTimeEffect(fp1),      destroyEffect(fp2){}

   Card(){};
   ~Card(){};
   std::string getName(void) {return cardInfo.name;}
   uint16_t getHealth(void) {return cardInfo.health;}
   uint16_t getDamage(void) {return cardInfo.damage;}
   UnitType getType(void) {return cardInfo.type;}
   void OnAttacked(uint16_t val) { cardInfo.health = (val > cardInfo.health) ? 0:(cardInfo.health - val);}
   void ActiveDestroyEffect(uint8_t id){if(NULL != destroyEffect) {destroyEffect(id);}}
   void ActiveRunTimeEffect(uint8_t id){if(NULL != runTimeEffect) {runTimeEffect(id);}}
private:
   UnitInfoType cardInfo;
   Card_FuncPtrType runTimeEffect;
   Card_FuncPtrType destroyEffect;
};

typedef struct
{
   Card::UnitInfoType unitInfo;
   Card_FuncPtrType runTimeEffect;
   Card_FuncPtrType destroyEffect;
} CardConfigType;

const CardConfigType minionContainer[3] =
{
   {
   .unitInfo =
      {
         .name = "Ragnaros the Firelord",
         .health = 1,
         .damage = 3,
         .type = Unit::UNIT_TYPE_MINION
      },
   .runTimeEffect = NULL,
   .destroyEffect = NULL
   },
   {
   .unitInfo =
      {
         .name = "Bloodmage Thalnos",
         .health = 1,
         .damage = 1,
         .type = Unit::UNIT_TYPE_MINION
      },
   .runTimeEffect = NULL,
   .destroyEffect = NULL
   },
   {
   .unitInfo =
      {
         .name = "Techies",
         .health = 2,
         .damage = 1,
         .type = Unit::UNIT_TYPE_MINION
      },
   .runTimeEffect = NULL,
   .destroyEffect = TechiesOnDestroyed
   }
};

const CardConfigType shamanContainer[1] =
{
   {
   .unitInfo =
      {
         .name = "Flametongue Totem",
         .health = 3,
         .damage = 0,
         .type = Unit::UNIT_TYPE_SHAMAN
      },
   .runTimeEffect = ShamanOnRunTimeEffect,
   .destroyEffect = ShamanRemoveEffectOnDestroyed
   }
};
const CardConfigType spellContainer[1] =
{
   {
   .unitInfo =
      {
         .name = "Brawl",
         .health = 0,
         .damage = 0,
         .type = Unit::UNIT_TYPE_SPELL
      },
   .runTimeEffect = NULL,
   .destroyEffect = BrawlOnDestroyed
   }
};
class Minion : public Card
{
public:
   Minion(int i) : Card(minionContainer[i].unitInfo,minionContainer[i].runTimeEffect,minionContainer[i].destroyEffect)
   {
      std::cout << "Minion Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};

class Shaman : public Card
{
public:
   Shaman(int i) : Card(shamanContainer[i].unitInfo,shamanContainer[i].runTimeEffect,shamanContainer[i].destroyEffect)
   {
      std::cout << "Shaman Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};

class Spell : public Card
{
public:
   Spell(int i) : Card(spellContainer[i].unitInfo,spellContainer[i].runTimeEffect,spellContainer[i].destroyEffect)
   {
      std::cout << "Spell Constructor\t" << getName() << "\t" << getHealth()<< "\t" << getDamage() << std::endl;
   }
};
#endif

