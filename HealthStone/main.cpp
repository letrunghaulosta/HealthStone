#include "Console.hpp"
#include "Hero.hpp"

#include <iostream>
#include <string>
Console console;

int main()
{
   console.GenerateHeader();
   Slark slarkObj;
   Butcher butcherObj;
   Hero* hero = &slarkObj;
   std::string test;

   std::cout << "Name: " << slarkObj.getName() << std::endl;
   std::cout << "Health: " << slarkObj.getHealth() << std::endl;
   std::cout << "Damage: " << slarkObj.getDamage() << std::endl;
   std::cout << "================================================" << std::endl;
   std::cout << "Name: " << butcherObj.getName() << std::endl;
   std::cout << "Health: " << butcherObj.getHealth() << std::endl;
   std::cout << "Damage: " << butcherObj.getDamage() << std::endl;
   std::cout << "===================SLARK attack==================" << std::endl;
   butcherObj.updateHealth(slarkObj.getDamage());
   std::cout << "Name: " << butcherObj.getName() << std::endl;
   std::cout << "Health: " << butcherObj.getHealth() << std::endl;
   std::cout << "Damage: " << butcherObj.getDamage() << std::endl;
   std::cout << "===================BUTCHER attack================" << std::endl;
   slarkObj.updateHealth(butcherObj.getDamage());
   std::cout << "Name: " << slarkObj.getName() << std::endl;
   std::cout << "Health: " << slarkObj.getHealth() << std::endl;
   std::cout << "Damage: " << slarkObj.getDamage() << std::endl;

   std::cout << "Name: " << slarkObj.getName() << std::endl;
   std::cout << "Health: " << slarkObj.getHealth() << std::endl;
   std::cout << "Damage: " << slarkObj.getDamage() << std::endl;

   return 0;
}