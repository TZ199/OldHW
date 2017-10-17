#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>

// -------------------------------------------------------------------
// TREE NODE CLASS
template <class T>
class TreeNode {
public:
  TreeNode() : left(NULL), right(NULL), parent(NULL) {}
  TreeNode(const T& init) : value(init), left(NULL), right(NULL), parent(NULL) {}
  T value;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent; // to allow implementation of iterator increment & decrement
};

template <class T>
void destroy_tree(TreeNode<T>* p) {
  if (!p) return;
  destroy_tree(p->right);
  destroy_tree(p->left);
  delete p;
}

template <class T>
const T& FindSmallestInTree(TreeNode<T>* root){
  while(root->left){
    root = root->left;
  }
  return root->value;
}

void IntTreeTest();

void StringTreeTest();

////////////////////////ABOVE IS DEBUGGING ONLY//////////////

template <class T>
const T& FindLargestInTree(TreeNode<T>* root){
  while(root->right){
    root = root->right;
  }
  return root->value;
}

//Provided 3.2 code
template <class T>
TreeNode<T>* FindSmallestInRange(const T& a, const T& b, TreeNode<T>* root){
  if(!root){
    return NULL;
  }

  T best_value = FindLargestInTree(root);
  TreeNode<T>* ret = FindSmallestInRange(a, b, root, best_value);
  if(best_value >= b){
    return NULL;
  }
  return ret;
}

template <class T>
TreeNode<T>* FindSmallestInRange(const T& a, const T& b, TreeNode<T>* root, T& best_value){
  if(!root){
  return NULL;
  }
  TreeNode<T>* left_subtree = FindSmallestInRange(a,b,root->left,best_value);
  TreeNode<T>* right_subtree = FindSmallestInRange(a,b,root->right,best_value);
  if(root->value > a && root->value < best_value){
  best_value = root->value;
  return root;
  }
  else if(left_subtree && left_subtree->value == best_value){
  return left_subtree;
  }
  else if (right_subtree){
  return right_subtree;
  }
  return NULL;
}

//DON'T ALLOW CALLING STD::SORT, NO MAPS, SETS, OR LISTS.
//IN THIS FUNCTION, NO USE OF ->left,->right,or ->parent. MAY CALL FUNCTIONS THAT USE THESE MEMBERS.
template <class T>
std::vector<T> TreeSort(TreeNode<T>* root){
  std::vector<T> ret;
  const T& smallest = FindSmallestInTree(root);
  const T& largest = FindLargestInTree(root);
  ret.push_back(smallest);
  TreeNode<T>* find = FindSmallestInRange(ret[ret.size()-1],largest,root);
  while(find){
  ret.push_back(find->value);
  find = FindSmallestInRange(ret[ret.size()-1],largest,root);
  }
  ret.push_back(largest);
  return ret;
  }

int main(){

  //Feel free to add your own tests, or add tests to either of the functions called below.

  StringTreeTest();

  IntTreeTest();

  return 0;
}


void IntTreeTest(){
  TreeNode<int>* root = new TreeNode<int>(12);
  root->left = new TreeNode<int>(7);
  root->left->left = new TreeNode<int>(2);
  root->left->left->right = new TreeNode<int>(5);
  root->left->right = new TreeNode<int>(10);
  root->left->right->left = new TreeNode<int>(8);
  root->left->right->right = new TreeNode<int>(11);
  root->right = new TreeNode<int>(37);
  root->right->left = new TreeNode<int>(14);
  root->right->right = new TreeNode<int>(91);
  root->right->right->left = new TreeNode<int>(44);

  int largest_in_tree = FindLargestInTree(root);
  assert(largest_in_tree == 91);

  assert(FindSmallestInRange(7,9,root));
  assert(FindSmallestInRange(7,9,root)->value==8);
  assert(!FindSmallestInRange(7,9,root->right));
  assert(FindSmallestInRange(6,8,root->left));
  assert(FindSmallestInRange(6,8,root->left)->value==7);
  assert(FindSmallestInRange(4,6,root));
  assert(FindSmallestInRange(4,6,root)->value==5);
  assert(FindSmallestInRange(11,14,root));
  assert(FindSmallestInRange(11,14,root)->value==12);
  assert(FindSmallestInRange(7,1000,root)->value==8);

  std::vector<int> sorted = TreeSort(root);
  std::vector<int> actually_sorted;
  actually_sorted.push_back(2);
  actually_sorted.push_back(5);
  actually_sorted.push_back(7);
  actually_sorted.push_back(8);
  actually_sorted.push_back(10);
  actually_sorted.push_back(11);
  actually_sorted.push_back(12);
  actually_sorted.push_back(14);
  actually_sorted.push_back(37);
  actually_sorted.push_back(44);
  actually_sorted.push_back(91);
  for(unsigned int i=0; i<sorted.size(); i++){
    std::cout << "Sorted " << i << "th: " << sorted[i] << std::endl;
  }
  assert(actually_sorted == sorted);

  destroy_tree(root);
}

void StringTreeTest(){
  TreeNode<std::string>* root = new TreeNode<std::string>("fox");
  root->left = new TreeNode<std::string>("cat");
  root->left->left = new TreeNode<std::string>("ape");
  root->left->right = new TreeNode<std::string>("dog");
  root->right = new TreeNode<std::string>("rat");
  root->right->left = new TreeNode<std::string>("gnu");
  root->right->right = new TreeNode<std::string>("yak");

  assert(FindSmallestInRange(std::string("fox"),std::string("zebra"),root)->value == "gnu");
  assert(FindSmallestInRange(std::string("ape"),std::string("dog"),root)->value == "cat");
  assert(FindSmallestInRange(std::string("ape"),std::string("dog"),root->left)->value == "cat");
  assert(FindSmallestInRange(std::string("ape"),std::string("cat"),root) == NULL);
  assert(FindSmallestInRange(std::string("ape"),std::string("yak"),root->right)->value == "gnu");

  std::string largest_in_tree = FindLargestInTree(root);
  assert(largest_in_tree == "yak");
  largest_in_tree = FindLargestInTree(root->left);
  assert(largest_in_tree == "dog");

  std::vector<std::string> sorted = TreeSort(root);
  std::vector<std::string> actually_sorted;
  actually_sorted.push_back("ape");
  actually_sorted.push_back("cat");
  actually_sorted.push_back("dog");
  actually_sorted.push_back("fox");
  actually_sorted.push_back("gnu");
  actually_sorted.push_back("rat");
  actually_sorted.push_back("yak");
  for(unsigned int i=0; i<sorted.size(); i++){
    std::cout << "Sorted " << i << "th: " << sorted[i] << std::endl;
  }
  assert(actually_sorted == sorted);

  destroy_tree(root);
}
