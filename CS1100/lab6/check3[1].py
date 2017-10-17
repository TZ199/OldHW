import lab06_util

def show_form(bd):
    print '-------------------------\n',
    i = 0
    while i<9:
        print '|',
        j = 0
        while j<9:
            print bd[i][j],
            if j%3 == 2:
                print '|',
            j += 1
        print '\n',
        if i%3 == 2:
            print '-------------------------\n',
        i += 1    
    
def ok_to_add(x,y,n,bd):
    i = 0
    while i<9:
        if bd[int(x)][i] == n:
            return 'This number cannot be added'
        i += 1
    i = 0
    while i<9:
        if bd[i][int(y)] == n:
            return 'This number cannot be added'
        i += 1
    i = int(x)/3*3
    while i<int(x)/3*3+3:
        j = int(y)/3*3
        while j<int(y)/3*3+3:
            if bd[i][j] == n:
                return 'This number cannot be added'
            j += 1
        i += 1
    return ''

def vertify_board(bd):
    i = 0
    while i<9:
        j = 0
        while j<9:
            if bd[i][j] == '.':
                return 'not solved'
            j += 1
        i += 1
    
    i = 0
    while i<9:
        j = 0
        while j<9:
            n = bd[i][j]
            bd[i][j] = ' '
            if ok_to_add(i,j,n,bd) == 'This number cannot be added':
                bd[i][j] = n                
                return 'not solved'
            bd[i][j] = n            
            j += 1
        i += 1
    return 'solved'

filename = raw_input('Please enter a file name ==> ')
bd = lab06_util.read_sudoku(filename)
show_form(bd)
print vertify_board(bd)
while vertify_board(bd) == 'not solved':
    x = int(raw_input('Please enter a raw ==> '))-1
    y = int(raw_input('Please enter a column ==> '))-1
    n = raw_input('Please enter a number ==> ') 
    if ok_to_add(x,y,n,bd) == '':
        bd[x][y] = n  
    show_form(bd)            