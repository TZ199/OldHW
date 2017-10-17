HOMEWORK 3: MATRIX CLASS


NAME:  < Tianxin Zhou >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< insert collaborators / resources >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 10 >



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of m = the number of rows and n = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get O(1)

set O(1)

num_rows O(1)

get_column O(m) 

operator<< O(mn)

quarter O(mn)

operator== O(mn)

operator!= O(mn)

swap_rows O(n)

rref (provided in matrix_main.cpp) O(nmm)


TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your Matrix class design & implementation?

I used dr.memory to test memory leak, and printf to test the corner cases and some bugs that related to syntax or algorithm.

For corner case, I have tried to write my own function in main with some cases like if no data in matrix, or if we set a data is exceed the scale. Then printout to check if I get the right answer.



EXTRA CREDIT: 
Indicate here if you implemented resize() for extra credit.  
Also document the order notation for your resize() function.

I have implemented the resize(). This function takes in three arguments: target rows, target columns and auto fill value. If the target size is bigger than the actual size, change the size of data and fill with provided value. Otherwise just shrink the size.

If target size is more bigger than actual size, the order notation for resize will be O(rc) r = target rows c  = target columns.

If target size is more smaller than actual size, the order notation for resize will be O(mn) m = target rows n = target columns.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

