#include "Console.hpp"
#include "GameController.hpp"
#include "Socket.hpp"
#include <iostream>
#include <unistd.h>
#include <map>
#include <list>
#include <algorithm>
#include <string.h>
#include <memory>

#define DUMP_UNIT_INFO(i,n,h,bh,d,bd,t)  do                                     \
                                 {                                      \
                                    cout << "ID: " << i << "\t";        \
                                    cout << "Name: ";cout.width(20);cout << left << n << "\t";\
                                    cout << "Health: " << h << " " << (bh != 0?"+"+to_string(bh):"") << "\t";    \
                                    cout << "Damage: " << d << " " << (bd != 0?"+"+to_string(bd):"") << "\t";    \
                                    cout << "Type: " << t << endl;      \
                                 }while(0)

#define DUMP_UNIT_INFO_S(i,n,h,d,t)  do                                     \
                                 {                                      \
                                    cout << "ID: " << i << "\t";        \
                                    cout << "Name: ";cout.width(20);cout << left << n << "\t";\
                                    cout << "Health: " << h << "\t";    \
                                    cout << "Damage: " << d << "\t";    \
                                    cout << "Type: " << t << "\n";      \
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
#define UNIT_NAME_SIZE  30
#define UNIT_INFO_SIZE  (UNIT_NAME_SIZE + (sizeof(Unit::UnitInfoType) - sizeof(std::string)))

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
   // system("clear");
   cout << "\033[H\033[J";
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
   // system("clear");
   cout << "\033[H\033[J";
   cout << "===========On Host============" << endl;
   Socket::GetInstance()->CreateSocket();
   cout << "CONNECT SUCCESS" << endl;
   sleep(1);
   SetSelect(ConsoleNext);
}

void Console::OnClient()
{
   // system("clear");
   cout << "\033[H\033[J";
   cout << "===========On Client============" << endl;
   Socket::GetInstance()->Connect();
   cout << "CONNECT SUCCESS" << endl;
   sleep(1);
   SetSelect(ConsoleNext);
}

void Console::OnLoading()
{
   // system("clear");
   cout << "\033[H\033[J";
   cout << "===========On Running============" << endl;
   GameController::GetInstance()->Start();
   cout << "Press enter to continue...";
   cin.ignore();
   // system("clear");
   cout << "\033[H\033[J";

   auto heroIdList_p1 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_1);
   DUMP_UNIT_LIST_INFO(heroIdList_p1,GC_PLAYER_1);
   cout << "====================================================================\n";
   cout << "||                                                                ||\n";
   cout << "====================================================================\n";
   auto heroIdList_p2 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_2);
   DUMP_UNIT_LIST_INFO(heroIdList_p2,GC_PLAYER_2);
   if(GameController::GameTypeHost == GameController::GetInstance()->GetGamePlayType())
   {
      Socket::SockMessage sockMessage;
      vector<uint8_t> temp;
      copy(begin(heroIdList_p1),end(heroIdList_p1),back_inserter(temp));
      sockMessage.messageType = Socket::HERO_LIST_ID_P1;
      memcpy(sockMessage.buffer,&temp[0],temp.size());
      Socket::GetInstance()->Send(&sockMessage, temp.size());
      temp.clear();
      copy(begin(heroIdList_p2),end(heroIdList_p2),back_inserter(temp));
      sockMessage.messageType = Socket::HERO_LIST_ID_P2;
      memcpy(sockMessage.buffer,&temp[0],temp.size());
      Socket::GetInstance()->Send(&sockMessage, temp.size());

      HandleMessage();
   }
   SetSelect(ConsoleNext);
}

