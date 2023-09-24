#include "Console.hpp"
#include "Hero.hpp"

#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>

class HighlightHelper
{
public:
   HighlightHelper(){select = 0;}
   ~HighlightHelper(){}
   void addContent(std::string str)
   {
      hlhQueue.push_back(str);
   }
   void show()
   {
      for(auto it = hlhQueue.begin(); it != hlhQueue.end(); it++)
      {
         if((it - hlhQueue.begin()) == select)
         {
            std::cout << "> ";
         }
         std::cout << *it;
      }
   }
   
   void updateUp()
   {
      if(select == 0)
      {
         return;
      }
      
      update(this->select-1);
   }
   
   void updateDown()
   {
      if(select >= hlhQueue.size())
      {
         return;
      }
      
      update(this->select+1);
   }
   
   void update(unsigned char newSelect)
   {
      if(newSelect == select || newSelect >= hlhQueue.size())
      {
         return;   
      }
      
      // auto it = hlhQueue[select].begin();
      // hlhQueue[select].erase(it,it+2);
      
      // hlhQueue[newSelect].insert(0,"> ");
      select = newSelect;
   }
   std::string sendSelected()
   {
      std::cout << "SELECT: " << hlhQueue[select];
      return hlhQueue[select];
   }
private:
   std::vector<std::string> hlhQueue;
   unsigned char select;
};



int main()
{
   // Console console;
   // ConsoleStateMachine consoleSM(&console);
   // consoleSM.next();
   // while(!console.IsExit())
   // {
      // switch(console.GetSelected())
      // {
         // case Console::ConsoleNext:
            // consoleSM.next();
            // break;
         // case Console::ConsoleBack:
            // consoleSM.back();
            // break;
         // default:
            // break;
      // }
   // }
   int c = 0;
   HighlightHelper hlHelper;
   hlHelper.addContent("Test1\r\n");
   hlHelper.addContent("Test2\r\n");
   hlHelper.addContent("Test3\r\n");
   initscr();
   raw();
   keypad(stdscr, true);
   noecho();
   while(c != 'q')
   {
      c=0;
      refresh();
      system("clear");
      hlHelper.show();
      switch(c = getch())
      {
         case KEY_UP:
            hlHelper.updateUp();
         break;
         case KEY_DOWN:
            hlHelper.updateDown();
         break;
         case '\n':
            c = 'q';
            hlHelper.sendSelected();
         break;
      }
   }
   getch();
   endwin();
   return 0;
}