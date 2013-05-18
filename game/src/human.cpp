//
// human.cpp for human in /home/robin/MonJeuxDeFantasie/games
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Fri Apr 26 02:22:11 2013 robin garbo
// ##Last update Fri Apr 26 02:38:49 2013 robin garbo
//


#include "human.hh"

Human::Human()
{
  m_life = 500;
  m_mana = 500;
  m_dammage = 50;
  m_armor = 10;
  m_magic_resist = 10;
  m_regen_life_point = 1;
  m_alive = true;
}

Human::~Human()
{
  std::cout << "Human is die" << std::endl;
}

void Human::GetDammage(int dammage)
{
  m_life = m_life - dammage;
}

bool Human::isAlive()
{
  if (m_life >= 0)
    return false;
  return true;
}
