def parse_line(txt):
    txt = txt.split("/",3)
    if txt[0].isdigit()== False or txt[1].isdigit()== False or txt[2].isdigit()== False:
        return None
    return (int(txt[0]), int(txt[1]),int(txt[2]),txt[3])

def get_line(fname,parno,lineno):
    f = open(fname+'.txt')
    f = f.read()
    f = f.split("\n\n")
    p = f[parno]
    p = p.split("\n")
    return p[lineno]

f = raw_input("Please file number ==> ")
p = raw_input("Please enter paragraph number ==> ")
l = raw_input("Please enter the line number ==> ")
pro = open('program.py','w')


for i in range(0,1000):      
    ll = get_line(str(f),int(p)-1,int(l)-1)     
    if parse_line(ll) == None:
        print "Egg hunting fails"
        break
    f,p,l,read_line = parse_line(ll)
    print parse_line(ll)
    if f == 0:
        break    
    pro.write(read_line + '\n')

    
pro.close()