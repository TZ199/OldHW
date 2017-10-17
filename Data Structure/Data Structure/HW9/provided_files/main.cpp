//DATA STRUCTURE HW9
//Tianxin Zhou
//Start date:04/16/2017
//Last edited 04/21/2017
//Added the extra_mode 04/21
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
#include <map>
#include "hashtable.h"

//Helper function to read the file in to a vector:library.
void readfile(std::string &filename,std::vector<char>& library)
{
  std::ifstream fin;
  fin.open(filename, std::ios::in);
  char my_character ;
  //Loop until the end.
  while (!fin.eof() )
  {
	fin.get(my_character);
  if (my_character == '\n'){}
  else
    library.push_back(my_character);
  }
}

//The helper function to find the proper sequence and print out the difference.
//If find the proper sequence return true, otherwise return false.
bool deal_sequence(const std::vector<char> &library,int location,
  const std::string & key, int diff)
{
  int check = 0;
  std::string temp = "";
  bool print = true;
  //Use a loop to check every char in the sequence.
  for(int i = 0; i<key.size(); i++)
  {
    if(i+location >= library.size())
    {
      print = false;
      return false;
    }
    else
    {
      if(library[location+i] != key[i])
      {
        check++;
        temp+=library[location+i];
      }
      else
        temp+=library[location+i];

      if(check > diff)
      {
        return false;
      }
    }
  }
  //If we found the sequence.
  if(print)
  {
    std::cout<<location << " " << check << " " << temp<<std::endl;
  }
  return true;

}


//Main function
int main () {
  //library for the input file; table for the initial table size;
  //occupancy for the initial occupancy; kmer for the initial kmer;
  std::vector<char> library;
  int table = 0;
  float occupancy = 0;
  int kmer = 0;
  Hashtable data;
  bool extra = false;
  //For the extra credit.
  std::map<std::string, std::vector<int > > extra_data;
  // Parse each command
  std::string command;
  while (std::cin >> command)
  {
    // load the sample text file
    if (command == "genome")
    {
      std::string filename;
      std::cin >> filename;
      readfile(filename,library);
    }
    //load the table_size.
    else if(command == "table_size")
    {
      std::cin >> table;
    }
    //load the occupancy
    else if(command == "occupancy")
    {
      std::cin >> occupancy;

    }
    //load the kmer and begin to construct hashtable.
    else if(command =="kmer")
    {
      std::cin >> kmer;
      //load the default occupancy.
      if(occupancy == 0)
      {
        occupancy = 0.5;
      }
      //load the default table_size.
      if(table ==0)
      {
        table = 100;
      }
      if(!extra){
        data = Hashtable(table,occupancy);
        //Use a loop to insert every possible sequence.
        for(int i = 0; i < (library.size()-kmer); i++)
        {
          std::string temp ="";
          for(int j = i; j<i+kmer; j++)
          {
            temp+=library[j];
          }
          data.insert(temp,i);
        }
      }
      //The map version.
      else
      {
        for(int i = 0; i < (library.size()-kmer); i++)
        {
          std::string temp ="";
          for(int j = i; j<i+kmer; j++)
          {
            temp+=library[j];
          }
          extra_data[temp].push_back(i);
        }
      }
    }
    //For the command query.
    else if(command == "query")
    {
      if(!extra){
        int different;
        bool print = false;
        std::string query;
        std::cin >> different >> query;
        std::cout <<"Query: " << query << std::endl;
        std::pair<std::vector<int>, bool> location;
        std::string key = query.substr(0,kmer);
        location = data.find(key);
        //If we found the kmer sequence in hashtable.
        if (location.second)
        {
          for(int i = 0; i< location.first.size(); i++)
          {
            if(deal_sequence(library,location.first[i], query, different))
              print = true;
          }
        }
        //If we didnt find the sequence, print no match.
        if(!print)
        {
          std::cout<<"No Match"<<std::endl;
        }
      }
    //The extra credits.
    else{
      int different;
      bool print = false;
      std::string query;
      std::cin >> different >> query;
      std::cout <<"Query: " << query << std::endl;
      std::string key = query.substr(0,kmer);
      std::map<std::string, std::vector<int > >::iterator itr = extra_data.find(key);
      if (itr!=extra_data.end())
      {
       for(int i = 0; i< extra_data[key].size(); i++)
       {
         if(deal_sequence(library,extra_data[key][i], query, different))
           print = true;
       }
      }
      if(itr == extra_data.end() || !print)
      {
        std::cout<<"No Match"<<std::endl;
      }
    }
  }

    //Quit the program.
    else if(command == "quit")
    {
      break;
    }
    //Self debug
    else if(command == "Self_debug")
    {
      std::cout << "The library: " << std::endl;
      for(int i = 0; i< library.size(); i++)
      {
        std::cout<< library[i];
      }
      std::cout << "table_size: " << table<< std::endl;
      std::cout<<"Occupancy: " << occupancy << std::endl;
    }
    else if(command == "Extra_mode")
    {
      extra = true;
      std::cout << "Now you are entering the extra mode" <<std::endl;
    }

    else{
      std::cerr <<"Unknown command" <<std::endl;
    }

  }

return 0;
}
