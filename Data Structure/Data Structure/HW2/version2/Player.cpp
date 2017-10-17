//Tianxin Zhou
//Added Custom case 2/01/2017
//Last edited 2/2/2017 for the comments
#include "Player.h"
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
Player::Player()
{
    death = 0;
    kill = 0;
    kdr = 0;
    name = "TBD";
    
}
Player::Player(std::string aname, std::string achampion)
{
    name = aname;
    champion.push_back(achampion);
    death = 0;
    kill = 0;
    kdr = 0;
    latekill = 0;
}
std::string Player::getName() const
{
    return name;
}
void Player::addChampion(std::string achampion)
{
    champion.push_back(achampion);
    sort( champion.begin(), champion.end() );
    champion.erase( unique( champion.begin(), champion.end() ), champion.end() );
}

void Player::addKill()
{
    kill++;
}

void Player::addDeath()
{
    death++;
}

void Player::addLatekill()
{
    latekill++;
}

int Player::getLatekill() const
{
    return latekill;
}
int Player::getDeath() const{
    return death;
}


int Player::getKill() const{
    return kill;
}

float Player::getKdr() const
{
    return kdr;
}

std::string Player::getChampion() const
{
    
    std::string output;
    output = champion[0];
    for(int i = 1; i<champion.size();i++)
    {
        output += ", ";
        output += champion[i];
    }
    return output;
}

void Player::setKdr()
{
    if(death == 0)
    {
        kdr = float(kill);

    }
    else
    {
        kdr = float(kill) / float(death);

    }
}


//This is for players
//The sort priority is kdr > kill > death > name.
bool sortHelper (const Player& player1, const Player& player2)
{
    return player1.getKdr() > player2.getKdr() || 
    (player1.getKdr() == player2.getKdr() && player1.getKill() > player2.getKill()) ||
    (player1.getKdr() == player2.getKdr() && player1.getKill() == player2.getKill() && player1.getDeath() < player2.getDeath()) ||
    (player1.getKdr() == player2.getKdr() && player1.getKill() == player2.getKill() && player1.getDeath() == player2.getDeath() && player1.getName() < player2.getName());
}


//This is for custom 
//The sort priority is late kill > kill > death > name.
bool sortHelper2 (const Player& player1, const Player& player2)
{
    return player1.getLatekill() > player2.getLatekill() || 
    (player1.getLatekill() == player2.getLatekill() && player1.getKill() > player2.getKill()) ||
    (player1.getLatekill() == player2.getLatekill() && player1.getKill() == player2.getKill() && player1.getDeath() < player2.getDeath()) ||
    (player1.getLatekill() == player2.getLatekill() && player1.getKill() == player2.getKill() && player1.getDeath() == player2.getDeath() && player1.getName() < player2.getName());
}
