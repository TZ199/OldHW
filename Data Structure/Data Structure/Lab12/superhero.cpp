#include <fstream>
#include <string>
#include <iostream>
#include "superhero.h"

using namespace std;

ostream& operator<<(ostream& out, const Superhero& a){
	if ( a.isGood()){ out<< " superhero ";}
	else{ out<< " supervillain ";}
	out << a.getName() << " has the power of " << a.getPower()<<endl;
	return out;
}

ostream& operator<<(ostream& out, Superhero& a){
	if (a.isGood()){ out<< " superhero ";}
	else{ out<< " supervillain ";}
	out << a.getName() << " has the power of " << a.getPower()<<endl;
	return out;
}
