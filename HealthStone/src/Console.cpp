#include "Console.hpp"
#include "GameController.hpp"
#include <iostream>

using namespace std;
void Console_NotifyEnd()
{
   
}

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
   cout << "Press enter to continue..." << endl;
   cin.ignore();
   SetSelect(ConsoleNext);
}

void Console::OnPlay(int8_t player)
{
   system("clear");
   cout << "===========PLAYER " << player + 1 << " TURN============\n";

   UnitInfoType unitInfo;
   GameController::GetInstance()->GetUnitInformation();
   cout << "Press enter to continue..." << endl;
   cin.ignore();
   SetSelect(ConsoleNext);
}
void Console::OnProcess()
{
   system("clear");
   cout << "===========On Process============\n";
   cout << "Press enter to continue..." << endl;
   cin.ignore();

}

void Console::OnEnd()
{
   system("clear");
   cout << "=========== END ============" << endl;
   cout << "Press enter to continue..." << endl;
   cin.ignore();
   isExit = true;
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
   sm.getActionInstance()->OnPlay(GC_PLAYER_1);
}
void ConsoleState_OnPlayer1Turn::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_OnProcess());
}
void ConsoleState_OnPlayer1Turn::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnPlayer1Turn::select(ConsoleStateMachine &sm, uint8_t option)
{
   return this->GetParent()->select(sm,option);
}
/***ConsoleState_OnPlayer2Turn**/
void ConsoleState_OnPlayer2Turn::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnPlay(GC_PLAYER_2);
}
void ConsoleState_OnPlayer2Turn::next(ConsoleStateMachine &sm)
{
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