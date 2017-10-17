//Tianxin Zhou
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
//The class of self_made hashtable.
class Hashtable
{
private:
  //The vector of vector of int to store the location.
  std::vector<std::vector<int> > hashvector;
  //The vector of string to store the actual string.
  std::vector<std::string> sequences;
  //initial size.
  unsigned int m_size;
  //number of unique words.
  unsigned int word_num;
  //The occupancy.
  float occupancy;
  //when word_num/m_size > occupancy then resize the hashtable.
  void resize_table(unsigned int new_size);

  //Hashfunction from stackoverflow
  //http://stackoverflow.com/questions/98153/whats-the-best-hashing-
  //algorithm-to-use-on-a-stl-string-when-using-hash-map
  unsigned int hashfunc(const std::string &key)
  {
    unsigned int hash= 0;
   for(int i = 0; i<key.size(); i++)
   {
       hash = hash * 101  +  key[i];
   }
   return hash;
  }

public:
  //Constructor.
  Hashtable(unsigned int table_size,float aoccupancy)
  {
    m_size = table_size;
    occupancy = aoccupancy;
    hashvector.resize(m_size);
    sequences.resize(m_size);
    word_num = 0;
    //Debug
    //std::cout <<"M_size: " << m_size << " hashvector.size: " << hashvector.size() <<std::endl;
  }
  //The default constructor.
  Hashtable()
  {
    m_size = 0;
    word_num = 0;
  }
  //Destructor.
  ~Hashtable(){}
  //Get the m_size.
  unsigned int size() const { return m_size; }
  //Insert the sequence.
  bool insert(std::string const& key, unsigned int position);
  //find the sequence
  std::pair<std::vector<int>, bool> find(const std::string& key);
  //Overload the operator=.
  Hashtable& operator=(const Hashtable& old) {
      this->sequences = old.sequences;
      this->hashvector = old.hashvector;
      this->m_size = old.m_size;
      this->word_num = old.word_num;
      this->occupancy = old.occupancy;
      return *this;
  }
  //Sef_debug.
  void hashprint()
  {
    std::cout<<"The table size = " << m_size << " The occupancy = " << occupancy <<
    " The size of hashvector "<< hashvector.size() << "the size of sequences" <<
    sequences.size() <<"the size of words: "<<word_num<< std::endl;
  }
};
