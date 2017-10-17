/* ASSUMPTIONS:
   1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
   2. There will never be a duplicate key passed to insert.
 */


//Tianxin Zhou
//Last edited 04/07/2017 -5

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode {
public:
BPlusTreeNode() : parent(NULL) {
};
bool is_leaf();
bool contains(const T& key);

//For grading only, this is actually bad to have because
//it exposes a private member variable.
BPlusTreeNode* get_parent() {
								return parent;
}

//We need this to let BPlusTree access private members
friend class BPlusTree<T>;
private:
bool contains(const T& key,std::size_t low,std::size_t high);
std::vector<T> keys;
std::vector<BPlusTreeNode*> children;
BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
								for(unsigned int i=0; i<children.size(); i++) {
																if(children[i]) {
																								return false;
																}
								}
								return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
								return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
								if(low>high) {
																return false;
								}
								if(low==high) {
																return key == keys[low];
								}
								std::size_t mid = (low+high)/2;
								if(key<=keys[mid]) {
																return contains(key,0,mid);
								}
								else{
																return contains(key,mid+1,high);
								}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
class BPlusTree {
public:
//Constructor.
BPlusTree(int t)
{
								root = NULL; degree = t;
}
//Copy.
BPlusTree(const BPlusTree &other)
{
								root = this->copy_tree(other.root,NULL);
								degree = other.degree;
}
//Operator =.
BPlusTree& operator=(const BPlusTree& old) {
								root = this->copy_tree(old.root,NULL);
								degree = old.degree;
								return *this;
}
//Destructor.
~BPlusTree()
{
								this->destroy_tree(root);
}
//Insert the value into B+ Tree.
void insert(const T &input);
//Find the input value in the tree and output the pointer to the leaf.
BPlusTreeNode<T>* find(const T &input);
BPlusTreeNode<T>* find(const T &input,BPlusTreeNode<T>* node);
//Driver function for print_sideways.
void print_sideways(std::ofstream& outfile)
{
								if(!root)
								{
																outfile << "Tree is empty." << std::endl;
								}
								else
																print_sideways(outfile, root, 0);
}
//Driver function for print_BFS.
void print_BFS(std::ofstream& outfile)
{
								if(!root)
								{
																outfile << "Tree is empty." << std::endl;
								}
								else
																print_BFS(outfile, root, 0);
}

//Driver function for print_BFS_pretty
void print_BFS_pretty(std::ofstream& outfile)
{
	if(!root)
	{
									outfile << "Tree is empty." << std::endl;
	}
	else
									print_BFS_pretty(outfile, root, 0);
}

//Debug method.
T getroot()
{
								return root->keys[0];
}
private:
//The maximum degree.
int degree;
//The root node.
BPlusTreeNode<T>* root;
//Helper function to copy tree.
BPlusTreeNode<T>* copy_tree(BPlusTreeNode<T>* old_root, BPlusTreeNode<T>* the_parent);
//Helper function to destroy tree.
void destroy_tree(BPlusTreeNode<T>* temp)
{
								if(!temp) return;
								for(int i = 0; i< temp->children.size(); i++)
								{
																destroy_tree(temp->children[i]);
								}
								delete temp;
}
//Helper funtion to print_sideways.
void print_sideways(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth);
//Helper funtion to print_BFS.
void print_BFS(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth);
//Helper funtion to print_BFS_pretty.
void print_BFS_pretty(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth);
//Helper funtion to split.
void split(BPlusTreeNode<T>* current);

void collect(std::vector<std::vector<std::vector<T> > > &data);
};

//The function to insert value into tree.
//Will modify the tree structure.
template <class T>
void BPlusTree<T>::insert(const T &input)
{
								//If this is a empty b+ tree.
								if(!root)
								{
																//Set the value for root.
																root = new BPlusTreeNode<T>();
																root->keys.push_back(input);
																std::sort(root->keys.begin(), root->keys.end());
								}
								//If this is not empty.
								else{
																//Locate the location we will insert.
																BPlusTreeNode<T>* node = this->find(input);
																//If no need to split.
																if(node->keys.size() < degree -1)
																{
																								node->keys.push_back(input);
																								std::sort(node->keys.begin(), node->keys.end());
																}
																//If need to split.
																else{
																								//split here.
																								node->keys.push_back(input);
																								std::sort(node->keys.begin(), node->keys.end());
																								this->split(node);
																}
								}
}

//Driver function to find.
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T &input)
{
								return this->find(input, root);
}
//Content of find.
//Arguments: const T &input,BPlusTreeNode<T>* node.
//Output: the location of we find:BPlusTreeNode<T>*, if empty tree, return NULL.
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T &input,BPlusTreeNode<T>* node)
{
								//If this is an empty tree.
								if(!node)
								{
																return NULL;
								}
								//If the node is leaf, return this.
								if(node->is_leaf())
								{
																return node;
								}
								//If the node is children, use recursion.
								else
								{
																int i =0;
																while(i <node->keys.size() &&input>=node->keys[i])
																{
																				i++;
																}
																//If the value is in the most right child.
																return this->find(input, node->children[i]);
								}
}

