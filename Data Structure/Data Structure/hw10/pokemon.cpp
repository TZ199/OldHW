//For hw 10
//Tianxin Zhou
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>
#include "pokemon.h"

//The implemtation for getlabel to return the label.
std::string Pokemon::getLabel()
{
  return label;
}

//implemtation od in_group to check if the pokemon in the group.
bool Pokemon::in_groups(const std::string & group)
{
  //A loop to go through all groups.
  for(int i = 0; i< eggGroups.size(); i++)
  {
    if(eggGroups[i] == group)
    {  return true;
    }
  }
  return false;
}

//implemtation of SharesEggGroup  to check if two pokemons share on group.
bool Pokemon::SharesEggGroup(const Pokemon* b)
{
  //Double loop to check each group.
  for(int i = 0; i< eggGroups.size(); i++)
  {
    for(int j = 0; j < b->eggGroups.size(); j++)
    {
      if(eggGroups[i] == b->eggGroups[j])
        return true;
    }
  }
  return false;
}
