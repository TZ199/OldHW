HOMEWORK 7: WORD FREQUENCY MAPS


NAME:  <Tianxin Zhou>


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< None >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 7 >



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

n = total number of words in the sample text file
m = number of unique words in the file
w = width of the sequencing window
p = average number of words observed to follow a particular word

How much memory will the map data structure require, in terms of n, m,
w, and p (order notation for memory use)?

When w = 2: O(m*p)
When w = 3: O(m*p*p)


What is the order notation for performance (running time) of each of
the commands?

l = the length of sentence required

When w = 2:
  Load: O(n*log(m))
  Print: O(log(m) + p)
  Generate_random: O(l*(log(m)+p)
  Generate_max: O(l*(log(m)+p)

When w = 3:
  Load: O(n*log(m)+ n*(log(m)+log(p)))
  Print: O(log(m) + log(p) + p)
  Generate_random: O(l*(log(m)+log(p)+p)
  Generate_max: O(l*(log(m)+log(p)+p)


EXTRA CREDIT
Parsing & using punctuation data, implementation supports *any*
window size, new test cases (describe & summarize performance, but
don't include large datasets with submission).

This extra is sooo hard.



MISC. COMMENTS TO GRADER:
(optional, please be concise!)
