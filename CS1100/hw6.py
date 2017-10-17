### Author: Bijia Zhao
import textwrap

def find_top(l):
    num = []
    top = []
    same_num = []
    name = []
    for i in range(1,len(l)):
        num.append(int(l[i][6]))
    max_num = list(set(num))
    max_num.sort()
    max_num.reverse()
    for i in range(10):
        for j in range(len(num)):
            if num[j] ==max_num[i]:
                same_num.append(l[j+1]) 
        for info in same_num:
            name.append(info[0])
        name.sort()
        name.reverse()
        for n in name:
            for info in same_num:
                if n == info[0]:
                    top.append(info)
        same_num = []
        name = []
    return top[:10]

def other_villains(villain, l):
    print "%s in %s stories, with the following other villains:"%(villain[0],villain[6])
    print "=================================================="
    story = villain[-1].split(',')
    v = set([villain[0]])
    other_v = [] 
    all_other_v = []
    story_only = []
    for s in story:
        for info in l:
            other_s = info[-1].split(',')
            for s2 in other_s:
                if s.strip() == s2.strip():
                    other_v.append(info[0])
        other_v = set(other_v)
        if other_v.issubset(v):
            story_only.append(s.strip())
        all_other_v += other_v
        other_v = []
    all_other_v = set(all_other_v)
    all_other_v = all_other_v.difference(v)
    all_other_v = list(all_other_v)
    all_other_v.sort()
    line = ""
    for name in all_other_v:
        line += name.strip() 
        if name != all_other_v[-1]:
            line += ', '
    line = textwrap.wrap(line,70)
    for l in line:
        print "     "+l
    print 
    
    if story_only == []:
        print "There are no stories with only this villain"
        print "=================================================="
    else:
        print "The stories that only features %s are:"%villain[0]
        print "=================================================="
        story_only = list(set(story_only))
        story_only.sort()
        line = ''
        for story in story_only:
            line += story.strip()
            if story != story_only[-1]:
                line += ', '   
        line = textwrap.wrap(line,70)
        for l in line:
            print "     "+l

if __name__ == '__main__':
    
    f = open('DrWhoVillains.tsv')
    f = f.read().strip()
    f = f.split('\n')
    for i in range(len(f)):
        f[i] = f[i].split('\t')
    top = find_top(f)

    num = 0
    while num != str(-1):
        print
        for i in range(10):
            print "%d. "%(i+1) + top[i][0]
        print "Please enter a number between 1 and 10, or -1 to end "
        num = raw_input("Enter a villain ==> ")
        print num
        if num.isdigit():
            num = int(num)
            if num<11 and num>0:
                other_villains(top[num-1], f)
                print
        if num == str(-1):
            print 'Exiting'
        
