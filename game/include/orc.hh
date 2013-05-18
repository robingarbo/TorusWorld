//
// coplien.hh for project in /home/robin
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Mon Apr 22 20:03:04 2013 robin garbo
// ##Last update Fri Apr 26 02:44:56 2013 robin garbo
//

#ifndef _DEF_ORC
#define _DEF_ORC


#include <iostream>
#include <string>
#include <list>
#include "character.hh"


class Orc : public Character
{
public:
  Orc();
  Orc(const Orc&); //copy constructor
  ~Orc();
  Orc &operator=(const Orc&); //affectation operator
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