void Console::OnClientLoading()
{
   // system("clear");
   cout << "\033[H\033[J";
   cout << "===========On Running============" << endl;

   Socket::SockMessage sockMessage;
   uint16_t messageLength;
   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   uint8_t size = messageLength;
   vector<uint8_t> heroIdList_p1(size);
   uint8_t index;
   memcpy(&heroIdList_p1[0], sockMessage.buffer, size);

   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   size = messageLength;
   vector<uint8_t> heroIdList_p2(size);
   memcpy(&heroIdList_p2[0], sockMessage.buffer, size);
   
   // Request Info Hero P1
   sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
   size = heroIdList_p1.size();
   memcpy(sockMessage.buffer, &heroIdList_p1[0], size);
   Socket::GetInstance()->Send(&sockMessage, size);
   Socket::GetInstance()->Receive(&sockMessage,&messageLength);
   index = 0;
   for(auto id : heroIdList_p1)
   {
      Unit::UnitInfoType unitInfo;
      vector<uint8_t> tempBuffer(38);
      memcpy(&tempBuffer[0],&sockMessage.buffer[index*UNIT_INFO_SIZE],UNIT_INFO_SIZE);

      unitInfo.name = string((char*)&tempBuffer[0], UNIT_NAME_SIZE);
      unitInfo.name.resize(unitInfo.name.find('\0'));
      memcpy(&unitInfo.health, &tempBuffer[UNIT_NAME_SIZE], UNIT_INFO_SIZE-UNIT_NAME_SIZE);
      DUMP_UNIT_INFO_S((int)id, unitInfo.name, unitInfo.health, unitInfo.damage, UnitTypeToStrMapping.at(unitInfo.type));
      index++;
   }
   cout << "====================================================================\n";
   cout << "||                                                                ||\n";
   cout << "====================================================================\n";
   // Request Info Hero P2
   sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
   size = heroIdList_p2.size();
   memcpy(sockMessage.buffer, &heroIdList_p2[0], size);
   Socket::GetInstance()->Send(&sockMessage, size);
   Socket::GetInstance()->Receive(&sockMessage,&messageLength);
   index = 0;
   for(auto id : heroIdList_p2)
   {
      Unit::UnitInfoType unitInfo;
      vector<uint8_t> tempBuffer(UNIT_INFO_SIZE);
      memcpy(&tempBuffer[0],&sockMessage.buffer[index*UNIT_INFO_SIZE],UNIT_INFO_SIZE);

      unitInfo.name = string((char*)&tempBuffer[0], UNIT_NAME_SIZE);
      unitInfo.name.resize(unitInfo.name.find('\0'));
      memcpy(&unitInfo.health, &tempBuffer[UNIT_NAME_SIZE], UNIT_INFO_SIZE-UNIT_NAME_SIZE);
      DUMP_UNIT_INFO_S((int)id, unitInfo.name, unitInfo.health, unitInfo.damage, UnitTypeToStrMapping.at(unitInfo.type));
      index++;
   }

   Socket::SockMessage doneMessage;
   doneMessage.messageType = Socket::DONE_MESSAGE;
   Socket::GetInstance()->Send(&doneMessage, 0);

   SetSelect(ConsoleNext);
}

void Console::OnPlayerMove(uint8_t player)
{
   cout << "==================     PLAYER " << player + 1 << " TURN     =================" << endl;
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeHost)
   {
      // Waiting player 2 finish turn
      if(player == GC_PLAYER_2)
      {
         cout << "Waiting to player 2" << endl;
         HandleMessage();
         SetSelect(ConsoleNext);
         return;
      }
   }
   // Handle place card to battle for Host and Offline
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
      char confirm = 0;
      do
      {
         cout << "Select by Unit ID to place to the battle: ";
         cin >> select;
         Unit::UnitInfoType unitInfo;
         GameController::GetInstance()->GetUnitInformation(select, &unitInfo);
         if(!cin || find(unitOnHandList.begin(),unitOnHandList.end(),select) == unitOnHandList.end())
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
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }
      } while(confirm != 'y' && confirm != 'Y');
      GameController::GetInstance()->PlaceToBattle(player, (int)select);
   }
   // Send Done_Message to Client then End turn
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeHost)
   {
      Socket::SockMessage sockMessage;
      sockMessage.messageType = Socket::DONE_MESSAGE;
      Socket::GetInstance()->Send(&sockMessage, 0);
   }

   SetSelect(ConsoleNext);
}

