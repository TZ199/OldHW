//Tianxin Zhou
//Start date:04/23
//For hw 10
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



class Pokemon{
public:
//Constructor
Pokemon(const std::map<std::string,std::vector<std::string> > &facts){
  std::map<std::string,std::vector<std::string> >::const_iterator find;
  find = facts.find("eggGroups");

  //Assume we must have eggGroups as the item in index.
  //Input every section from the data.
  eggGroups = find->second;
  find = facts.find("label");
  label = find->second[0];
  find = facts.find("genderThreshold");
  std::string myString = find->second[0];
  genderThreshold = atoi(myString.c_str());
  find = facts.find("baseExpYield");
  myString = find->second[0];
  baseExpYield = atoi(myString.c_str());
  find = facts.find("abilities");
  ability = find->second[0];
  find = facts.find("species");
  species = find->second[0];
  find = facts.find("expGroup");
  expGroup = find->second[0];


}
//Destructor
virtual ~Pokemon() {};
//Return the label.
std::string getLabel();
//Check if the pokemon is in the group.
bool in_groups(const std::string & group);
//Check if two pokemon share one egggroup.
bool SharesEggGroup(const Pokemon* b);
//Protected variables.
protected:
  std::vector<std::string> eggGroups;
  std::string label;
  std::string species;
  //To check the pokemon.
  int baseExpYield;
  int genderThreshold;
  std::string ability;
  std::string expGroup;

};



//All egggroups
class Amorphous : public Pokemon{
public:
  Amorphous(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Amorphous"))
      throw 0;
  }
};

class Bug : public Pokemon{
public:
  Bug(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Bug"))
      throw 0;
  }

};
//virtual group since one pokemon in two egggroups.
class Dragon : virtual public Pokemon{
public:
  Dragon(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Dragon"))
      throw 0;
  }
};
//virtual group since one pokemon in two egggroups.
class Fairy : virtual public Pokemon{
public:
  Fairy(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Fairy"))
      throw 0;
  }
};
//virtual group since one pokemon in two egggroups.
class Field : virtual public Pokemon{
public:
  Field(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Field"))
      throw 0;
  }
};

class Flying : public Pokemon{
public:
  Flying(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Flying"))
      throw 0;
  }
};

class Grass : public Pokemon{
public:
  Grass(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Grass"))
      throw 0;
  }
};


class HumanLike : public Pokemon{
public:
  HumanLike(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("HumanLike"))
      throw 0;
  }
};

class Mineral : public Pokemon{
public:
  Mineral(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Mineral"))
      throw 0;
  }
};

class Monster : public Pokemon{
public:
  Monster(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Monster"))
      throw 0;
  }
};

class Water1 : public Pokemon{
public:
  Water1(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Water1"))
      throw 0;
  }
};

class Water2 : public Pokemon{
public:
  Water2(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Water2"))
      throw 0;
  }
};

class Water3 : public Pokemon{
public:
  Water3(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Water3"))
      throw 0;
  }
};

class Ditto : public Pokemon{
public:
  Ditto(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Ditto"))
      throw 0;
  }
};

class Undiscovered : public Pokemon{
public:
  Undiscovered(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts)
  {
    if(!in_groups("Undiscovered"))
      throw 0;
  }
};

//Pokemon -> Fairy, Field -> Snubbull
class Snubbull : public Fairy, public Field{
public:
  Snubbull(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts), Fairy(facts), Field(facts)
  {
    if(ability!="70" || genderThreshold != 191)
      throw 0;
  }
};
//Pokemon -> Snubbull -> Granbull
class Granbull : public Snubbull{
public:
  Granbull(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts), Snubbull(facts)
  {
    if(baseExpYield < 158)
      throw 0;
  }
};
//Pokemon-> Fairy -> Spritzee
class Spritzee : public Fairy{
public:
  Spritzee(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts), Fairy(facts)
  {
    if(ability!="54" || genderThreshold != 127|| expGroup!="2")
      throw 0;
  }
};

//Pokemon -> Spritzee -> Aromatisse
class Aromatisse : public Spritzee{
public:
  Aromatisse(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts), Spritzee(facts)
  {
    if(baseExpYield < 162 || species!="Fragrance")
      throw 0;
  }
};

//Pokemon -> Field, Dragon -> Seviper
class Seviper : public Field, public Dragon
{
  public:
    Seviper(const std::map<std::string,std::vector<std::string> > &facts) : Pokemon(facts), Field(facts),Dragon(facts)
    {
      if(ability!="134" || genderThreshold != 127|| expGroup!="6" ||species!="FangSnake")
        throw 0;
    }
};

//Pokemon -> Mineral -> Trubbish
class Trubbish : public Mineral
{
  public:
    Trubbish(const std::map<std::string,std::vector<std::string> > &facts) : Mineral(facts)
    {
      if(ability!="152" || genderThreshold != 127)
        throw 0;
    }
};

//Pokemon -> Trubbish -> Garbodor
class Garbodor : public Trubbish{
public:
  Garbodor(const std::map<std::string,std::vector<std::string> > &facts) : Trubbish(facts)
  {
    if(baseExpYield < 166)
      throw 0;
  }
};

//Pokemon ->> Flying -> Zubat
class Zubat : public Flying
{
  public:
    Zubat(const std::map<std::string,std::vector<std::string> > &facts) : Flying(facts)
    {
      if(ability!="68" || genderThreshold != 127)
        throw 0;
    }
};

//Pokemon -> Zubat -> Golbat
class Golbat : public Zubat{
public:
  Golbat(const std::map<std::string,std::vector<std::string> > &facts) : Zubat(facts)
  {
    if(baseExpYield < 159)
      throw 0;
  }
};

//Pokemon -> Field -> Dunsparce
class Dunsparce : public Field
{
  public:
    Dunsparce(const std::map<std::string,std::vector<std::string> > &facts) :Pokemon(facts), Field(facts)
    {
      if(ability!="132" || genderThreshold != 127)
        throw 0;
    }
};
