# Tianxin Zhiu
# HW2
# Reference https://www.cs.cornell.edu/courses/cs312/2002sp/lectures/rec21.htm,
# Reference2 http://univasity.iteye.com/blog/1170216


# File process
file = open("input.txt", "r")
output = open("output.txt", "w")
# Key as the pathl + "+" + paths and value be the maximum of the effeciency
map = dict()
debug = 0
# Number of beds in the shelter
b, p, L, S, A = 0, 0, 0, 0, 0
bList = []
pList = []
appList = []


# Process of streaming data from input
for i, line in enumerate(file):
    line = line.strip('\r\n')
    if not line:
        continue
    if i == 0:
        b = int(line)
    elif i == 1:
        p = int(line)
    elif i == 2:
        L = int(line)
    elif i > 2 and i <= 2+L:
        bList.append(line)
    elif i == L+3:
        S = int(line)
    elif i > L+3 and i <= L+3+S:
        pList.append(line)
    elif i == L+S+4:
        A = int(line)
    else:
        appList.append(line)
# End of the streaming data
appListCheck = [1] * len(appList)
bspace = [b]*7
pspace = [p]*7

# Lahsa
for i in bList:
    # Mark the applicant as picked
    appListCheck[int(i)-1] = 0
    for ci, c in enumerate(appList[int(i)-1][-7:]):
        if int(c):
            bspace[ci] -= 1
# Spla
for i in pList:
    # Mark the applicant as picked
    appListCheck[int(i)-1] = 0
    for ci, c in enumerate(appList[int(i)-1][-7:]):
        if int(c):
            pspace[ci] -= 1


# For parking appli is the index of the applicant
def checkSpla(appli, pspace):
    applicant = appList[appli]
    if applicant[10] == "N" and applicant[11] == "Y" and applicant[12] == "Y":
        # Check if we have the space for the new applicant
        for ci, c in enumerate(applicant[-7:]):
            if int(c):
                if pspace[ci] < int(applicant[13+ci]):
                    return False
        return True
    else:
        return False


# For beds appli is the index of the applicant
def checkLahsa(appli, bspace):
    applicant = appList[appli]
    if (applicant[5] == "F" and int(applicant[6:9]) > 17 and
            applicant[9] == 'N'):
        # Check if we have the space for the new applicant
        for ci, c in enumerate(applicant[-7:]):
            if int(c):
                if bspace[ci] < int(applicant[13+ci]):
                    return False

        return True
    # If the applicant is not fitted for the requiremnets
    else:
        return False


if debug:
    print(bspace)
    print(pspace)
    print(b, p, L, S, A)
    print(bList)
    print(pList)
    print(appList)
    print(appListCheck)
    for i in range(len(appList)):
        if checkSpla(i):
            print("True")
        else:
            print("False")
    for i in range(len(appList)):
        if checkLahsa(i):
            print("True")
        else:
            print("False")


