//DATA STRUCTURE HW2
//Tianxin Zhou     1/28/2017
//Edited for custom case 1/30/2017
//Added comments 1/30/2017
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
#include "Player.h"
#include "Champion.h"

//This function is going to deal with repeat players
//If we find the player in vector players return true, if not return false.
bool check_repeat(std::vector <Player>& players, std::string aname, std::string champion)
{
    for(int i = 0; i<players.size(); i++)
    {
        if(players[i].getName() == aname )
        {
            players[i].addChampion(champion);
            return true;
        }
    }
    return false;
}

//This function is going to deal with repeat champions
//If we find the champion in vector players return true, if not return false.
//Whats more, if we find the champion add lose or win to its vector.
bool c_check_repeat(std::vector <Champion>& champions, std::string aname, int check, std::string apeople)
{
    for(int i = 0; i<champions.size(); i++)
    {
        if(champions[i].getName() == aname )
        {
            if (check==1)
            {
                champions[i].addWin();
                champions[i].setPeople(apeople);
                return true;
            }
            else
                champions[i].addLose();
                champions[i].setPeople(apeople);
                return true;
        }
    }
    return false;
}

//Main function, process the input file
//Command example: ./a.out input_file.txt output_file.txt command(players, champions, custom)
int main(int argc, char* argv[])
{
    //Check the arguments number
    if(argc !=4)
    {
        std::cerr << "Error in arguments" << std::endl;
        return 1;
    }

    std::vector<Champion> champions;
    std::vector<Player> players;
    std::ifstream input_str(argv[1]);
    std::string option = argv[3];
    std::ofstream out_put(argv[2]);
    
    //Process the command here, now we are dealing with players.
    if(option == "players")
    {
        
        //Process the input file here
        std::string temp;
        while(input_str >> temp) 
        {
            //Start a new match
            if(temp == "MATCH")
            {
                std::string temp2;
                while(input_str >> temp2)
                {
                    //End the loop for match, wait for a new match begins.
                    if(temp2 == "END")
                    {
                        break;
                    }
                    
                    //Enter into the team section, collect the champion for players.
                    else if(temp2 == "TEAM")
                    {
                        std::string p1, n1,n2,c1;
                        
                        //Process the part of LOSING TEAM
                        for(int i = 0; i < 5; i++)
                        {    
                            input_str >> p1 >> n1 >> n2 >> c1;
                            
                            //Adding the player in to vector players.
                            bool check = check_repeat(players, p1, c1);
                            if (!check)
                            {
                                players.push_back(Player(p1,c1));
                            }
                            
                        }
                    }

                    //Process the events section.
                    else if(temp2 == "@")
                    {
                        std::string e1,e2,e3;
                        input_str >> e1 >> e2;
                        
                        //Ignore the minion case, this is not important for players.
                        if(e2 != "minion")
                        {
                            //Record the kill for players
                            for(int j = 0; j<players.size(); j++)
                            {
                                if(players[j].getName() == e2 )
                                {
                                	
                                    players[j].addKill();
                                    
                                }
                            }
                        }
                        
                        while(input_str >> e3)
                        {
                            //Record the death for players
                            if(e3 == "killed")
                            {
                                input_str >> e3;
                                for(int k = 0; k<players.size(); k++)
                                {
                                if(players[k].getName() == e3 )
                                {

                                    players[k].addDeath();
                                    
                                }
                            }
                                break;
                            }
                        }
                    }
                    
                }
            }
        }

        //Set the kdr for every players
        for(int l = 0; l < players.size();l++ )
        {
            players[l].setKdr();
        }
        //Sort here
        sort(players.begin(), players.end(), sortHelper);
    	//Cout the resluts to file.
        out_put << std::setw(23) << std::left << "PLAYER NAME" << std::setw(5) << std::right << "KILLS" <<
        std::setw(8) << std::right <<"DEATHS" << std::setw(8) << std::right <<"KDR" <<
        std::setw(26) << std::right <<"PLAYED WITH CHAMPION(S)" <<std::endl;
        for(int w = 0; w < players.size(); w++)
        {
            out_put << std::setw(23) << std::left << players[w].getName() << std::setw(5) << std::right << players[w].getKill() <<
            std::setw(8) << std::right <<players[w].getDeath() << std::setw(8) << std::right <<std::setprecision(2) << std::fixed <<
            players[w].getKdr() <<"   "<< players[w].getChampion() <<std::endl;
        }
            
    }
    
    //Process the command champions
    else if(option == "champions")
    {
        std::string temp;
        while(input_str >> temp) 
        {
            //Start the new match
            if(temp == "MATCH")
            {
                std::string temp2;
                while(input_str >> temp2)
                {
                    //Finish the old match
                    if(temp2 == "END")
                    {
                        break;
                    }
                    //Process the winning section
                    else if(temp2 == "WINNING")
                    {
                        input_str >> temp2;
                        std::string p1, n1,n2,c1;
                   		
                   		//Add the champion in to vector or just add a win
                        for(int i = 0; i < 5; i++)
                        {    
                            input_str >> p1 >> n1 >> n2 >> c1;
                            bool check = c_check_repeat(champions, c1,1,p1);
                            if (!check)
                            {
                                Champion champion1(c1,1);
                                champion1.setPeople(p1);
                                champions.push_back(champion1);
                                
                            }
                            
                            
                        }
                    }

                    //Process the losing section.
                    else if(temp2 == "LOSING")
                    {
                        input_str >> temp2;
                        std::string p1, n1,n2,c1;
                        //Add the champion in to vector or just add a win
                        for(int j = 0; j < 5; j++)
                        {    
                            input_str >> p1 >> n1 >> n2 >> c1;
                            bool check = c_check_repeat(champions, c1,0,p1);
                            if (!check)
                            {
                                Champion champion2(c1,0);
                                champion2.setPeople(p1);
                                champions.push_back(champion2);
                            }
                        }
                    }
                    //Finding the minion kill
                    else if(temp2 == "@")
                    {
                        std::string x1,x2,x3,x4;
                        input_str>>x1>>x2;
                        if(x2 == "minion")
                        {
                            input_str>>x3>>x4;
                            std::cout << "we got " << x4 << std::endl;
                            for(int k = 0; k < champions.size(); k++ )
                            {
                                if(champions[k].getPeople() == x4)
                                {
                                    champions[k].addMinion();
                                    break;
                                }
                            }
                        }
                    }
                }    
            }        
        }
        //Calculate the winrate for each player
        for(int l = 0; l < champions.size();l++ )
        {
            champions[l].setWinrate();
        }
        //Sort for champions
        sort(champions.begin(), champions.end(), c_sortHelper);
    	//Output the result in file.
        out_put << std::setw(24) << std::left << "CHAMPION NAME" << std::setw(4) << std::right << "WINS" <<
        std::setw(8) << std::right <<"LOSSES" << std::setw(8) << std::right <<"WIN%" <<
        std::setw(16) << std::right <<"MINION DEATHS" <<std::endl;
        for(int w = 0; w < champions.size(); w++)
        {
            out_put << std::setw(24) << std::left << champions[w].getName() << std::setw(4) << std::right << champions[w].getWin() <<
            std::setw(8) << std::right <<champions[w].getLose() << std::setw(8) << std::right <<std::setprecision(2) << std::fixed <<
            champions[w].getWinrate() <<std::setw(16) << std::right << champions[w].getMinion() <<std::endl;
        }
    }
    //If the command is not a valid one 
    else
    {
        std::cerr << "Error in command" << std::endl;
        return 1;
    }
    
    //For debuging
    #if(0)
    for(int l = 0; l < players.size();l++ )
    {
        std::cout << "name: " << players[l].getName() << " Kills " << players[l].getKill() << " Deaths: " << players[l].getDeath() << " Champion " << 
        players[l].getChampion() << " KDR " << players[l].getKdr() << std::endl;
    }
    #endif
    
    
    
        
    
    
    
    
    return 0;
}