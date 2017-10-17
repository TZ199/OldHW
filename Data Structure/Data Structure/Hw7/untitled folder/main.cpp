// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
//Tianxin Zhou,  zhout3@rpi.edu
//03/30/2017

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <assert.h>
#include <algorithm>
#include "mtrand.h"




// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  std::map<std::string,std::map<std::string,int> > MY_MAP;
typedef  std::map<std::string,std::map<std::string,std::map<std::string, int> > > MY_MAP3;
typedef MY_MAP::iterator it_type;
typedef std::map<std::string, int>::iterator small_it;
typedef MY_MAP3::iterator big_it;
// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
        char c;
        word.clear();
        while (istr) {
                // just "peek" at the next character in the stream
                c = istr.peek();
                if (isspace(c)) {
                        // skip whitespace before a word starts
                        istr.get(c);
                        if (word != "") {
                                // break words at whitespace
                                return true;
                        }
                } else if (c == '"') {
                        // double quotes are a delimiter and a special "word"
                        if (word == "") {
                                istr.get(c);
                                word.push_back(c);
                        }
                        return true;
                } else if (isalpha(c)) {
                        // this a an alphabetic word character
                        istr.get(c);
                        word.push_back(tolower(c));
                } else {
                        // ignore this character (probably punctuation)
                        istr.get(c);
                }
        }
        return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
        // returns a vector of strings of the different words
        std::vector<std::string> answer;
        std::string word;
        bool open_quote = false;
        while (ReadNextWord(istr,word)) {
                if (word == "\"") {
                        if (open_quote == false) { open_quote=true; }
                        else { break; }
                } else {
                        // add each word to the vector
                        answer.push_back(word);
                }
        }
        return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(MY_MAP &data, MY_MAP3 &data3,const std::string &filename, int window, const std::string &parse_method) {
        // open the file stream
        std::ifstream istr(filename.c_str());
        if (!istr) {
                std::cerr << "ERROR cannot open file: " << filename << std::endl;
                exit(1);
        }
        // verify the window parameter is appropriate
        if (window < 2) {
                std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
        }
        // verify that the parse method is appropriate
        bool ignore_punctuation = false;
        if (parse_method == "ignore_punctuation") {
                ignore_punctuation = true;
        } else {
                std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
                exit(1);
        }

        //Start my work
        std::vector<std::string> Words;
        std::string word;
        while (ReadNextWord(istr,word)) {
                // skip the quotation marks (not used for this part)
                if (word == "\"") continue;
                else
                {
                  //Store all word in a vector
                  Words.push_back(word);

                }
        }


        //Fill the map with window = 2
        for(int i = 0; i<Words.size(); i++)
        {
                std::map<std::string, int> temp;
                //Process the last word
                if(i<Words.size()-1)
                {
                        data[Words[i]][Words[i+1]]++;
                }
                else if(i == Words.size() - 1)
                {data[Words[i]][""]++;}
        }
        //Fill the map with window = 3
        if(window == 3)
        {
                for(int i = 0; i<Words.size(); i++)
                {
                        //Process the last words
                        if(i<Words.size()-2)
                        {
                                data3[Words[i]][Words[i+1]][Words[i+2]]++;
                        }
                        else if(i == Words.size() - 2)
                        {data3[Words[i]][Words[i+1]][""]++;}
                }
        }

}


//Printer for window =2
//Input: data for window = 2, and the target string
void printer(MY_MAP &data, std::string target)
{
        //Find the string.
        it_type iterator = data.find(target);
        //If we find the string
        if(iterator!=data.end())
        {
                int size = 0;
                //Go through second map and add the value to size.
                for(small_it iterator2 = iterator->second.begin(); iterator2 !=iterator->second.end(); iterator2++)
                {
                        size+=iterator2->second;
                }
                std::cout << iterator->first << " (" << size <<")" << std::endl;
                //Go through second map and print out the key in second map
                for(small_it iterator2 = iterator->second.begin(); iterator2 !=iterator->second.end(); iterator2++)
                {
                        if(iterator2->first == "")
                                continue;
                        std::cout << iterator->first << " " <<iterator2->first << " " <<"(" <<  iterator2->second << ")"<< std::endl;
                }
                std::cout << std::endl;
        }
        //If the string is not in the map
        else
        {
                std::cerr <<"The word u looking for is not in the map" << std::endl;
                return;
        }
}

