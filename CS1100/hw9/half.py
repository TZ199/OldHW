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
print     

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
    o = 1 
    n = 1
    ad = []
    cou = []
    while o  < 8 :
        stri = sl[o+p]
        
        num = stri[::-1].find(ref)
        if num == -1:
            num = 6
       
        
        cou.append(6-num)
        o += 1
        
    
    ma = max(cou)
    while n < 8 :
        strin = sl[n+p][:(ma)] + reb
        
        ad2.append(strin)
        n += 1
    d2[sl[p]]=ad2
    p += 8    
d2["32 ' '"] = d["32 ' '"]
#print kerning
w1 = ''
w2 = ''
w3 = ''
w4 = ''
w5 = ''
w6 = ''
w7 = ''
for w in word:
    if w == ' ':
        d2[w] = d2["32 ' '"]
    else:
        for s in sl:
            if w in s:
                d2[w] = d2[s]

    w1 += reb + d2[w][0]
    w2 += reb + (d2[w][1])
    w3 += reb + (d2[w][2])
    w4 += reb + (d2[w][3])
    w5 += reb + (d2[w][4])
    w6 += reb + (d2[w][5])
    w7 += reb + (d2[w][6])
print 'with kerning'
print w1[1:]
print w2[1:]
print w3[1:]
print w4[1:]
print w5[1:]
print w6[1:]
print w7[1:]
        
     
    
        
        
    

        
        



































