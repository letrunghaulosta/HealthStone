#ifndef _GAMECONTROLLER_HPP_INC_
#define _GAMECONTROLLER_HPP_INC_

#include <vector>
#include <memory>
#include <list>
#include <map>

#include "HeroManager.hpp"
#include "CardManager.hpp"

#define GC_TABLE_SIZE   20

#define GC_MAX_PLAYER   2
#define GC_PLAYER_1     0u
#define GC_PLAYER_2     1u

#define GC_TABLE_SLOT_AVAILABLE  0u
class GameController
{
public:
   struct UnitBonusType
   {
      int16_t damage;
      int16_t health;
   };

   enum GamePlayType
   {
      GameTypeNotSet = 0,
      GameTypeOffline,
      GameTypeHost,
      GameTypeClient
   };

   GameController(){
      heroManager.resize(GC_MAX_PLAYER);
      cardManager.resize(GC_MAX_PLAYER);
      unitOnHand.resize(GC_MAX_PLAYER);
      unitOnTable.resize(GC_MAX_PLAYER);
      bonusManager.resize(GC_MAX_PLAYER);
      currentRound = 0;
      gameType = GameTypeNotSet;
   }
   ~GameController(){delete gcInstance;}
   void Attack(uint8_t,uint8_t);
   void RemoveUnitById(uint8_t);
   void Start();
   UnitManager* GetUnitInformation(uint8_t, Unit::UnitInfoType*);
   std::list<uint8_t> GetHeroIdList(uint8_t player);
   std::list<uint8_t> GetUnitOnHandIdList(uint8_t player){return unitOnHand[player];}
   std::list<uint8_t> GetUnitOnTableIdList(uint8_t player){return unitOnTable[player];}
   std::list<uint8_t> GetCardIdList(uint8_t player);
   UnitBonusType*     GetBonusById(uint8_t player,uint8_t id){return &bonusManager[player][id];}
   void PlaceToBattle(uint8_t, uint8_t);
   void SetBonusUnit(uint8_t player,uint8_t id, int16_t damage, int16_t health);
   // void InformEffective();

   static GameController* GetInstance(){return gcInstance;}

   GamePlayType GetGamePlayType(){return gameType;}
   uint8_t GetPlayerByUnitId(uint8_t id);
   uint8_t CurrentPlayerTurn(){return playerTurn;}
   void ChangePlayerTurn(){playerTurn ^= 1;};
   int GetCurrentRound(){return currentRound;}
   void IncreaseRound(){currentRound++;};
   void SetGameType(GamePlayType t){gameType = t;}
private:
   GamePlayType gameType;
   int currentRound;
   uint8_t playerTurn;
   std::vector<std::list<uint8_t>> unitOnTable;
   std::vector<std::list<uint8_t>> unitOnHand;
   std::vector<HeroManager*> heroManager;
   std::vector<CardManager*> cardManager;
   std::vector<std::map<uint8_t,UnitBonusType>> bonusManager;
   static GameController *gcInstance;
};

#endif