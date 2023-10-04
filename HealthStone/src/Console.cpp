#include "Console.hpp"
#include "GameController.hpp"
#include "Socket.hpp"
#include <iostream>
#include <unistd.h>
#include <map>
#include <list>
#include <algorithm>
#include <string.h>

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
   cout << "1. Play Offline\n";
   cout << "2. Play Online(Host)\n";
   cout << "3. Play Online(Client)\n";
   int select;
   do
   {
      cin >> select;
      if(!cin || ((select != 1) && (select != 2) && (select != 3)))
      {
         cin.clear();
         cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
         cout << "Invalid CMD, please correct select" << endl;
      }
      else
      {
         GameController::GetInstance()->SetGameType((GameController::GamePlayType)select);
      }
   }while(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeNotSet);
   cin.ignore();
   SetSelect(ConsoleNext);
}

void Console::OnHost()
{
   system("clear");
   cout << "===========On Host============" << endl;
   Socket::GetInstance()->CreateSocket();
   cout << "CONNECT SUCCESS" << endl;
   sleep(1);
   SetSelect(ConsoleNext);
}

void Console::OnClient()
{
   system("clear");
   cout << "===========On Client============" << endl;
   Socket::GetInstance()->Connect();
   cout << "CONNECT SUCCESS" << endl;
   sleep(1);
   SetSelect(ConsoleNext);
}

void Console::OnLoading()
{
   system("clear");
   cout << "===========On Running============" << endl;
   GameController::GetInstance()->Start();
   cout << "Press enter to continue...";
   cin.ignore();
   system("clear");

   auto heroIdList_p1 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_1);
   DUMP_UNIT_LIST_INFO(heroIdList_p1,GC_PLAYER_1);
   cout << "====================================================================\n";
   cout << "||                                                                ||\n";
   cout << "====================================================================\n";
   auto heroIdList_p2 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_2);
   DUMP_UNIT_LIST_INFO(heroIdList_p2,GC_PLAYER_2);
   if(GameController::GamgeTypeHost == GameController::GetInstance()->GetGamePlayType())
   {
      Socket::SockMessage sockMessage;
      // sockMessage.messageType = Socket::MESSAGE_LENGTH;
      // sockMessage.size = 0;
      // for(auto id : heroIdList_p1)
      // {
      //    sockMessage.buffer[sockMessage.size] = id;
      //    sockMessage.size++;
      // }
      // sockMessage.size += 4;
      // Inform to client ready to send data
      // Socket::GetInstance()->Send(&sockMessage, 4);
      
      // Socket::SockMessage sockAck;
      // sockAck.messageType = Socket::ACK_KNOWLEDGE;
      // Socket::GetInstance()->Receive(&sockAck, 4);
      sockMessage.messageType = Socket::MESSAGE_LENGTH;
      Socket::GetInstance()->CopyData(heroIdList_p1,sockMessage.buffer,&sockMessage.size);
      Socket::GetInstance()->Send(&sockMessage, 4);
      sockMessage.messageType = Socket::HERO_LIST_ID_P1;
      Socket::GetInstance()->Send(&sockMessage, sockMessage.size);

      sockMessage.messageType = Socket::MESSAGE_LENGTH;
      Socket::GetInstance()->CopyData(heroIdList_p2,sockMessage.buffer,&sockMessage.size);
      Socket::GetInstance()->Send(&sockMessage, 4);
      sockMessage.messageType = Socket::HERO_LIST_ID_P2;
      Socket::GetInstance()->Send(&sockMessage, sockMessage.size);
   }
   SetSelect(ConsoleNext);
}

