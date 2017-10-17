file = open("simple_font.txt","r")
line = file.readline().strip()
width = int(line.split(" ")[0])
height = int(line.split(" ")[1])

lines = file.readlines()
i = 0
letters = {}
letters_kerning = {}
while i < len(lines):
    line = lines[i].strip()
    letter = line.split(" ")[0]
    letter = chr(int(letter))
    matrix = []
    for j in range(height):
        matrix.append(lines[i + j + 1].strip())
    letters[letter] = matrix
    matrix_kerning = matrix[:]
    
    if letter == ' ':
        letters_kerning[' '] = matrix
        i += height + 1
        continue
    
    # zip kerning letter matrix
    while True:
        col = len(matrix_kerning[0]) - 1
        if col <= 0:
            break
        has_foreground = False
        for line in matrix_kerning:
            if line[col] == '#':
                has_foreground = True
                break
        if has_foreground:
            break
        else:
            for row in range(len(matrix_kerning)):
                line = matrix_kerning[row]
                matrix_kerning[row] = line[:-1]
    
    # append . to last column
    for row in range(len(matrix_kerning)):
        line = matrix_kerning[row]
        matrix_kerning[row] = str("%s."%line)
    
    letters_kerning[letter] = matrix_kerning
#     print(letter)
#     for line in matrix_kerning:
#         print(line)
#     print()
    i += height + 1

file.close()


message = raw_input("Please enter the message ==> ")
background = raw_input("Please enter the background char (enter for .) ==> ")
foreground = raw_input("Please enter the foreground char (enter for #) ==> ")
print("No kerning")
for row in range(height):
    for i in range(len(message)):
        char = message[i]
        line = letters[char][row]
        for c in line:
            if c == '.':
                print background,
            elif c == '#':
                print foreground,
        if i < len(message) - 1:
            print background,
    print 
    
print 
print("With kerning")

for row in range(height):
    for i in range(len(message)):
        char = message[i]
        line = letters_kerning[char][row]
        for c in line:
            if c == '.':
                print background,
            elif c == '#':
                print  foreground,
        if i < len(message) - 1:
            print background,
    print 

    