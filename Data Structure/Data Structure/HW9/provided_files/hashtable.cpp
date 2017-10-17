//Tianxin Zhou
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "hashtable.h"

//The insert function:
//Input the sequence and the position of the sequence.
//Return true if insert succeessfully, otherwise return false.
bool Hashtable::insert(std::string const& key, unsigned int position)
{
  //Get the hashvalue for the sequence.
  unsigned int hash_value = hashfunc(key);
  //Get the index of hashvalue.
  unsigned int index = hash_value % m_size;
  //If the index is empty, then insert the sequence to sequences,
  //and insert the position to hashvector.
  if(sequences[index] == "")
  {
    sequences[index] = key;
    hashvector[index].push_back(position);
    word_num++;
  }
  //If the sequence is already in
  //push_back new position.
  else if(sequences[index] == key)
  {
    hashvector[index].push_back(position);
    return false;
  }
  //If the index is occupied and not the same sequence.
  else
  {
    //loop until find another proper position after the current.
    while(1)
      {
      index++;
      //If there is not proper position to insert,
      //push the sequence in the end.
      if(index == sequences.size())
      {
        sequences.push_back(key);
        std::vector<int> temp;
        temp.push_back(position);
        hashvector.push_back(temp);
        word_num++;
        break;
      }
      //Find a position.
      else if(sequences[index] == "")
      {
        sequences[index] = key;
        hashvector[index].push_back(position);
        word_num++;
        break;

      }
      //Find the same sequence.
      else if(sequences[index] == key)
      {
        hashvector[index].push_back(position);
        return false;
      }

    }
  }
  //Resize the table.
  if(occupancy < float(word_num)/float(sequences.size()) )
  {
    this->resize_table(2*sequences.size()+1);
  }
  return true;
}

//Find the sequence and return the position.
//If find the sequence return pair of positions and true
//Otherwise return empty vector and false.
std::pair<std::vector<int>, bool> Hashtable::find(const std::string& key)
{
  unsigned int hash_value = hashfunc(key);
  unsigned int index = hash_value % m_size;
  std::vector<int> temp;
  //Found the sequence.
  if(sequences[index] == key)
  {
    for(int i = 0; i< hashvector[index].size();i++)
    {
      temp.push_back(hashvector[index][i]);
    }
    std::pair<std::vector<int>, bool> answer  = std::make_pair(temp, true);
    return answer;
  }
  //If we cannot find the sequence at the first,
  //Check the index after the current index.
  else{
    while(index<sequences.size())
    {
      if(sequences[index] == key)
      {
        for(int i = 0; i< hashvector[index].size();i++)
        {
          temp.push_back(hashvector[index][i]);
        }
        std::pair<std::vector<int>, bool> answer  = std::make_pair(temp, true);
        return answer;
      }
      else
        index++;
    }
    std::pair<std::vector<int>, bool> answer  = std::make_pair(temp, false);
    return answer;
}
}

//Reseize the table by new_size.
void Hashtable::resize_table(unsigned int new_size)
{
  //Create a new hashtable
  Hashtable temp = Hashtable(new_size,occupancy);
  if(sequences.size()!=hashvector.size())
  {
    std::cerr<<"Big problem" << std::endl;
  }
  //Copy the data.
  for(int i = 0; i< sequences.size(); i++)
  {
    for(int j = 0; j < hashvector[i].size(); j++)
    {
      temp.insert(sequences[i],hashvector[i][j]);
    }
  }
  //Copy the private variables.
  m_size = temp.m_size;
  hashvector = temp.hashvector;
  sequences = temp.sequences;
  word_num = temp.word_num;
}