void Console::OnClientLoading()
{
   system("clear");
   cout << "===========On Running============" << endl;

   Socket::SockMessage sockMessage;
   Socket::GetInstance()->Receive(&sockMessage, 4);
   Socket::GetInstance()->Receive(&sockMessage, sockMessage.size);
   list<uint8_t> heroIdList_p1;
   uint16_t size = sockMessage.size - 4;
   while(size)
   {
      heroIdList_p1.push_front(sockMessage.buffer[--size]);
   }

   for(auto id : heroIdList_p1)
   {
      cout << "ID1: " << (int)id << " ";
   }

   Socket::GetInstance()->Receive(&sockMessage, 4);
   Socket::GetInstance()->Receive(&sockMessage, sockMessage.size);
   list<uint8_t> heroIdList_p2;
   size = sockMessage.size - 4;
   while(size)
   {
      heroIdList_p2.push_front(sockMessage.buffer[--size]);
   }
   for(auto id : heroIdList_p2)
   {
      cout << "ID2: " << (int)id << " ";
   }
   cout << endl;
   SetSelect(ConsoleNext);
}

void Console::OnPlayerMove(uint8_t player)
{
   cout << "==================     PLAYER " << player + 1 << " TURN     =================" << endl;;
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GamgeTypeHost)
   {
      if(player == GC_PLAYER_2)
      {
         cout << "Waiting to player 2" << endl;
         Socket::SockMessage sockMessage;
         while(sockMessage.messageType != Socket::DONE_MESSAGE)
         {
            Socket::GetInstance()->Receive(&sockMessage, sizeof(Socket::SockMessage));
            switch(sockMessage.messageType)
            {
               case Socket::CLIENT_REQUEST_ATTACK:
               break;
               case Socket::CLIENT_REQUEST_UNIT_INFO:
               {   
                  Unit::UnitInfoType unitInfo;
                  GameController::GetInstance()->GetUnitInformation(sockMessage.buffer[0], &unitInfo);
                  sockMessage.messageType = Socket::RESPOND_UNIT_INFO;
                  // Socket::GetInstance()->CopyData(&unitInfo,sizeof(unitInfo),&sockMessage.buffer,&sockMessage.size);
                  Socket::GetInstance()->Send(&sockMessage, sockMessage.size);
               }   
               break;
               case Socket::CLIENT_PLACE_CARD:
                  GameController::GetInstance()->PlaceToBattle(GC_PLAYER_2, sockMessage.buffer[0]);
               break;
            }
         }
         SetSelect(ConsoleNext);
         return;
      }
   }
   else if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeClient)
   {
      if(player == GC_PLAYER_1)
      {
         cout << "Waiting to player 1" << endl;;
         Socket::SockMessage sockMessage;
         while(sockMessage.messageType != Socket::DONE_MESSAGE)
         {
            Socket::GetInstance()->Receive(&sockMessage, 4);
         }
      }
      else
      {
         Socket::SockMessage sockMessage;
      }
      // SetSelect(ConsoleNext);
      return;
   }

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
   
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GamgeTypeHost)
   {
      Socket::SockMessage sockMessage;
      sockMessage.messageType = Socket::DONE_MESSAGE;
      Socket::GetInstance()->Send(&sockMessage, 4);
   }

   SetSelect(ConsoleNext);
}

void Console::OnClientMove()
{

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
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeOffline)
   {
      setState(sm, new ConsoleState_OnRunning());
   }
   else
   {
      setState(sm, new ConsoleState_SetupConnection());
   }
}
void ConsoleState_StartUp::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_StartUp::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_SetupConnection**/
void ConsoleState_SetupConnection::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNext);
}
void ConsoleState_SetupConnection::next(ConsoleStateMachine &sm)
{
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GamgeTypeHost)
   {
      setState(sm, new ConsoleState_HostSetup());
   }
   else
   {
      setState(sm, new ConsoleState_ClientSetup());
   }
}
void ConsoleState_SetupConnection::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_SetupConnection::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_HostSetup**/
void ConsoleState_HostSetup::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnHost();
}
void ConsoleState_HostSetup::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_OnRunning());
}
void ConsoleState_HostSetup::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_HostSetup::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_ClientSetup**/
void ConsoleState_ClientSetup::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnClient();
}
void ConsoleState_ClientSetup::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_OnRunning());
}
void ConsoleState_ClientSetup::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_ClientSetup::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}
/***ConsoleState_OnRunning**/
void ConsoleState_OnRunning::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   if(GameController::GameTypeClient == GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->OnClientLoading();
   }
   else
   {
      sm.getActionInstance()->OnLoading();
   }
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