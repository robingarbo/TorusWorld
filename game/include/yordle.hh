//
// coplien.hh for project in /home/robin
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Mon Apr 22 20:03:04 2013 robin garbo
// ##Last update Fri Apr 26 02:47:36 2013 robin garbo
//

#ifndef _DEF_YORDLE
#define _DEF_YORDLE


#include <iostream>
#include <string>
#include <list>
#include "character.hh"


class Yorlde : public Character
{
public:
  Yorlde();
  Yorlde(const Yorlde&); //copy constructor
  ~Yorlde();
  Yorlde &operator=(const Yorlde&); //affectation operator
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
