#ifndef superhero_h_
#define superhero_h_
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

class Superhero{

public:

	Superhero(string name_,string identity_,string power_){
		name=name_;
		identity=identity_;
		power=power_;
		good = true;
	}

	friend class Team;
	string getName() const{ return name; }
	string getPower() const { return power; }

	bool operator == (const Superhero& a){
		return (name==a.name && identity== a.identity && power==a.power); }

	bool operator == (const string a){
		return (identity== a); }

	bool operator != (const Superhero& a){
		return !(name==a.name && identity== a.identity && power==a.power); }

	bool operator != (const string a){
		return !(identity== a); }

	bool isGood() const { return good;}

	void operator -(){
		good = !good;}

	bool operator >(const Superhero& a){
		if (power== "Fire" && a.getPower() == "Wood" ){
			return true;}
		if (power== "Wood" && a.getPower() == "Water" ){
			return true;}
		if (power== "Water" && a.getPower() == "Fire" ){
			return true;}
		return false;
	}

private:
	string name;
	string identity;
	string power;
	bool good;

	string getTrueIdentity() const { return identity; }
};

ostream& operator<< (ostream& ostr, Superhero const& a); // non-member function
ostream& operator<< (ostream& ostr, Superhero& a); // non-member function
#endif
