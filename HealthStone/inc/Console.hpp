#ifndef _CONSOLE_HPP_INC_
#define _CONSOLE_HPP_INC_

class Console
{
   enum ConsoleSM
   {
      ConsoleSM_Start,
      ConsoleSM_Running,
      ConsoleSM_End
   };

public:
   Console(){consoleSM = ConsoleSM_Start;};
   ~Console(){};
   void MainFunction(void);
   void GenerateStart(void);
   void OnRunning(void);
   void GenerateEnd(void);
private:
   static ConsoleSM consoleSM;
};

#endif