//The content of copy tree.
//Argument: the tree we'll copy from :old_root, root :the_parent.
//Output: A pointer point to a BPlusTreeNode that is the root of new tree.
template <class T>
BPlusTreeNode<T>*  BPlusTree<T>::copy_tree(BPlusTreeNode<T>* old_root, BPlusTreeNode<T>* the_parent)
{
								//If empty tree.
								if (old_root == NULL) return NULL;
								//If not empty, copy every element.
								BPlusTreeNode<T> *answer = new BPlusTreeNode<T>();
								answer->keys = old_root->keys;
								for(int i = 0; i < old_root->children.size(); i++)
								{
																answer->children.push_back(copy_tree(old_root->children[i], answer));
								}
								answer->parent = the_parent;
								return answer;
}
//The content of print_sideways
//Arguments: outfile: the file we'll output, root: the root of the tree, depth the depth of the value.
//Resource: Lab10
template <class T>
void BPlusTree<T>::print_sideways(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth)
{
								if(root)
								{
																int middle = root->children.size()/2;
																//For the upper side.
																for(int i =0; i < middle; i++)
																{
																								print_sideways(outfile, root->children[i], depth+1);
																}
																//For the middle side.
																for (int i=0; i<depth; ++i) outfile << "\t";
																for(int i = 0; i<root->keys.size(); i++)
																{
																								if(i>=1)
																								{
																																outfile<< ",";
																								}
																								outfile << root->keys[i];
																}
																outfile << std::endl;
																//For the down side.
																for(int i =middle; i < root->children.size(); i++)
																{
																								print_sideways(outfile, root->children[i], depth+1);
																}
								}


}

//The content of print_BFS.
//Arguments: outfile: the file we'll output, root: the root of the tree, depth the depth of the value.
//Resource lecture notes.
template <class T>
void BPlusTree<T>::print_BFS(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth)
{
								//Create two lists to store the nodes we'll print.
								std::list<BPlusTreeNode<T>*> current_row;
								std::list<BPlusTreeNode<T>*> next_row;
								if(root) {
																current_row.push_back(root);
								}
								while(!current_row.empty()) {
																//Proccess the current row
																while(!current_row.empty()) {
																								BPlusTreeNode<T>*& tmp = current_row.front();
																								for(int i = 0; i<tmp->keys.size(); i++)
																								{
																																if(i>=1)
																																{
																																								outfile<<",";
																																}
																																outfile << tmp->keys[i];
																																if(i==tmp->keys.size()-1&&tmp!=current_row.back())
																																{
																																								outfile<<"\t";
																																}
																								}

																								for(int i = 0; i< tmp->children.size(); i++) {
																																next_row.push_back(tmp->children[i]);
																								}
																								current_row.pop_front(); //Advance the current "queue"
																}
																outfile << std::endl;
																//Make the next row the current row
																current_row = next_row;
																next_row.clear();
								}
}


