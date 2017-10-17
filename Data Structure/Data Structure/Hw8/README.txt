HOMEWORK 8: B+ TREES


NAME:  < Tianxin Zhou >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< 
Lab10, Lecture 19 notes, 
https://en.wikipedia.org/wiki/B%2B_tree
http://btechsmartclass.com/DS/U5_T3.html
Classmate: Daming Xu,   we read the assignment description together. 
>

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 30 >


TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your implementation?

For this homework, I used Dr. Memory, std::cout and gdb to debug. 
I have met segmentation fault many times. 
When I met the seg fault, firstly, I added the std::cout in test case code.
If I cannot find the bug location, I used Dr. Memory to check the memory bug.
Since Dr. Memory can display the bug location clearly, I can easily find the bug.
If I still have trouble with the bug, I would use GDB to use traceback to locate the seg fault, and set breakpoint to check the variable.

Since this homework has assumption that the key passed into is unique, and a node must be split and has an odd number of keys, I can skip many corner case. And I have tested the copy of empty tree, copy of one node tree, and insert into empty tree and one node tree.



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

I have completed the extra credit, thank you very much!

