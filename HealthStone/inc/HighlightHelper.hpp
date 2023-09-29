#ifndef _HIGHLIGHTHELPER_HPP_INC_
#define _HIGHLIGHTHELPER_HPP_INC_

#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>

class HighlightHelper
{
public:
   HighlightHelper(){
    select = 0;
   }

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

   int stream()
   {
        int c = 0;
        initscr();
        raw();
        keypad(stdscr, true);
        noecho();
        while(c != '\n')
        {
            c=0;
            refresh();
            system("clear");
            show();
            switch(c = getch())
            {
                case KEY_UP:
                    updateUp();
                break;
                case KEY_DOWN:
                    updateDown();
                break;
            }
        }
        endwin();
        return getSelected();
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
      select = newSelect;
   }
   uint8_t getSelected()
   {
      return select;
   }

   void clear()
   {
      hlhQueue.clear();
   }


private:
   std::vector<std::string> hlhQueue;
   unsigned char select;
};

#endif