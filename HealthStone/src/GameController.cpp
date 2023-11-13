#include "GameController.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <iterator>

GameController* GameController::gcInstance = new GameController();
std::vector<bool> UnitManager::unitIdContainer(true);

void GameController::Start()
{
   heroManager[GC_PLAYER_1] = new HeroManager();
   heroManager[GC_PLAYER_1]->Generate(1);
   for(auto heroId : heroManager[GC_PLAYER_1]->GetIdList())
   {
      bonusManager[GC_PLAYER_1][heroId] = {0,0};
   }
   heroManager[GC_PLAYER_2] = new HeroManager();
   heroManager[GC_PLAYER_2]->Generate(1);
   for(auto heroId : heroManager[GC_PLAYER_2]->GetIdList())
   {
      bonusManager[GC_PLAYER_2][heroId] = {0,0};
   }

   cardManager[GC_PLAYER_1] = new CardManager();
   cardManager[GC_PLAYER_1]->Generate(10);
   unitOnHand[GC_PLAYER_1] = cardManager[GC_PLAYER_1]->GetIdList();
   for(auto unitId : unitOnHand[GC_PLAYER_1])
   {
      bonusManager[GC_PLAYER_1][unitId] = {0,0};
   }

   cardManager[GC_PLAYER_2] = new CardManager();
   cardManager[GC_PLAYER_2]->Generate(10);
   unitOnHand[GC_PLAYER_2] = cardManager[GC_PLAYER_2]->GetIdList();
   for(auto unitId : unitOnHand[GC_PLAYER_2])
   {
      bonusManager[GC_PLAYER_2][unitId] = {0,0};
   }
}

UnitManager* GameController::GetUnitInformation(uint8_t id,Unit::UnitInfoType* unitInfo)
{
   for(auto hero = heroManager.begin(); hero != heroManager.end();hero++)
   {
      if(true == (*hero)->GetInformationById(id,unitInfo))
      {
         return *hero;
      }
   }
   
   for(auto card = cardManager.begin(); card != cardManager.end();card++)
   {
      if(true == (*card)->GetInformationById(id,unitInfo))
      {
         return *card;
      }
   }

   return NULL;
}

void GameController::Attack(uint8_t attackerId, uint8_t beAttackedId)
{

   Unit::UnitInfoType attackerInfo;
   Unit::UnitInfoType beAttackedInfo;
   UnitManager* attackerManager = GetUnitInformation(attackerId,&attackerInfo);
   UnitManager* beAttackedManager = GetUnitInformation(beAttackedId,&beAttackedInfo);
   UnitBonusType* bonus = GetBonusById(GetPlayerByUnitId(attackerId),attackerId);

   uint8_t damage = attackerInfo.damage + bonus->damage;

   std::cout << "attackerId " << (int)attackerId << ":\tbeAttackedId " << (int)beAttackedId << std::endl;
   std::cout << "attackerInfoName " << attackerInfo.name << ":\tbeAttackedInfoName " << beAttackedInfo.name << std::endl;
   std::cout << "attackerInfoDamage " << attackerInfo.damage << "+" << bonus->damage << ":\tbeAttackedInfoDamage " << beAttackedInfo.name << std::endl;

   beAttackedManager->OnAttacked(beAttackedId, damage, std::bind(&GameController::RemoveUnitById,this, std::placeholders::_1));
}

std::list<uint8_t> GameController::GetHeroIdList(uint8_t player)
{
   return heroManager[player]->GetIdList();
}
std::list<uint8_t> GameController::GetCardIdList(uint8_t player)
{
   return cardManager[player]->GetIdList();
}
uint8_t GameController::GetPlayerByUnitId(uint8_t id) {
   for(uint8_t player = GC_PLAYER_1;player < GC_MAX_PLAYER; player++)
   {
      std::list<uint8_t> heroIdList = heroManager[player]->GetIdList();
      std::list<uint8_t> cardIdList = cardManager[player]->GetIdList();

      if(std::find(cardIdList.begin(),cardIdList.end(),id) != cardIdList.end()
      || std::find(heroIdList.begin(),heroIdList.end(),id) != heroIdList.end())
      {
         return player;
      }
   }
   return 255;
}

void GameController::RemoveUnitById(uint8_t id)
{
   Unit::UnitInfoType unitInfo;
   UnitManager* unitManager = GetUnitInformation(id,&unitInfo);
   auto player = GetPlayerByUnitId(id);
   bonusManager[player].erase(bonusManager[player].find(id));

   if(unitInfo.type == Unit::UNIT_TYPE_HERO)
   {
      static_cast<HeroManager*>(unitManager)->DestroyById(id);
   }
   else
   {
      for(auto& playerUnitOnTable : unitOnTable)
      {
         playerUnitOnTable.remove(id);
      }
      static_cast<CardManager*>(unitManager)->DestroyById(id);
   }

}