void Console::OnClientMove(uint8_t player)
{
   cout << "==================     PLAYER " << player + 1 << " TURN     =================" << endl;
   // Waiting Play 1 finish turn
   if(player == GC_PLAYER_1)
   {
      cout << "Waiting to player 1" << endl;;
      HandleMessage();
   }
   // Clien pick a card to battle and send to Host to process
   else
   {
      Socket::SockMessage sockMessage;
      uint16_t messageLength;

      sockMessage.messageType = Socket::CARD_ON_HAND_LIST_ID_P2;
      Socket::GetInstance()->Send(&sockMessage,0);
      Socket::GetInstance()->Receive(&sockMessage, &messageLength);
      uint16_t size = messageLength;
      vector<uint8_t> unitOnHandIdList_p2(size);
      uint8_t index;

      memcpy(&unitOnHandIdList_p2[0], sockMessage.buffer, size);

      if(0 == unitOnHandIdList_p2.size())
      {
         cout << "There is no card to place. Press enter to continue..." << endl;
         cin.ignore();
      }
      else
      {
         // Request Info Unit On Hand P2
         sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
         size = unitOnHandIdList_p2.size();
         memcpy(sockMessage.buffer, &unitOnHandIdList_p2[0], size);
         Socket::GetInstance()->Send(&sockMessage, size);
         Socket::GetInstance()->Receive(&sockMessage,&messageLength);
         index = 0;
         for(auto id : unitOnHandIdList_p2)
         {
            Unit::UnitInfoType unitInfo;
            vector<uint8_t> tempBuffer(UNIT_INFO_SIZE);
            memcpy(&tempBuffer[0],&sockMessage.buffer[index*UNIT_INFO_SIZE],UNIT_INFO_SIZE);

            unitInfo.name = string((char*)&tempBuffer[0], UNIT_NAME_SIZE).c_str();
            // unitInfo.name.resize(unitInfo.name.find('\0'));
            memcpy(&unitInfo.health, &tempBuffer[UNIT_NAME_SIZE], UNIT_INFO_SIZE-UNIT_NAME_SIZE);
            DUMP_UNIT_INFO_S((int)id, unitInfo.name, unitInfo.health, unitInfo.damage, UnitTypeToStrMapping.at(unitInfo.type));
            index++;
         }

         int select;
         char confirm;
         do
         {
            cout << "Select by Unit ID to place to the battle: ";
            cin >> select;
            if(!cin ||
               find(unitOnHandIdList_p2.begin(),unitOnHandIdList_p2.end(),select) == unitOnHandIdList_p2.end())
            {
               cin.clear();
               cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
               cout << "Invalid ID, please correct select" << endl;
            }
            else
            {
               Unit::UnitInfoType unitInfo;
               sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
               sockMessage.buffer[0] = (uint8_t)select;
               Socket::GetInstance()->Send(&sockMessage, 1);
               Socket::GetInstance()->Receive(&sockMessage,&messageLength);
               unitInfo.name = string((char*)sockMessage.buffer, 30);
               unitInfo.name.resize(unitInfo.name.find('\0'));
               memcpy(&unitInfo.health, &sockMessage.buffer[30], messageLength-30);

               // GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
               DUMP_UNIT_INFO_S((int)select,
                     unitInfo.name,
                     unitInfo.health,
                     unitInfo.damage,
                     UnitTypeToStrMapping.at(unitInfo.type));
               cout << "Is put on battle?(y/n):";
               cin >> confirm;
               cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
         } while(confirm != 'y' && confirm != 'Y');
         // GameController::GetInstance()->PlaceToBattle(player, (int)select);
         sockMessage.messageType = Socket::CLIENT_PLACE_CARD;
         sockMessage.buffer[0] = (uint8_t)select;
         Socket::GetInstance()->Send(&sockMessage, 1);
      }
      Socket::SockMessage doneMessage;
      doneMessage.messageType = Socket::DONE_MESSAGE;
      Socket::GetInstance()->Send(&doneMessage, 0);
   }
   SetSelect(ConsoleNext);
}

void Console::AfterPlayerMove(uint8_t player)
{
   cout << "===============AFTERPLAYERMOVE=============" << endl;
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

   if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeHost)
   {
      Socket::SockMessage readyMessage;
      readyMessage.messageType = Socket::READY_MESSAGE;
      Socket::GetInstance()->Send(&readyMessage,0);
   }
}