//Printer for window =3.
//Input: data for window = 3, and the target string.
void printer(MY_MAP3 &data3, std::vector<std::string> target)
{
        //Find the string.
        big_it iterator = data3.find(target[0]);
        //If we find the string
        if(iterator!=data3.end())
        {
                //Find the second string
                it_type iterator2 = iterator->second.find(target[1]);
                //If we can't get the second string
                if(iterator2 == iterator->second.end())
                {
                        std::cerr << "Cant find the " << target[1] << std::endl;
                        return;
                }

                int size = 0;
                //Go through second map and add the value to size.
                for(small_it iterator3 = iterator2->second.begin(); iterator3 !=iterator2->second.end(); iterator3++)
                {
                        size+=iterator3->second;
                }
                std::cout << iterator->first <<" " << iterator2->first << " (" << size <<")" << std::endl;
                //Go through second map and print out the key in second map
                for(small_it iterator3 = iterator2->second.begin(); iterator3 !=iterator2->second.end(); iterator3++)
                {
                        if(iterator3->first == "")
                                continue;
                        std::cout << iterator->first << " " <<iterator2->first << " " << iterator3->first << " (" <<  iterator3->second << ")"<< std::endl;
                }
        }
        std::cout << std::endl;
}

//Helper Function to print out the random string
//Input: vector of string: the target strings, Map with window = 2, length of the required sentence,
//mode: true = print for window = 2, false = for produce a random word after the first word.
//Outpiut: Random string in the end of the sentence
std::string generate_random(std::vector<std::string> target, MY_MAP &data,int length,bool mode )
{
        //Set up the rand seed to stardand one.
        static MTRand_int32 mtrand;
        //Lock the length of sentence to 1.
        if(target.size() == 1)
        {
                if(mode)
                        std::cout << target[0];
                std::string current_target = target[0];
                //For loop to generate required lenghth sentence.
                for(int i = 0; i< length; i++)
                {
                        it_type it = data.find(current_target);
                        std::vector<std::string> temp;
                        //If the word is not in the sentence
                        if(it == data.end())
                        {
                                break;
                        }
                        else
                        {
                                //If there exists words after the current target string
                                if(it->second.size() > 0)
                                {
                                        //Go through all string after the target string
                                        for(small_it it2 = it->second.begin(); it2 != it->second.end(); it2++)
                                        {
                                                //Fill a vector of string with value*key
                                                for(int i = 0; i < it2->second; i++)
                                                {
                                                        temp.push_back(it2->first);
                                                }
                                        }
                                        //Produce a random number
                                        int max = temp.size();
                                        int random = mtrand() % max;
                                        if(mode)
                                                std::cout << " " <<temp[random];
                                        current_target = temp[random];
                                }
                                else
                                        break;
                        }
                }
                if(mode)
                        std::cout << std::endl;
                return current_target;
        }
        else
                return NULL;

}

//Helper Function to print out the random string
//Input: the first target string, the second target string, Map with window = 3, length of the required sentence,
void generate_random(std::string target1, std::string target2, MY_MAP3 &data3,int length )
{       //Set up the rand seed to stardand one.
        static MTRand_int32 mtrand;
        std::cout << target1 << " " << target2;
        //For loop to generate required lenghth sentence.
        for(int i = 0; i< length; i++)
        {
                big_it it = data3.find(target1);
                std::vector<std::string> temp;
                //If the word is not in the sentence
                if(it == data3.end())
                {
                        std::cerr <<"cant find the " << target1 << std::endl;
                        break;
                }
                else
                {
                        it_type it2 = it->second.find(target2);
                        //If the word is not in the sentence
                        if(it2 == it->second.end())
                        {
                                std::cerr <<"cant find the " << target2 << std::endl;
                                break;
                        }
                        std::string max_string;
                        //If there exists words after the current target string
                        if(it2->second.size() > 0)
                        {
                              //Go through all string after the target string
                                for(small_it it3 = it2->second.begin(); it3 != it2->second.end(); it3++)
                                {
                                        for(int i = 0; i < it3->second; i++)
                                        {
                                                //Fill a vector of string with value*key
                                                temp.push_back(it3->first);
                                        }
                                }
                                //Produce a random number
                                int max = temp.size();
                                int random = mtrand() % max;
                                std::cout << " " <<temp[random];
                                target1 = target2;
                                target2 = temp[random];

                        }
                        else
                                break;
                }

        }

        std::cout << std::endl;

}

//Helper Function to print out the max counted string
//Input: vector of string: the target strings, Map with window = 2, length of the required sentence,
//mode: true = print for window = 2, false = for produce a max counted word after the first word.
//Outpiut: max counted string in the end of the sentence
std::string generate_max(std::vector<std::string> target, MY_MAP &data,int length, bool mode)
{
        //Lock the length of sentence to 1.
        if(target.size() == 1)
        {
                if(mode)
                        std::cout << target[0];
                std::string current_target = target[0];
                //For loop to generate required lenghth sentence.
                for(int i = 0; i< length; i++)
                {
                        it_type it = data.find(current_target);
                        std::vector<std::string> temp;
                        //If the word is not in the sentence
                        if(it == data.end())
                        {
                                break;
                        }
                        else
                        {
                                int max = 0;
                                std::string max_string;
                                //If there exists words after the current target string
                                if(it->second.size() > 0)
                                {       //Go through all string after the target string
                                        for(small_it it2 = it->second.begin(); it2 != it->second.end(); it2++)
                                        {
                                                //If a word's count is bigger than max, then replace it.
                                                if(it2->second > max)
                                                {
                                                        max = it2->second;
                                                        max_string = it2->first;
                                                }
                                        }
                                        if(mode)
                                                std::cout << " " <<max_string;
                                        current_target = max_string;
                                }
                                else
                                        break;
                        }

                }
                if(mode)
                        std::cout << std::endl;
                return current_target;
        }
        else
                return NULL;
}

