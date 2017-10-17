#include <vector>
#include <list>
#include <iostream>

using namespace std;

void reverse_vector(vector<int> &v)
{
	int i=0;
	int temp;
	while (i<(v.size()/2))
	{
		temp = v[i];
		v[i]=v[v.size()-1-i];
		v[v.size()-1-i]=temp;
		i++;
	}
}

void reverse_list(list<int> &l)
{
	list<int>::iterator itr = l.begin();
	list<int>::reverse_iterator itr_r = l.rbegin();
	int temp;
	int count;
	while (count<(l.size()/2))
	{
		temp = (*itr);
		(*itr) = (*itr_r);
		(*itr_r) = temp;
		count++;
		itr++;
		itr_r++;
	}
}

void print_vector(const vector<int> &v)
{
	cout << "vector size: " << v.size() << endl;
	cout << "vector content: " << endl;
	for (unsigned int i=0; i<v.size(); ++i)
		cout << "  " << v[i];
	cout << endl;
}

void print_list(const list<int> &l)
{
	cout << "list size: " << l.size() << endl;
	cout << "list content: " << endl;
	for (list<int>::const_iterator i=l.begin(); i!=l.end(); ++i)
		cout << "  " << (*i);
	cout << endl;
}

int main()
{
	cout << "------- TEST VECTOR -------" << endl;
	vector<int> test_vector;
	test_vector.push_back(1);
	test_vector.push_back(3);
	test_vector.push_back(5);
	test_vector.push_back(7);
	test_vector.push_back(9);
	print_vector(test_vector);
	
	cout << "After reverse_vector function: " << endl;
	reverse_vector(test_vector);
	print_vector(test_vector);
	cout << endl;
	
	cout << "-------- TEST LIST --------" << endl;
	list<int> test_list;
	test_list.push_back(1);
	test_list.push_back(3);
	test_list.push_back(5);
	test_list.push_back(7);
	test_list.push_back(9);
	print_list(test_list);

	cout << "After reverse_list function: " << endl;
	reverse_list(test_list);
	print_list(test_list);
	cout << endl;
}