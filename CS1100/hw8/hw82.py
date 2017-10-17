import json
for line in open('businesses.json'):
     b = json.loads(line)
     print b
     break
import json
import hw8util
if __name__ == '__main__':
     data = []
     for line in open('businesses.json'):
          data.append(json.loads(line))
     while True:
          print "=============================="          
          
          place = raw_input('Where (-1 to exit) ==>')
          print place
          if place == -1:
               break
          if place == '':
               break
          
          fliters = raw_input('Filter by what ==> ')
          print fliters          
          
          
          
     
     
          
     
     
     
     
     
