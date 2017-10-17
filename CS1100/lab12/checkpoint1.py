import sys

sys.setrecursionlimit(4000)

def add(m,n):
    if n == 0:
        return m 
    else:
        
        return add(m, n-1) +1

    
def mult(m,n):
    if n == 1:
        return m
    else:
        return add(m,mult(m,n-1))
    
def power(m,n):
    if n == 1:
        return m 
    else:
        return mult(m,power(m,n-1))

print add(0,984)  
print mult(1,9)
print power(6,4)

    
