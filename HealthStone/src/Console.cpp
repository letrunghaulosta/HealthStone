#include "Console.hpp"
#include <iostream>

using namespace std;

void Console::GenerateStart()
{
   cout << "===========HEALTH STONE============" << endl;
}

void Console::MainFunction()
{
   switch(consoleSM)
   {
      case ConsoleSM_Start:
         GenerateStart();
      break;
      case ConsoleSM_Running:
         OnRunning();
      break;
      case ConsoleSM_End:
         GenerateEnd();
      break;
   }
}