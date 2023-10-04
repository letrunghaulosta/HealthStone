#ifndef _CONSOLE_HPP_INC_
#define _CONSOLE_HPP_INC_

#include <cstdint>
#include <functional>
class ConsoleStateMachine;

class Console
{
public:

   enum ConsoleSelectedType
   {
      ConsoleNotSet = 0,
      ConsoleNext,
      ConsoleBack,
      ConsoleSelect
   };

   Console(){
      selected = ConsoleNotSet;
      isExit = false;
   }
   ~Console(){};
   void OnStart(void);
   void OnLoading(void);
   void OnClientLoading(void);
   void OnPlayerMove(uint8_t);
   void AfterPlayerMove(uint8_t);
   void OnProcess();
   void OnEnd(void);
   void OnHost(void);
   void OnClient(void);
   void SetSelect(ConsoleSelectedType s){selected = s;}
   ConsoleSelectedType GetSelected(){return selected;}
   bool IsExit(){return isExit;}
   void SetExit(){isExit = true;}

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
protected:
   ConsoleState* GetParent(){return this;}
};

class ConsoleState_StartUp : public ConsoleState_Root
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_SetupConnection : public ConsoleState_Root
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_HostSetup : public ConsoleState_SetupConnection
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_ClientSetup : public ConsoleState_SetupConnection
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_OnRunning : public ConsoleState_Root
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_OnPlayer1Turn : public ConsoleState_OnRunning
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_OnPlayer2Turn : public ConsoleState_OnRunning
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_OnProcess : public ConsoleState_OnRunning
{
public:
   virtual void entry(ConsoleStateMachine &sm);
   virtual void next(ConsoleStateMachine &sm);
   virtual void back(ConsoleStateMachine &sm);
   virtual void select(ConsoleStateMachine &sm, uint8_t option);
};

class ConsoleState_End : public ConsoleState_Root
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
   ConsoleState *prevState;
   Console      *console;
};

#endif