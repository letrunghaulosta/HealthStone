#include "Console.hpp"
#include "Hero.hpp"
#include "HighlightHelper.hpp"
#include <iostream>
#include <string>
#include <vector>

int main()
{
   Console console;
   ConsoleStateMachine consoleSM(&console);
   consoleSM.next();
   while(!console.IsExit())
   {
      switch(console.GetSelected())
      {
         case Console::ConsoleNext:
            consoleSM.next();
            break;
         case Console::ConsoleBack:
            consoleSM.back();
            break;
         case Console::ConsoleSelect:
            consoleSM.select(1);
            std::cout << "PASS SELECT IN MAIN" << std::endl;
            break;
         default:
            break;
      }
   }
   int c = 0;
   // HighlightHelper hlHelper;
   // hlHelper.addContent("Test1\r\n");
   // hlHelper.addContent("Test2\r\n");
   // hlHelper.addContent("Test3\r\n");
   // int result = hlHelper.stream();
   // std::cout << "Select: " << result << std::endl;
   return 0;
}