//
// coplien.hh for project in /home/robin
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Mon Apr 22 20:03:04 2013 robin garbo
// ##Last update Tue Apr 30 04:35:26 2013 robin garbo
//

#ifndef _DEF_CHARACTER
#define _DEF_CHARACTER

#include <iostream>
#include <string>
#include <list>



class Character
{
public:
  Character(){};
  Character(const Character&); //copy constructor
  ~Character(){};
  Character &operator=(const Character&); //affectation operator
  void GetDammage(int dammage);
  void attack(Character &target);
  void addStuff(char *stuff, int value);
  bool isAlive();
private:
  std::string name;
  int m_life;
  int m_mana;
  std::list<std::string> m_stuff;
  int m_dammage;
  int m_armor;
  int m_magic_resist;
  int m_regen_life_point;
  bool m_alive;
};

#endif
