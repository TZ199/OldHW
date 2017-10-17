#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>
#include <map>

using namespace std;
int main(int argc, char* argv[]) {
	ifstream in_str(argv[1]);
	map<int,int> count;

	int temp=0, max=0;
	while(in_str >>temp){
		++count[temp];
	}
	for( map<int,int>::iterator it=count.begin();
			it!=count.end();it++){
				if (it->second>=max){
					max=it->second;
				}
	}
	cout<<"The Mode is: ";
	for( map<int,int>::iterator it=count.begin();
			it!=count.end();it++){
				if (it->second==max){
					cout<<it->first<<"  ";
				}
	}
}
