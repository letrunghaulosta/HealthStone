#include "Console.hpp"
#include "GameController.hpp"
#include <iostream>
#include <unistd.h>
#include <map>
#include <list>
#include <algorithm>

#define DUMP_UNIT_INFO(i,n,h,bh,d,bd,t)  do                                     \
                                 {                                      \
                                    cout << "ID: " << i << "\t";        \
                                    cout << "Name: ";cout.width(20);cout << left << n << "\t";\
                                    cout << "Health: " << h << " " << (bh != 0?"+"+to_string(bh):"") << "\t";    \
                                    cout << "Damage: " << d << " " << (bd != 0?"+"+to_string(bd):"") << "\t";    \
                                    cout << "Type: " << t << endl;      \
                                 }while(0)

#define DUMP_UNIT_LIST_INFO(l,p) do                                                                    \
                              {                                                                      \
                                 for (auto Id : l)                                                   \
                                 {                                                                   \
                                    Unit::UnitInfoType unitInfo;                                     \
                                    GameController::GetInstance()->GetUnitInformation(Id, &unitInfo);\
                                    GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(p,Id);\
                                    DUMP_UNIT_INFO((int)Id,                                          \
                                                   unitInfo.name,                                    \
                                                   unitInfo.health,                                  \
                                                   b->health,                                         \
                                                   unitInfo.damage,                                  \
                                                   b->damage,                                         \
                                                   UnitTypeToStrMapping.at(unitInfo.type));          \
                                 }                                                                   \
                              }while (0)

using namespace std;

const std::map<Unit::UnitType, std::string> UnitTypeToStrMapping =
{
   {Unit::UNIT_TYPE_HERO, "HERO"},
   {Unit::UNIT_TYPE_MINION, "MINION"},
   {Unit::UNIT_TYPE_SHAMAN, "SHAMAN"},
   {Unit::UNIT_TYPE_SPELL, "SPELL"}
};

void Console::OnStart()
{
   system("clear");
   cout << "===========HEALTH STONE============\n";
   cout << "Press enter to continue...\n";
   cin.ignore();
   SetSelect(ConsoleNext);
}

void Console::OnLoading()
{
   system("clear");
   cout << "===========On Running============" << endl;
   GameController::GetInstance()->Start();
   cout << "Press enter to continue..." << endl << endl;
   cin.ignore();
   system("clear");

   auto heroIdList = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_1);
   DUMP_UNIT_LIST_INFO(heroIdList,GC_PLAYER_1);
   cout << "====================================================================\n";
   cout << "||                                                                ||\n";
   cout << "====================================================================\n";
   heroIdList = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_2);
   DUMP_UNIT_LIST_INFO(heroIdList,GC_PLAYER_2);

   SetSelect(ConsoleNext);
}

void Console::OnPlayerMove(uint8_t player)
{
   cout << "==================     PLAYER " << player + 1 << " TURN     =================\n\n";
   list<uint8_t> unitOnHandList = GameController::GetInstance()->GetUnitOnHandIdList(player);
   if(0 == unitOnHandList.size())
   {
      cout << "There is no card to place. Press enter to continue..." << endl;
      cin.ignore();
   }
   else
   {
      DUMP_UNIT_LIST_INFO(unitOnHandList,player);
      int select;
      char confirm;
      do
      {
         cout << "Select by Unit ID to place to the battle: ";
         cin >> select;
         Unit::UnitInfoType unitInfo;
         if(!cin ||
            NULL == GameController::GetInstance()->GetUnitInformation(select, &unitInfo) || 
            find(unitOnHandList.begin(),unitOnHandList.end(),select) == unitOnHandList.end())
         {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid ID, please correct select" << endl;
         }
         else
         {
            GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
            DUMP_UNIT_INFO((int)select,
                  unitInfo.name,
                  unitInfo.health,
                  b->health,
                  unitInfo.damage,
                  b->damage,
                  UnitTypeToStrMapping.at(unitInfo.type));
            cout << "Is put on battle?(y/n):";
            cin >> confirm;
         }
      } while(confirm != 'y' && confirm != 'Y');
      GameController::GetInstance()->PlaceToBattle(player, (int)select);
   }
   SetSelect(ConsoleNext);
}

void Console::AfterPlayerMove(uint8_t player)
{
   list<uint8_t> heroIdList = GameController::GetInstance()->GetHeroIdList(player);
   list<uint8_t> targetHeroIdList = GameController::GetInstance()->GetHeroIdList(player^1);
   
   list<uint8_t> cardIdList = GameController::GetInstance()->GetUnitOnTableIdList(player);
   list<uint8_t> targetCardIdList = GameController::GetInstance()->GetUnitOnTableIdList(player^1);

   for(auto heroId : heroIdList)
   {
      targetHeroIdList = GameController::GetInstance()->GetHeroIdList(player^1);
      for(auto targetHeroId : targetHeroIdList)
      {
         GameController::GetInstance()->Attack(heroId,targetHeroId);
      }
   }

   for(auto cardId : cardIdList)
   {
      targetCardIdList = GameController::GetInstance()->GetUnitOnTableIdList(player^1);
      for(auto targetCardId : targetCardIdList)
      {
         GameController::GetInstance()->Attack(cardId,targetCardId);
      }

      targetHeroIdList = GameController::GetInstance()->GetHeroIdList(player^1);
      for(auto targetHeroId : targetHeroIdList)
      {
         GameController::GetInstance()->Attack(cardId,targetHeroId);
      }
   }
}

