from Pig import *
from Bird import *

birdsfilename = input("Enter the name of the birds file ==> ")
pigsfilename = input("Enter the name of the pigs file ==> ")
print()

# init birds from file
birds = list()
birdsfile = open(birdsfilename)
for line in birdsfile:
    attrs = line.split("|")
    bird = Bird(attrs[0],float(attrs[1]),float(attrs[2]),float(attrs[3]),float(attrs[4]),float(attrs[5]))
    birds.append(bird)
birdsfile.close()

# print all birds
print("Num birds %d:"%(len(birds)))
for bird in birds:
    print("%s (%.1f,%.1f)"%(bird.name,bird.x,bird.y))
print("....")

# init pigs from file
pigs = list()
pigsfile = open(pigsfilename)
for line in pigsfile:
    attrs = line.split("|")
    pig = Pig(attrs[0],float(attrs[1]),float(attrs[2]),float(attrs[3]))
    pigs.append(pig)
pigsfile.close()

# print all pigs
print("Num pigs %d:"%(len(pigs)))
for pig in pigs:
    print("%s (%.1f,%.1f)"%(pig.name,pig.x,pig.y))
print("....")

# start simulation
time = 0
bird = birds[0]
print("Time %d: %s starts at (%.1f,%.1f)"%(time,bird.name,bird.x,bird.y))
while len(birds) > 0 or len(pigs) > 0:
    # bird move
    bird.move()
    time += 1
    # check if the bird pops a pig
    for pig in pigs:
        if bird.is_poped(pig):
            pigs.remove(pig)
            pig.pop()
            bird.decrease_speed()
            print("Time %d: %s at location (%.1f,%.1f) has poped %s"%(time,bird.name,bird.x,bird.y,pig.name))
            print("Time %d: %s at location (%.1f,%.1f) now has (dx,dy) = (%.1f,%.1f)"%(time,bird.name,bird.x,bird.y,bird.dx,bird.dy))

    # check if bird left the game
    if bird.is_left():
        print("Time %d: %s at location (%.1f,%.1f) has left the game"%(time,bird.name,bird.x,bird.y))
        birds.remove(bird)
        if len(birds) > 0:
            bird = birds[0]
            print("Time %d: %s starts at (%.1f,%.1f)"%(time,bird.name,bird.x,bird.y))
        else:
            break

    # check if bird reaches a mininum speed
    if bird.speed() < 6:
        print("Time %d: %s at location (%.1f,%.1f) with speed %.1f stops"%(time,bird.name,bird.x,bird.y,bird.speed()))
        birds.remove(bird)
        if len(birds) > 0:
            bird = birds[0]
            print("Time %d: %s starts at (%.1f,%.1f)"%(time,bird.name,bird.x,bird.y))
        else:
            break

    # if all pigs poped stop simulation
    if len(pigs) == 0:
        break

# print result
if len(pigs) == 0:
    print("Time %d: All pigs poped. The birds win!"%(time))
elif len(birds) == 0:
    print("Time %d: No more birds. The pigs win!"%(time))
    print("Remaining pigs:")
    for pig in pigs:
        print(pig.name)
    