def search(choice, count_s, count_l, A_check, pspace, bspace, b_list, p_list):
    # if all applicants are selected
    # if all(x == 0 for x in A_check):
    #    return count_s, count_l
    # If there is no space for applicants
    # if all(v == 0 for v in pspace) and all(z == 0 for z in bspace):
    #    return count_s, count_l

    global debug
    bigcheck = 0
    max_s, max_l = 0, 0

    for xd, x in enumerate(A_check):
        if x:
            if checkLahsa(xd, bspace):
                bigcheck = 1
            elif checkSpla(xd, pspace):
                bigcheck = 1
    if not bigcheck:
        return count_s, count_l

    # Optimization
    if choice != "F":
        pathl = ''.join(str(p_list))
        paths = ''.join(str(b_list))
        path = pathl+'+'+paths
        if map.get(path):
            return map[path][0], map[path][1]

    ans = 0
    ansl = 0
    index = ""

    # Start the first traverse loop
    if choice == "F":
        # Traverse all possible applicants as the S'next
        for i in range(len(appList)):
            if appListCheck[i] and checkSpla(i, pspace):
                appListCheck[i] = 0
                days = 0
                bspace2 = bspace[:]
                for ci, c in enumerate(appList[i][-7:]):
                    if int(c):
                        bspace2[ci] -= 1
                        days += 1
                curs, curl = search("L", days, 0, appListCheck, pspace,
                                    bspace2, [], [i+1])
                print(i+1, curs)

                appListCheck[i] = 1
                if ans < curs:
                    ans = curs
                    ansl = curl
                    index = appList[i][:5]
        print("Answer is: ", index)
        output.write(index)
        return ans, ansl
    # If there is at least one applicant and at least one space
    # If its Spla's turn
    elif choice == "S":
        # Loop for enter in other recursions
        # Traverse all applicants
        for i in range(len(A_check)):
            A_check2 = A_check[:]
            pspace2 = pspace[:]
            # If there is a person can be picked
            if A_check2[i] and checkSpla(i, pspace2):
                A_check2[i] = 0
                days = 0
                # Update the pspace and days(effeciency)
                for ci, c in enumerate(appList[i][-7:]):
                    if int(c):
                        pspace2[ci] -= 1
                        days += 1
                p_list2 = sorted(p_list + [int(appList[i][:5])])
                # Get the current count

                cur_s, cur_l = search("L", count_s+days, count_l, A_check2,
                                      pspace2, bspace, b_list, p_list2)
                if debug:
                    print("\nS")
                    print(cur_s)
                    print(cur_l)
                    print(A_check2)
                    print(pspace2)
                    print(bspace)
                    print(p_list2)
                    print(b_list)

                # Compare to get the max in the same level
                if cur_s > max_s:
                    max_s = cur_s
                    max_l = cur_l
        # If we can put applicant in
        if max_s != 0 or max_l != 0:
            # Store the maximum into map
            map[path] = [max_s, max_l]
            return max_s, max_l
        # If S have no applicant to choose
        else:
            # return count_s, count_l
            cur_s, cur_l = search("L", count_s, count_l,
                                  A_check, pspace, bspace, b_list, p_list)
            return cur_s, cur_l
    # If its Lahsa's turn
    elif choice == "L":
        # Loop for enter in other recursions
        # Traverse all applicants
        for i in range(len(A_check)):
            A_check2 = A_check[:]
            bspace2 = bspace[:]
            # If there is a person can be picked
            if A_check2[i] and checkLahsa(i, bspace2):
                A_check2[i] = 0
                days = 0
                # Update the pspace and days(effeciency)
                for ci, c in enumerate(appList[i][-7:]):
                    if int(c):
                        bspace2[ci] -= 1
                        days += 1
                b_list2 = sorted(b_list + [int(appList[i][:5])])

                # Get the current count
                cur_s, cur_l = search("S", count_s, count_l+days, A_check2,
                                      pspace, bspace2, b_list2, p_list)

                if debug:
                    print("\nL")
                    print(count_s)
                    print(count_l+days)
                    print(A_check2)
                    print(pspace)
                    print(bspace2)
                    print(p_list)
                    print(b_list2)

                # Compare to get the max in the same level
                if cur_l > max_l:
                    max_s = cur_s
                    max_l = cur_l
        # If we can put applicant in
        if max_s != 0 or max_l != 0:
            map[path] = [max_s, max_l]
            return max_s, max_l

        # If S have no applicant to choose
        else:
            cur_s, cur_l = search("S", count_s, count_l, A_check,
                                  pspace, bspace, b_list, p_list)
            return cur_s, cur_l


# Mark the applicant that cant be selected by both L and S to 0
for i in range(len(appList)):
    if not checkLahsa(i, bspace) and not checkSpla(i, pspace):
        appListCheck[i] = 0

# Enter into the recursion fuction
anss, ansl = 0, 0
anss, ansl = search("F", 0, 0, appListCheck, pspace,
                    bspace, [], [])
