HOMEWORK 7: MINIBLAST


NAME:  < Tianxin Zhou >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< 
Hashfunction from http://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map
And the lecture notes.
Daming Xu and I have read the homework description together.
>

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 20 >


HASH FUNCTION DESCRIPTION
My hash function comes from stack overflow,
The url:  http://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map

The code:
unsigned int hash= 0;
   for(int i = 0; i<key.size(); i++)
   {
       hash = hash * 101  +  key[i];
   }
   return hash;

This hash function is absolutely O(1), it multiply every char of the string by different number and add them up to ensure the difference.
I have tried the hash function from lecture notes, but that was too slow.

HASH TABLE IMPLEMENTATION

My hash table contains two vectors, one called hahsvector and one called sequences.
The hashvector is a vector of vector of int, which is used to storing the position of the kmer sequences.
And the sequences is a vector of string, which is used to storing the corresponding sequence for the hashvector.
When I insert a Kmer sequence, the program will check the sequences first to see if there is already a same sequence in or just put the sequence in. 
Then the program will store the position in hash vector.
If the collision happens, I use open addressing(linear probing) to solve the problem: simply check the index after the current index until find a proper index to insert.






ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

L = length of the genome sequence
q - query length
p - number of different locations where key is found
k - key size (k-mer size)

How much memory will the map data structure require (order notation for memory use)?
The hash table will inshore every 

The program basically creates three container a vector of char to store the genome sequences O(L), 
and a vector of vector of int to store the position O(L*k*p), a vector of string to store the kmer sequence: O(L*k)
So if we regard the int size and char size as same, the order notation of the memory will be O(L*k*p).


What is the order notation for performance (running time) of each of
the commands?
genome: O(L): my program will loop the entire file of genome and store it into a vector.

table_size: O(1): only store a int.

Occupancy: O(1): only store a int.

Kmer O(LK): We will insert every kmer sequences into the hashtable, so the order notation of combining chars to string will be O(k) and the Size of the genome is L.
The insert function is considered as O(1), so the order notation of kmer will be O(LK).

Query: O(QP): Since the find() can nearly to be considered as O(1), then we only need to count the order notation of checking the possible sequences.
Q if the length of query and p if the number of position need to check.






EXTRA CREDIT
Add a new command to implement the database using one of the other
 data structures that we have covered so far in the course: 
vectors, lists, arrays etc. 

I have completed the extra credits, the new command is:”Extra_mode”.
Enter the new command at the first then use the normal input command.
The new structure is a map<string, vector<int>> the string stores the kmer sequences, and the vector of int stores the positions.

Compare the performance your alternative method to the homework method
by making a table of run times for each of the genomes and query 
sets provided with the homework  and compare the times to build
the index and the times to process the queries. 
Document any new commands you have added in your README file.

                                     		  Speed test table
input\time		The hashtable version					The extra credit version(map)
Small 			real	0m0.027s					real	0m0.018s
			user	0m0.016s					user	0m0.014s
			sys	0m0.003s					sys	0m0.002s

Medium 			real	0m0.043s					real	0m0.082s
			user	0m0.032s					user	0m0.077s
			sys	0m0.002s					sys	0m0.002s

Large			real	0m0.050s					real	0m0.116s
			user	0m0.045s					user	0m0.110s
			sys	0m0.003s					sys	0m0.002s

Larger			real	0m0.497s					real	0m0.558s
			user	0m0.470s					user	0m0.540s
			sys	0m0.022s					sys	0m0.012s



From the data set we can find that the hashtable is generally faster than map except the small input.
And the bigger input is the smaller difference between two versions will be.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