//Helper Function to print out the max counted string
//Input: the first target string, the second target string, Map with window = 3, length of the required sentence,

void generate_max(std::string target1, std::string target2, MY_MAP3 &data3,int length)
{
        std::cout << target1 << " " << target2;
        //For loop to generate required lenghth sentence.
        for(int i = 0; i< length; i++)
        {
                big_it it = data3.find(target1);
                //If the word is not in the sentence
                if(it == data3.end())
                {
                        std::cerr <<"cant find the " << target1 << std::endl;
                        break;
                }
                else
                {
                        it_type it2 = it->second.find(target2);
                        //If the word is not in the sentence
                        if(it2 == it->second.end())
                        {
                                std::cerr <<"cant find the " << target2 << std::endl;
                                break;
                        }
                        int max = 0;
                        std::string max_string;
                        //If there exists words after the current target string
                        if(it2->second.size() > 0)
                        {       //Go through all string after the target string
                                for(small_it it3 = it2->second.begin(); it3 != it2->second.end(); it3++)
                                {
                                        //If a word's count is bigger than max, then replace it.
                                        if(it3->second > max)
                                        {
                                                max = it3->second;
                                                max_string = it3->first;
                                        }
                                }

                                std::cout << " " <<max_string;
                                target1 = target2;
                                target2 = max_string;

                        }
                        else
                                break;
                }

        }

        std::cout << std::endl;
}



int main () {

        // ASSIGNMENT: THE MAIN DATA STRUCTURE
        MY_MAP data;
        MY_MAP3 data3;
        int window;
        // Parse each command
        std::string command;
        while (std::cin >> command) {

                // load the sample text file
                if (command == "load") {
                        std::string filename;
                        std::string parse_method;
                        std::cin >> filename >> window >> parse_method;
                        LoadSampleText(data, data3,filename,  window,parse_method);
                        std::cout <<"Loaded "<<filename<<" with window = "<<window<<" and parse method = "<<parse_method << std::endl <<std::endl;

                }

                // print the portion of the map structure with the choices for the
                // next word given a particular sequence.
                else if (command == "print") {
                        std::vector<std::string> sentence = ReadQuotedWords(std::cin);

                        if(window == 2)
                        {
                                if(sentence.size() != 1)
                                {
                                        std::cerr<<"Wrong with command" << std::endl;
                                }
                                printer(data, sentence[0]);
                        }
                        else if(window == 3)
                        {       //Check the sentence length, if only one
                                if(sentence.size() == 1)
                                {
                                        printer(data,sentence[0]);
                                }
                                else if(sentence.size() == 2)
                                {
                                        printer(data3,sentence);
                                }
                        }


                }

                // generate the specified number of words
                else if (command == "generate") {
                        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
                        // how many additional words to generate
                        int length;
                        std::cin >> length;
                        std::string selection_method;
                        std::cin >> selection_method;
                        bool random_flag;
                        if (selection_method == "random") {
                                random_flag = true;
                        } else {
                                assert (selection_method == "most_common");
                                random_flag = false;
                        }
                        if(random_flag == true)
                        {
                                if(window == 2)
                                {
                                        generate_random(sentence, data, length,true);
                                        std::cout << std::endl;
                                }

                                else if(window == 3)
                                {
                                        //Check the sentence length, if only one, then generate another word.
                                        if(sentence.size() == 1)
                                        {
                                                std::string target2 = generate_random(sentence, data, 1,false);
                                                generate_random(sentence[0], target2, data3, length-1);
                                                std::cout << std::endl;
                                        }
                                        else if(sentence.size() == 2)
                                        {
                                                generate_random(sentence[0], sentence[1], data3, length);
                                                std::cout << std::endl;
                                        }
                                }
                        }

                        else{
                                if(window == 2)
                                {
                                        generate_max(sentence, data, length,true);
                                        std::cout << std::endl;
                                }
                                else if(window == 3)
                                {
                                        //Check the sentence length, if only one, then generate another word.
                                        if(sentence.size() == 1)
                                        {
                                                std::string target2 = generate_max(sentence, data, 1,false);
                                                generate_max(sentence[0], target2, data3, length-1);
                                                std::cout << std::endl;

                                        }
                                        else if(sentence.size() == 2)
                                        {
                                                generate_max(sentence[0], sentence[1], data3, length);
                                                std::cout << std::endl;
                                        }
                                }

                        }

                } else if (command == "quit") {
                        break;
                } else {
                        std::cout << "WARNING: Unknown command: " << command << std::endl;
                }
        }
}