//The content of print_BFS_pretty.
//Arguments: outfile: the file we'll output, root: the root of the tree, depth the depth of the value.
//Resource lecture notes.
template <class T>
void BPlusTree<T>::print_BFS_pretty(std::ofstream& outfile,BPlusTreeNode<T>* root, int depth)
{
								std::vector<std::vector<std::vector<T> > > data;
								this->collect(data);
								for(int i = 0 ; i < data.size(); i++)
								{
									for(int j = 0; j<data[i].size(); j++)
									{

										int format = ((data[data.size()-1].size()) /(i+2));
										std::string temp = std::string(format, '\t');

										if(i==data.size() -1 && j==0 )
										 temp = "";

										outfile << temp;
										for(int k = 0; k < data[i][j].size(); k++)
										{
											if(k>0)
												outfile<<",";
											outfile<<data[i][j][k];

										}
									}
									outfile <<std::endl;
								}

}

template <class T>
void BPlusTree<T>::collect(std::vector<std::vector<std::vector<T> > > &data)
{
	//Create two lists to store the nodes we'll print.
	std::list<BPlusTreeNode<T>*> current_row;
	std::list<BPlusTreeNode<T>*> next_row;
	if(root) {
									current_row.push_back(root);
	}
	while(!current_row.empty()) {
									std::vector<std::vector<T> > temp2;
									//Proccess the current row
									while(!current_row.empty()) {
																	BPlusTreeNode<T>*& tmp = current_row.front();
																	std::vector<T> temp;
																	for(int i = 0; i<tmp->keys.size(); i++)
																	{

																									temp.push_back(tmp->keys[i]);
																	}

																	for(int i = 0; i< tmp->children.size(); i++) {
																									next_row.push_back(tmp->children[i]);
																	}
																	current_row.pop_front(); //Advance the current "queue"
																	temp2.push_back(temp);
									}
									data.push_back(temp2);

									//Make the next row the current row
									current_row = next_row;
									next_row.clear();
	}





}


//The most important function split.
//Argument: The node we want to split.
//Modifies: The structure of the tree.
template <class T>
void BPlusTree<T>::split(BPlusTreeNode<T>* current)
{
								//Choose the value to pop up.
								int middle = current->keys.size()/2;
								T middle_value = current->keys[middle];
								//Make another node.
								BPlusTreeNode<T>* newnode = new BPlusTreeNode<T>;
								int keysize = current->keys.size();
								//Make the other vector and clean the current vector.
								for(int i = keysize - 1; i >= middle; i--)
								{
																newnode->keys.push_back(current->keys[i]);
																current->keys.pop_back();
								}
								//If the node is not leaf, we need to remove the middle_value.
								if(!current->is_leaf())
								{
																newnode->keys.pop_back();
								}
								//Change the order of the newnode.
								std::reverse( newnode->keys.begin(), newnode->keys.end());

								//Now solve the relationship to their children
								if(!current->is_leaf())
								{
																//Split the children to two nodes.
																int size = current->children.size();
																for(int i = size-1; i>=(size/2); i--)
																{
																								current->children[i]->parent = newnode;
																								newnode->children.push_back(current->children[i]);
																								current->children.pop_back();
																}
																//Then reverse can give the newnode correct order.
																std::reverse(newnode->children.begin(), newnode->children.end());
								}

								//Solve the relationship to their parent, if the cureent is not root.
								if(current->parent)
								{
																//Pop up the middle and assign the relationship.
																current->parent->keys.push_back(middle_value);
																//Sort the children again.
																std::sort(current->parent->keys.begin(), current->parent->keys.end());
																newnode->parent = current->parent;
																//Find the proper position to insert the child.
																for(typename std::vector<BPlusTreeNode<T>*>::iterator i = current->parent->children.begin(); i!=current->parent->children.end(); i++ )
																{
																								//Since the insert only can insert a value before the old value.
																								if(*i == current)
																								{
																																*i = newnode;
																																current->parent->children.insert(i,current);
																																break;
																								}
																}
																//If the parent node also full.
																if(current->parent->keys.size() >= degree)
																{
																								this->split(current->parent);
																}

								}

								//If it is the root, then we need to produce a new root.
								else if(current == root)
								{
																//Put the newnode right after the current nodes in parent's children.
																BPlusTreeNode<T>* newroot = new BPlusTreeNode<T>;
																//Children[0] < key, Children[1] >= key.
																newroot->children.push_back(current);
																newroot->children.push_back(newnode);
																newroot->keys.push_back(middle_value);
																current->parent = newroot;
																newnode->parent = newroot;
																root = newroot;
								}
}




#endif
