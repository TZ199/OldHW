'''
This is code to test your implement of the Ball class.  It is driven
by a Python module called nose, which has been imported here.  Nose is
actually relative simple.

To understand it, start by looking at the main code at the bottom of
the file.  The function call nose.runmodule() causes nose to scan back
up through the preceeding code and find all functions that start with
test_.  Nose runs these functions in turn.  Each function must end
with an 'assert' statement followed by a boolean condition.  If the
boolean condition evaluate to True, nose does not say anything.  If a
condition evaluates to False, the "assert" is said to "fail".  Nose
detects these failures and tells you about them.  To see the behavior
of Nose, change some of the assert statements (such as x == 120
to x == 145) and see what happens.

'''

import nose
from Ball import *

def test_move1():
    b = Ball(100,150,20,-15,10,'red')
    b.move()
    (x,y) = b.position()
    assert x == 120 and y == 135

def test_move2():
    b = Ball(190,100,-10,18,25,'green')
    b.move()
    assert b.position() == (180,118) and b.get_color() == 'green'

def test_box1():
    b = Ball(190,100,-10,18,25,'green')
    assert b.bounding_box() == (165,75,215,125)

def test_inside1():
    b = Ball(190,100,-10,18,25,'green')
    assert b.some_inside(170,90)

def test_inside2():
    b = Ball(190,100,-10,18,25,'green')
    assert not b.some_inside(164,190)

if __name__ == "__main__":
    nose.runmodule()