void Console::OnProcess()
{
   system("clear");
   cout << "====================================================================\n";
   auto heroIdList = GameController::GetInstance()->GetHeroIdList(GC_PLAYER_1);
   auto unitOnTableList = GameController::GetInstance()->GetUnitOnTableIdList(GC_PLAYER_1);
   if(heroIdList.size() == 0)
   {
      heroIdList = GameController::GetInstance()->GetHeroIdList(GC_PLAYER_2);
      if(heroIdList.size() == 0)
      {
         cout << "DRAW" << endl;
      }
      else
      {
         cout << "PLAYER 2 WIN" << endl;
      }
      SetExit();
   }
   else
   {
      DUMP_UNIT_LIST_INFO(heroIdList,GC_PLAYER_1);
      DUMP_UNIT_LIST_INFO(unitOnTableList,GC_PLAYER_1);
   }

   cout << "====================================================================\n";
   cout.width(30);cout << left << "||" << "ROUND ";
   cout.width(30);cout << left << GameController::GetInstance()->GetCurrentRound()/2;
   cout << "||" << endl;
   cout << "====================================================================\n";

   heroIdList = GameController::GetInstance()->GetHeroIdList(GC_PLAYER_2);
   unitOnTableList = GameController::GetInstance()->GetUnitOnTableIdList(GC_PLAYER_2);
   if(heroIdList.size() == 0)
   {
      cout << "PLAYER 1 WIN" << endl;
      SetExit();
   }
   else
   {
      DUMP_UNIT_LIST_INFO(heroIdList,GC_PLAYER_2);
      DUMP_UNIT_LIST_INFO(unitOnTableList,GC_PLAYER_2);
   }
   cout << "====================================================================\n";
   SetSelect(ConsoleNext);
   usleep(500000);
}

void Console::OnEnd()
{
   system("clear");
   cout << "=========== END ============" << endl;
   cout << "Press enter to continue..." << endl;
   cin.ignore();
}

void ConsoleState::setState(ConsoleStateMachine &sm, ConsoleState *state)
{
   ConsoleState *freeState = sm.consoleState;
   sm.consoleState = state;
   sm.entry();
   delete freeState;
}

void ConsoleState_Root::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   return;
}
void ConsoleState_Root::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_StartUp());
}
void ConsoleState_Root::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_Root::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_StartUp**/
void ConsoleState_StartUp::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnStart();
}
void ConsoleState_StartUp::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_OnRunning());
}
void ConsoleState_StartUp::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_StartUp::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_OnRunning**/
void ConsoleState_OnRunning::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnLoading();
}
void ConsoleState_OnRunning::next(ConsoleStateMachine &sm)
{
   if(sm.getActionInstance()->IsExit())
   {
      setState(sm, new ConsoleState_End());
      return;
   }
   setState(sm, new ConsoleState_OnPlayer1Turn());
}
void ConsoleState_OnRunning::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnRunning::select(ConsoleStateMachine &sm, uint8_t option)
{
   if(option == 9)
   {
      setState(sm, new ConsoleState_End());
   }
}
/***ConsoleState_OnPlayer1Turn**/
void ConsoleState_OnPlayer1Turn::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);

   sm.getActionInstance()->OnPlayerMove(GC_PLAYER_1);
}
void ConsoleState_OnPlayer1Turn::next(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->AfterPlayerMove(GC_PLAYER_1);
   setState(sm, new ConsoleState_OnProcess());
}
void ConsoleState_OnPlayer1Turn::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnPlayer1Turn::select(ConsoleStateMachine &sm, uint8_t option)
{
}
/***ConsoleState_OnPlayer2Turn**/
void ConsoleState_OnPlayer2Turn::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnPlayerMove(GC_PLAYER_2);
}
void ConsoleState_OnPlayer2Turn::next(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->AfterPlayerMove(GC_PLAYER_2);
   setState(sm, new ConsoleState_OnProcess());
}
void ConsoleState_OnPlayer2Turn::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnPlayer2Turn::select(ConsoleStateMachine &sm, uint8_t option)
{
   return this->GetParent()->select(sm,option);
}
/***ConsoleState_OnProcess**/
void ConsoleState_OnProcess::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   GameController::GetInstance()->IncreaseRound();
   sm.getActionInstance()->OnProcess();

   GameController::GetInstance()->ChangePlayerTurn();
}
void ConsoleState_OnProcess::next(ConsoleStateMachine &sm)
{

   if(sm.getActionInstance()->IsExit())
   {
      this->GetParent()->next(sm);
   }

   if(GC_PLAYER_1 == GameController::GetInstance()->CurrentPlayerTurn())
   {
      setState(sm, new ConsoleState_OnPlayer1Turn());
   }
   else
   {
      setState(sm, new ConsoleState_OnPlayer2Turn());
   }
}
void ConsoleState_OnProcess::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnProcess::select(ConsoleStateMachine &sm, uint8_t option)
{
   return this->GetParent()->select(sm,option);
}
/***ConsoleState_End**/
void ConsoleState_End::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnEnd();
}
void ConsoleState_End::next(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_End::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_End::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}