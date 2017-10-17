#include <iostream>
#include <vector>
#include <list>
using namespace std;

void reverse_vector(vector<int> &a,int size){
	for (int i=0;i<size/2;i++){
		int temp=0;
		temp=a[i];
		a[i]=a[size-1-i];
		a[size-1-i]=temp;
	}
}

void print(vector<int> &a,int size){
	cout<<"size: "<<size<<endl;
	for (int i=0;i<size;i++){
		cout<<a[i]<<" \n";
	}
}

void reverse_list(list<int> &a){
	list<int>::iterator itr = a.begin();  
	list<int>::reverse_iterator ritr = a.rbegin();
	for (int i=0;i<a.size()/2;i++){
		int temp=*itr;
		*itr=*ritr;
		*ritr=temp;
		itr++;
		ritr++;
	}
}


int main(){
	vector<int> v1;
	//vector<int> v2;
	for (int i = 0; i < 15; i+=2) {
		v1.push_back(i);
	}
	
	cout << "test vector\n"
		 << "previous vector: ";
	for (vector<int>::iterator itr=v1.begin(); itr!=v1.end();itr++){
		cout << *itr << " ";
	}
	cout << "\n";

	reverse_vector(v1,v1.size());
	cout << "vector reversed: ";
	for (vector<int>::iterator itr=v1.begin(); itr!=v1.end();itr++){
		cout << *itr << " ";
	}
	cout << "\n";

	list <int> a;
	list <int> b;
	a.push_back(10);
	a.push_back(2);
	a.push_back(11);
	a.push_back(4);
	/*
	list<int>c;
	reverse_list(c);

	for (list<int>::iterator itr=c.begin();itr!=c.end();itr++){
		cout<<*itr;
	}
	*/
	for (int j=1;j < 21;j+=2){
		b.push_back(j);
	}
	cout <<"test list\n"
		 <<"previous list: ";
	for (list<int>::iterator itr=b.begin();itr!=b.end();itr++){
		cout<<*itr<<" ";
	}
	cout<<endl;
	reverse_list(b);
	cout<<"after reverse: ";
	for (list<int>::iterator itr=b.begin();itr!=b.end();itr++){
		cout<<*itr<<" ";
	}
	cout<<endl;
	cout<<"previous list: ";
	for (list<int>::iterator itr=a.begin();itr!=a.end();itr++){
		cout<<*itr<<" ";
	}
	cout<<endl;
	reverse_list(a);
	cout<<"after reverse: ";
	for (list<int>::iterator itr=a.begin();itr!=a.end();itr++){
		cout<<*itr<<" ";
	}
	cout<<endl;
	
	
	return 0;
}