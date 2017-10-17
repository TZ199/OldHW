HOMEWORK 5: DSRADIO SONG GROUPS


NAME:  < Tianxin Zhou >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< insert collaborators / resources >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 15 >



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of s = the number of songs in the library,
g = the number of song groups, a = the average number of songs by the same 
artist, and l = the average length of song groups.

AddSongToLibrary
O(s)
GroupExists
O(g)
AddToGroup
O(l^2) due to RecomputePrevByYear function 
MakeGroup
O(g)
PrintGroupRewind
O(l)
PrintLongestGroupRewind
O(l^2)
RemoveGroup
O(l+g)
DeleteAllSongGroups
O(lg+g^2)
RemoveFromGroup
O(l^2) due to RecomputePrevByYear function 
PrintGroupMarathon
O(a)


TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your implementation?

For this program, I used Dr.memory most often. Since for coding a double linked list program, we can easily make bugs in memory, like unaddressed memory read, or memory leak.
And I met many segmentation faults in this hw due to the memory bugs. Thus I think dr.memory helped a lot for hw5. I also have tried printf, but only for check the print template. 

For the corner case in this hw5, I have wrote my own test case input file, and tried some cases like Addtogroup for an empty group, combine two empty lists, test remove the first node in group list and so on.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)
This homework is harder than before.

