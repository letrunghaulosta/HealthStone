#ifndef _GAMECONTROLLER_HPP_INC_
#define _GAMECONTROLLER_HPP_INC_

#include <vector>
#include <memory>

#include "HeroManager.hpp"
#include "CardManager.hpp"

#define GC_TABLE_SIZE   20
#define GC_MAX_PLAYER   2
#define GC_PLAYER_1     0
#define GC_PLAYER_2     1
class GameController
{
public:
   GameController(){
      heroManager.reserve(GC_MAX_PLAYER);
   }
   ~GameController(){delete gcInstance;}
   void Attack(uint8_t,uint8_t);
   void Start();
   UnitManager* GetUnitInformation(uint8_t,UnitInfoType*);
   std::vector<uint8_t> GetHeroIdList(uint8_t player);
   // void InformEffective();
   static GameController* GetInstance(){return gcInstance;}
   uint8_t CurrentPlayerTurn(){return playerTurn;}
   void ChangePlayerTurn(){playerTurn ^= 1;};
private:
   uint8_t playerTurn;
   uint8_t unitOntable[GC_TABLE_SIZE] = {0};
   std::vector<std::vector<uint8_t>> unitOnHand[GC_MAX_PLAYER];
   std::vector<HeroManager*> heroManager;
   std::vector<CardManager*> cardManager[GC_MAX_PLAYER];
   std::vector<std::vector<UnitInfoType>> bonusManager[GC_MAX_PLAYER];
   static GameController *gcInstance;
};

#endif