void Console::AfterPlayerMoveOnClient()
{
   cout << "===============AfterPlayerMoveOnClient=============" << endl;
   Socket::SockMessage sockMessage;
   uint16_t messageLength;
   while(sockMessage.messageType != Socket::READY_MESSAGE)
   {
      Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   }
}

void Console::OnProcess()
{
   // system("clear");
   cout << "\033[H\033[J";
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

void Console::OnHostProcess()
{
   HandleMessage();
   OnProcess();
}

void Console::OnClientProcess()
{
   // system("clear");
   cout << "\033[H\033[J";
   cout << "====================================================================\n";

   Socket::SockMessage sockMessage;
   uint16_t messageLength;
   uint16_t size;
   list<uint8_t> heroIdList_p1;
   list<uint8_t> unitOnTableList_p1;
   list<uint8_t> heroIdList_p2;
   list<uint8_t> unitOnTableList_p2;

   sockMessage.messageType = Socket::HERO_LIST_ID_P1;
   Socket::GetInstance()->Send(&sockMessage,0);
   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   size = messageLength;
   while(size--)
   {
      heroIdList_p1.push_front(sockMessage.buffer[size]);
   }

   sockMessage.messageType = Socket::HERO_LIST_ID_P2;
   Socket::GetInstance()->Send(&sockMessage,0);
   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   size = messageLength;
   while(size--)
   {
      heroIdList_p2.push_front(sockMessage.buffer[size]);
   }

   sockMessage.messageType = Socket::CARD_ON_TABLE_LIST_ID_P1;
   Socket::GetInstance()->Send(&sockMessage,0);
   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   size = messageLength;
   while(size--)
   {
      unitOnTableList_p1.push_front(sockMessage.buffer[size]);
   }
   
   sockMessage.messageType = Socket::CARD_ON_TABLE_LIST_ID_P2;
   Socket::GetInstance()->Send(&sockMessage,0);
   Socket::GetInstance()->Receive(&sockMessage, &messageLength);
   size = messageLength;
   while(size--)
   {
      unitOnTableList_p2.push_front(sockMessage.buffer[size]);
   }

   if(heroIdList_p1.size() == 0)
   {
      if(heroIdList_p2.size() == 0)
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
      for(auto heroId_p1 : heroIdList_p1)
      {
         Unit::UnitInfoType unitInfo;
         sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
         sockMessage.buffer[0] = (uint8_t)heroId_p1;
         Socket::GetInstance()->Send(&sockMessage, 1);
         Socket::GetInstance()->Receive(&sockMessage,&messageLength);
         unitInfo.name = string((char*)sockMessage.buffer, 30);
         unitInfo.name.resize(unitInfo.name.find('\0'));
         memcpy(&unitInfo.health, &sockMessage.buffer[30], messageLength-30);

         // GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
         DUMP_UNIT_INFO_S((int)heroId_p1,
               unitInfo.name,
               unitInfo.health,
               unitInfo.damage,
               UnitTypeToStrMapping.at(unitInfo.type));
      }
      for(auto unitOnTable_p1 : unitOnTableList_p1)
      {
         Unit::UnitInfoType unitInfo;
         sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
         sockMessage.buffer[0] = (uint8_t)unitOnTable_p1;
         Socket::GetInstance()->Send(&sockMessage, 1);
         Socket::GetInstance()->Receive(&sockMessage,&messageLength);
         unitInfo.name = string((char*)sockMessage.buffer, 30);
         unitInfo.name.resize(unitInfo.name.find('\0'));
         memcpy(&unitInfo.health, &sockMessage.buffer[30], messageLength-30);

         // GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
         DUMP_UNIT_INFO_S((int)unitOnTable_p1,
               unitInfo.name,
               unitInfo.health,
               unitInfo.damage,
               UnitTypeToStrMapping.at(unitInfo.type));         
      }
   }

   cout << "====================================================================\n";
   cout.width(30);cout << left << "||" << "ROUND ";
   cout.width(30);cout << left << GameController::GetInstance()->GetCurrentRound()/2;
   cout << "||" << endl;
   cout << "====================================================================\n";

   if(heroIdList_p2.size() == 0)
   {
      cout << "PLAYER 1 WIN" << endl;
      SetExit();
   }
   else
   {
      for(auto heroId_p2 : heroIdList_p2)
      {
         Unit::UnitInfoType unitInfo;
         sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
         sockMessage.buffer[0] = (uint8_t)heroId_p2;
         Socket::GetInstance()->Send(&sockMessage, 1);
         Socket::GetInstance()->Receive(&sockMessage,&messageLength);
         unitInfo.name = string((char*)sockMessage.buffer, 30);
         unitInfo.name.resize(unitInfo.name.find('\0'));
         memcpy(&unitInfo.health, &sockMessage.buffer[30], messageLength-30);

         // GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
         DUMP_UNIT_INFO_S((int)heroId_p2,
               unitInfo.name,
               unitInfo.health,
               unitInfo.damage,
               UnitTypeToStrMapping.at(unitInfo.type));
      }
      for(auto unitOnTable_p2 : unitOnTableList_p2)
      {
         Unit::UnitInfoType unitInfo;
         sockMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
         sockMessage.buffer[0] = (uint8_t)unitOnTable_p2;
         Socket::GetInstance()->Send(&sockMessage, 1);
         Socket::GetInstance()->Receive(&sockMessage,&messageLength);
         unitInfo.name = string((char*)sockMessage.buffer, 30);
         unitInfo.name.resize(unitInfo.name.find('\0'));
         memcpy(&unitInfo.health, &sockMessage.buffer[30], messageLength-30);

         // GameController::UnitBonusType* b = GameController::GetInstance()->GetBonusById(player,select);
         DUMP_UNIT_INFO_S((int)unitOnTable_p2,
               unitInfo.name,
               unitInfo.health,
               unitInfo.damage,
               UnitTypeToStrMapping.at(unitInfo.type));         
      }
   }
   cout << "====================================================================" << endl;
   
   Socket::SockMessage doneMessage;
   doneMessage.messageType = Socket::DONE_MESSAGE;
   Socket::GetInstance()->Send(&doneMessage, 0);

   SetSelect(ConsoleNext);
   // usleep(500000);
}

void Console::OnEnd()
{
   // system("clear");
   cout << "\033[H\033[J";
   cout << "=========== END ============" << endl;
   cout << "Press enter to continue..." << endl;
   cin.ignore();
}

void Console::HandleMessage()
{
   uint16_t messageLength;
   Socket::SockMessage revMessage;
   Socket::SockMessage sendMessage;

   do
   {
      Socket::GetInstance()->Receive(&revMessage, &messageLength);
      switch(revMessage.messageType)
      {
         case Socket::HERO_LIST_ID_P1:
         {
            auto heroListId_p1 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_1);
            vector<uint8_t> temp;
            copy(begin(heroListId_p1),end(heroListId_p1),back_inserter(temp));
            memcpy(sendMessage.buffer,&temp[0],temp.size());
            sendMessage.messageType = Socket::HERO_LIST_ID_P1;
            Socket::GetInstance()->Send(&sendMessage, temp.size());
         }
         break;
         case Socket::HERO_LIST_ID_P2:
         {
            auto heroListId_p2 = GameController::GetInstance()->GetHeroIdList((uint8_t)GC_PLAYER_2);
            vector<uint8_t> temp;
            copy(begin(heroListId_p2),end(heroListId_p2),back_inserter(temp));
            memcpy(sendMessage.buffer,&temp[0],temp.size());
            sendMessage.messageType = Socket::HERO_LIST_ID_P2;
            Socket::GetInstance()->Send(&sendMessage, temp.size());            
         }
         break;
         case Socket::CARD_ON_TABLE_LIST_ID_P1:
         {
            auto unitOnTableIdList_p1 = GameController::GetInstance()->GetUnitOnTableIdList((uint8_t)GC_PLAYER_1);
            vector<uint8_t> temp;
            copy(begin(unitOnTableIdList_p1),end(unitOnTableIdList_p1),back_inserter(temp));
            memcpy(sendMessage.buffer,&temp[0],temp.size());
            sendMessage.messageType = Socket::CARD_ON_TABLE_LIST_ID_P1;
            Socket::GetInstance()->Send(&sendMessage, temp.size());
         }
         break;
         case Socket::CARD_ON_TABLE_LIST_ID_P2:
         {
            auto unitOnTableIdList_p2 = GameController::GetInstance()->GetUnitOnTableIdList((uint8_t)GC_PLAYER_2);
            vector<uint8_t> temp;
            copy(begin(unitOnTableIdList_p2),end(unitOnTableIdList_p2),back_inserter(temp));
            memcpy(sendMessage.buffer,&temp[0],temp.size());
            sendMessage.messageType = Socket::CARD_ON_TABLE_LIST_ID_P2;
            Socket::GetInstance()->Send(&sendMessage, temp.size());
         }
         break;
         case Socket::CARD_ON_HAND_LIST_ID_P2:
         {
            auto unitOnHandIdList_p2 = GameController::GetInstance()->GetUnitOnHandIdList((uint8_t)GC_PLAYER_2);
            vector<uint8_t> temp;
            copy(begin(unitOnHandIdList_p2),end(unitOnHandIdList_p2),back_inserter(temp));
            memcpy(sendMessage.buffer,&temp[0],temp.size());
            sendMessage.messageType = Socket::CARD_ON_HAND_LIST_ID_P2;
            Socket::GetInstance()->Send(&sendMessage, temp.size());
         }
         break;
         case Socket::CLIENT_REQUEST_UNIT_INFO:
         {   
            for(int i=0;i<messageLength;i++)
            {
               uint8_t id = revMessage.buffer[i];
               Unit::UnitInfoType unitInfo;
               GameController::GetInstance()->GetUnitInformation(id,&unitInfo);
               memcpy(&sendMessage.buffer[UNIT_INFO_SIZE*i], unitInfo.name.c_str(), unitInfo.name.size());
               memcpy(&sendMessage.buffer[UNIT_INFO_SIZE*i+UNIT_NAME_SIZE], &unitInfo.health, UNIT_INFO_SIZE-UNIT_NAME_SIZE);
            }
            sendMessage.messageType = Socket::CLIENT_REQUEST_UNIT_INFO;
            Socket::GetInstance()->Send(&sendMessage,UNIT_INFO_SIZE*messageLength);
         }   
         break;
         case Socket::CLIENT_PLACE_CARD:
            GameController::GetInstance()->PlaceToBattle(GC_PLAYER_2, revMessage.buffer[0]);
         break;
         default:
         break;
      }
   }while(revMessage.messageType != Socket::DONE_MESSAGE);
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
   if(GameController::GetInstance()->GetGamePlayType() == GameController::GameTypeHost)
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
   if(GameController::GameTypeClient != GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->OnPlayerMove(GC_PLAYER_1);
   }
   else
   {
      sm.getActionInstance()->OnClientMove(GC_PLAYER_1);
   }
}
void ConsoleState_OnPlayer1Turn::next(ConsoleStateMachine &sm)
{
   if(GameController::GameTypeClient != GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->AfterPlayerMove(GC_PLAYER_1);
   }
   else
   {
      sm.getActionInstance()->AfterPlayerMoveOnClient();
   }
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
   if(GameController::GameTypeClient != GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->OnPlayerMove(GC_PLAYER_2);
   }
   else
   {
      sm.getActionInstance()->OnClientMove(GC_PLAYER_2);
   }
}
void ConsoleState_OnPlayer2Turn::next(ConsoleStateMachine &sm)
{
   if(GameController::GameTypeClient != GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->AfterPlayerMove(GC_PLAYER_2);
   }
   else
   {
      sm.getActionInstance()->AfterPlayerMoveOnClient();
   }
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
   if(GameController::GameTypeHost == GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->OnHostProcess();
   }
   else if (GameController::GameTypeClient == GameController::GetInstance()->GetGamePlayType())
   {
      sm.getActionInstance()->OnClientProcess();
   }
   else
   {
      sm.getActionInstance()->OnProcess();
   }

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