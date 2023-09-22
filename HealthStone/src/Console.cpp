#include "Console.hpp"
#include <iostream>

using namespace std;

void Console::GenerateStart()
{
   cout << "===========HEALTH STONE============" << endl;
   cout << "Press enter to continue...";
   cin.ignore();
   SetSelect(ConsoleNext);
}

void Console::OnRunning()
{
   cout << "===========On Running============" << endl;
   cout << "Press enter to continue...";
   cin.ignore();
   SetSelect(ConsoleNext);
}

void Console::GenerateEnd()
{
   cout << "=========== END ============" << endl;
   cout << "Press enter to continue...";
   cin.ignore();
   isExit = true;
}

void ConsoleState::setState(ConsoleStateMachine &sm, ConsoleState *state)
{
   ConsoleState *preState = sm.consoleState;
   sm.consoleState = state;
   sm.entry();
   delete preState;
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

void ConsoleState_StartUp::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->GenerateStart();
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

void ConsoleState_OnRunning::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->OnRunning();
}
void ConsoleState_OnRunning::next(ConsoleStateMachine &sm)
{
   setState(sm, new ConsoleState_End());
}
void ConsoleState_OnRunning::back(ConsoleStateMachine &sm)
{
   return;
}
void ConsoleState_OnRunning::select(ConsoleStateMachine &sm, uint8_t option)
{
   return;
}

void ConsoleState_End::entry(ConsoleStateMachine &sm)
{
   sm.getActionInstance()->SetSelect(Console::ConsoleNotSet);
   sm.getActionInstance()->GenerateEnd();
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