//Tianxin Zhou
//Added Custom case 2/01/2017
//Last edited 2/02/2017 for comments
#include "Champion.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip> 
#include <algorithm>
Champion::Champion()
{
    lose = 0;
    win = 0;
    winrate = 0;
    name = "TBD";
    minion = 0;
    
}
Champion::Champion(std::string aname, int check)
{
    name = aname;
    if (check == 1)
    {
        win = 1;
        lose = 0;
    }
    else
    {
        lose = 1;
        win = 0;
    }
    winrate = 0;
    minion = 0;
}
std::string Champion::getName() const
{
    return name;
}


void Champion::addWin()
{
    win++;
}

void Champion::addLose()
{
    lose++;
}

void Champion::addMinion()
{
    minion++;
}

int Champion::getLose() const{
    return lose;
}


int Champion::getWin() const{
    return win;
}

float Champion::getWinrate() const
{
    return winrate;
}

int Champion::getMinion() const
{
    return minion;
}

std::string Champion::getPeople() const{
    return people;
}

//Set the player in current match
void Champion::setPeople(std::string apeople)
{
    people = apeople;
}

//Set the winrate here
void Champion::setWinrate()
{
    if(lose == 0)
    {
        winrate = 1;

    }
    else
    {
        winrate = float(win) / (float(win)+float(lose));

    }
}



//This is for champions
//The sort priority is winrate > kill > death > name.
bool c_sortHelper (const Champion& Champion1, const Champion& Champion2)
{
    return Champion1.getWinrate() > Champion2.getWinrate() || 
    (Champion1.getWinrate() == Champion2.getWinrate() && Champion1.getWin() > Champion2.getWin()) ||
    (Champion1.getWinrate() == Champion2.getWinrate() && Champion1.getWin() == Champion2.getWin() && Champion1.getLose() < Champion2.getLose()) ||
    (Champion1.getWinrate() == Champion2.getWinrate() && Champion1.getWin() == Champion2.getWin() && Champion1.getLose() == Champion2.getLose() && Champion1.getName() < Champion2.getName());
}
