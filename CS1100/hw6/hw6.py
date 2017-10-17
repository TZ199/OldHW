ope = open('DrWhoVillains.tsv')
ope = ope.read().strip()
ope = ope.split('\n')
'''ÕÒÃû×Ö'''
fi = []
for i in range(1,len(ope)):
    villain = ope[i].split('\t')
    v = [int(villain[6]),villain[0]]
    fi.append(v)
fi.sort()
print fi
'''###'''
print 'Please enter a number between 1 and 10, or -1 to end'
num = raw_input("Enter a villain ==>")
print num
'''###'''
print fi