void GameController::PlaceToBattle(uint8_t player, uint8_t id)
{
   unitOnHand[player].remove(id);
   unitOnTable[player].push_back(id);

   Unit::UnitInfoType unitInfo;
   CardManager* cardMng = static_cast<CardManager*>(GameController::GetInstance()->GetUnitInformation(id,&unitInfo));
   if(unitInfo.type == Unit::UNIT_TYPE_SPELL)
   {
      cardMng->OnAttacked(id,0,std::bind(&GameController::RemoveUnitById,GameController::GetInstance(), std::placeholders::_1));
   }
   else
   {
      cardMng->ActivateRunTimeEffect(id);
   }
}

void GameController::ActiveRunTimeEffect()
{
   for(auto unitIdList : unitOnTable)
      for(auto id : unitIdList)
      {
         Unit::UnitInfoType unitInfo;
         CardManager* cardMng = static_cast<CardManager*>(GameController::GetInstance()->GetUnitInformation(id,&unitInfo));
         cardMng->ActivateRunTimeEffect(id);
      }
}

void GameController::SetBonusUnit(uint8_t player,uint8_t id, int16_t damage, int16_t health){
   bonusManager[player][id].damage += damage;
   bonusManager[player][id].health += health;
   std::cout << "Player:" << (int)player << "id:" <<(int)id << "damage:" << bonusManager[player][id].damage << "health:" << bonusManager[player][id].health << std::endl;
}

void GameController::RefreshBonusUnit()
{
   for(auto& bonusMng : bonusManager)
      for(auto& unit : bonusMng)
         unit.second = {0};
}

void GameController::SpawnCard()
{
   uint8_t unitId = cardManager[GC_PLAYER_1]->Spawn();
   unitOnHand[GC_PLAYER_1].push_back(unitId);
   bonusManager[GC_PLAYER_1][unitId] = {0,0};
   std::cout << "SPAWN_P1 ID:" << (int)unitId << std::endl; 

   unitId = cardManager[GC_PLAYER_2]->Spawn();
   unitOnHand[GC_PLAYER_2].push_back(unitId);
   bonusManager[GC_PLAYER_2][unitId] = {0,0};
   std::cout << "SPAWN_P2 ID:" << (int)unitId << std::endl;
}

void TechiesOnDestroyed(uint8_t id)
{
   std::cout << "Techies[Id" << (int)id <<"] has been destroyed. Deal 3 damage to All Hero\n";
   for(uint8_t player = GC_PLAYER_1;player < GC_MAX_PLAYER; player++)
   {
      std::list<uint8_t> heroIdList = GameController::GetInstance()->GetHeroIdList(player);
      Unit::UnitInfoType beAttackedInfo;
      for(auto heroId : heroIdList)
      {
         UnitManager* beAttackedManager = GameController::GetInstance()->GetUnitInformation(heroId,&beAttackedInfo);
         beAttackedManager->OnAttacked(heroId, 3, std::bind(&GameController::RemoveUnitById,GameController::GetInstance(), std::placeholders::_1));
      }
   }
}

void BrawlOnDestroyed(uint8_t id)
{
   std::cout << "Brawl[Id" << (int)id << "] has been activated: ";
   for(uint8_t player = GC_PLAYER_1;player < GC_MAX_PLAYER; player++)
   {
      std::list<uint8_t> cardIdList = GameController::GetInstance()->GetUnitOnTableIdList(player);
      Unit::UnitInfoType beAttackedInfo;
      if(std::find(cardIdList.begin(),cardIdList.end(),id) == cardIdList.end() && 0 != cardIdList.size())
      {
         std::list<uint8_t> minionIdList;
         for(auto cardId : cardIdList)
         {
            (void)GameController::GetInstance()->GetUnitInformation(cardId,&beAttackedInfo);
            if(beAttackedInfo.type == Unit::UNIT_TYPE_MINION)
            {
               minionIdList.push_back(cardId);
            }
         }
         if(0 != minionIdList.size())
         {
            uint8_t random = rand()%minionIdList.size();

            std::list<uint8_t>::iterator it = minionIdList.begin();
            std::advance(it,random);

            UnitManager* beAttackedManager = GameController::GetInstance()->GetUnitInformation(*it,&beAttackedInfo);
            std::cout << "ID: " << (int)*it << "\t";
            std::cout << "Name: ";std::cout.width(20);std::cout << std::left << beAttackedInfo.name<< "\t";
            std::cout << "Is destroyed" << std::endl;

            beAttackedManager->OnAttacked(*it, beAttackedInfo.health, std::bind(&GameController::RemoveUnitById,GameController::GetInstance(), std::placeholders::_1));
         }
      }
   }
}

void ShamanOnRunTimeEffect(uint8_t id)
{
   std::cout << "Shaman[Id" << (int)id << "] has been activated: ";
   uint8_t player = GameController::GetInstance()->GetPlayerByUnitId(id);
   std::list<uint8_t> cardIdList = GameController::GetInstance()->GetCardIdList(player);
   for(auto cardId : cardIdList)
   {
      Unit::UnitInfoType unitInfo;
      GameController::GetInstance()->GetUnitInformation(cardId,&unitInfo);
      if(unitInfo.type != Unit::UNIT_TYPE_SPELL && cardId != id)
      {
         GameController::GetInstance()->SetBonusUnit(player,cardId,1,0);
      }
   }
}
