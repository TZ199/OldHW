# input
word = raw_input('Please enter the message ==> ')
print word
reb = raw_input('Please enter the background char (enter for .) ==> ')
print reb
ref = raw_input('Please enter the foreground char (enter for #) ==> ')
print ref
s=open('simple_font.txt')
simple = s.read()

# replace
simple = simple.replace('.',reb)
simple = simple.replace('#',ref)

# to be a list 
sl= simple.split('\n')
sl.remove('6 7')

# make a dict
i = 0
d = dict()
while i < 760:
    m = 1 
    ad = []
    while m <8:
        ad.append(sl[i+m])
        m += 1
        
    d[sl[i]]=ad
    i += 8
print d    

# first one 
s1 = ''
s2 = ''
s3 = ''
s4 = ''
s5 = ''
s6 = ''
s7 = ''
for w in word:
    if w == ' ':
        d[w] = d["32 ' '"]
    else:
        for s in sl:
            if w in s:
                d[w] = d[s]

    s1 += d[w][0]
    s2 += (d[w][1])
    s3 += (d[w][2])
    s4 += (d[w][3])
    s5 += (d[w][4])
    s6 += (d[w][5])
    s7 += (d[w][6])
print 'no kerning'
print s1
print s2
print s3
print s4
print s5
print s6
print s7

# second one kerning , make a another dic
p = 0
d2 = dict()
while p < 760:
    ad2 = []    
    m = 1 
    n = 1
    ad = []
    cou = []
    while m  < 8 :
        stri = sl[m+p]
        cou.append(stri[::-1].find(ref))
        m += 1
    ma = max(cou)
    while n < 8 :
        strin = sl[m+p][:ma] + reb
        ad2.append(strin)
    d[sl[p]]=ad2
    i += 8    
        
        
        
    

        
        



































