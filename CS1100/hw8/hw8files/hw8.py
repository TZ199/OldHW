import json
import hw8util

if __name__ == '__main__':
    data = []
    for line in open('businesses.json'):
        data.append(json.loads(line))
    while True:
        print("==============================")
        location = input("Where (-1 to exit) ==> ")
        print(location)
        if location == '-1':
            break
        categories = {}
        fliters = input('Filter by what ==> ')
        print(fliters)
        fliter_in = []
        fliter_not_in = []

        # split fliters
        if len(fliters) > 0:
            for f in fliters.split(','):
                f = f.strip().lower()
                if f[0] == '~':
                    fliter_not_in.append(f)
                else:
                    fliter_in.append(f)
        
        businesses = []

        # fliter data
        for d in data:
            if d['city'].lower() == location.lower() or d['full_address'].lower().find(location.lower()) >= 0:
                category = d['categories']
                # if no fliters
                if len(fliter_in) == 0 and len(fliter_not_in) == 0:
                    for c in category:
                        categories[c] = categories.get(c,0) + 1
                    businesses.append(d)
                else:
                    is_in = []
                    not_in = []
                    for c in category:
                        if c.lower().strip() in fliter_not_in:
                            not_in.append(c.lower().strip())
                        if c.lower().strip() in fliter_in:
                            is_in.append(c.lower().strip())

                    if len(fliter_not_in) == 0 and len(fliter_in) > 0 and sorted(is_in) == sorted(fliter_in):
                        for c in category:
                            categories[c] = categories.get(c,0) + 1
                        businesses.append(d)

                    if len(fliter_in) == 0 and len(fliter_not_in) > 0 and sorted(not_in) != sorted(fliter_not_in):
                        for c in category:
                            categories[c] = categories.get(c,0) + 1
                        businesses.append(d)

                    if len(fliter_in) > 0 and len(fliter_not_in) > 0 and sorted(not_in) != sorted(fliter_not_in) and sorted(is_in) == sorted(fliter_in):
                        for c in category:
                            categories[c] = categories.get(c,0) + 1
                        businesses.append(d)
                    
        # print catgories
        for category in sorted(categories):
            print('       ',category,' (%d)'%categories[category])

        # calculate distance
        distance = []
        for b1 in businesses:
            for b2 in businesses:
                dis = hw8util.dist(b1['latitude'],b1['longitude'],b2['latitude'],b2['longitude'])
                distance.append(dis)
        if len(distance) > 0:
            max_dis = max(distance)
        else:
            max_dis = 0
        print('Found %d businesses in %.2f radius'%(len(businesses),max_dis))

        choice = input('Print the businesses (y/n)? ==> ')
        print(choice)
        if choice == 'y':
            for i in range(len(businesses)):
                b = businesses[i]
                print('%d. %s (%s reviews, %s stars) [%s]'%((i+1),b['name'],b['review_count'],b['stars'],b['full_address'].replace('\n',' ')))
