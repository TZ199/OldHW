// A simple "caller ID" program

#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

void add_map(map<int,string> &phonebook_map,int num, const string & name){
  phonebook_map[num] = name;
}

void identify_map(map<int,string> & phonebook, int number) {
  if (phonebook.find(number)==phonebook.end())
    cout << "unknown caller!" << endl;
  else
    cout << phonebook[number] << " is calling!" << endl;
}

int main() {
  // create the phonebook; initially all numbers are unassigned
  map<int,string> phonebook_map;

  add_map(phonebook_map, 1111111, "fred");
  add_map(phonebook_map, 2222222, "sally");
  add_map(phonebook_map, 3333333, "george");
  // test the phonebook
  identify_map(phonebook_map, 2222222);
  identify_map(phonebook_map, 4444444);

}
