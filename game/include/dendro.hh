//
// coplien.hh for project in /home/robin
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Mon Apr 22 20:03:04 2013 robin garbo
// ##Last update Fri Apr 26 02:43:30 2013 robin garbo
//

#ifndef _DEF_DENDRO
#define _DEF_DENDRO


#include <iostream>
#include <string>
#include <list>
#include "character.hh"


class Dendro : public Character
{
public:
  Dendro();
  Dendro(const Dendro&); //copy constructor
  ~Dendro();
  Dendro &operator=(const Dendro&); //affectation operator
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
