#ifndef _CONSOLE_HPP_INC_
#define _CONSOLE_HPP_INC_

#include <cstdint>

class ConsoleStateMachine;

class Console
{
public:

   enum ConsoleSelectedType
   {
      ConsoleNotSet,
      ConsoleNext,
      ConsoleBack
   };

   Console(){
      selected = ConsoleNotSet;
      isExit = false;
      }
   ~Console(){};
   void GenerateStart(void);
   void OnRunning(void);
   void GenerateEnd(void);
   void SetSelect(ConsoleSelectedType s){selected = s;}
   ConsoleSelectedType GetSelected(){return selected;}
   bool IsExit(){return isExit;}
private:
   ConsoleSelectedType selected;
   bool isExit;
};

class ConsoleState
{
public:
   virtual void entry(ConsoleStateMachine &sm) = 0;
   virtual void next(ConsoleStateMachine &sm) = 0;
   virtual void back(ConsoleStateMachine &sm) = 0;
   virtual void select(ConsoleStateMachine &sm, uint8_t option) = 0;
protected:
   void setState(ConsoleStateMachine &sm, ConsoleState *state);
};

class ConsoleState_Root : public ConsoleState
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
   ConsoleState_Root(){};
   ~ConsoleState_Root(){};
};

class ConsoleState_StartUp : public ConsoleState
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_OnRunning : public ConsoleState
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_End : public ConsoleState
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleStateMachine
{
   friend class ConsoleState;
public:
   ConsoleStateMachine(Console *c){
      this->console = c;
      consoleState = static_cast<ConsoleState *>(new ConsoleState_Root());}
   void entry(){consoleState->entry(*this);}
   void next(){consoleState->next(*this);}
   void back(){consoleState->back(*this);}
   void select(uint8_t option){consoleState->select(*this,option);}
   Console* getActionInstance(){return console;}
private:
   ConsoleState *consoleState;
   Console      *console;
};

#endif