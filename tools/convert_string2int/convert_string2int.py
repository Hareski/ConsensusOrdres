import re
import sys

# Initialisation
if len(sys.argv) != 3:
    print('Usage : ' + sys.argv[0] + ' black_relations_file green_relations_file')
    sys.exit(2)
blackFile = open(sys.argv[1], "r")
greenFile = open(sys.argv[2], "r")
nameToId = {}
counter = 0

# Graphe noir
print("Noir:")
if blackFile.mode == 'r':
    lines = blackFile.readlines()
    for line in lines:
        match = re.match(r"([^ ]+) ([^ ]+).*\n", line)
        if match:
            if not (match.group(1) in nameToId):
                nameToId[match.group(1)] = counter
                counter = counter + 1
            if not (match.group(2) in nameToId):
                nameToId[match.group(2)] = counter
                counter = counter + 1
            print(str(nameToId[match.group(1)])
                  + " " +
                  str(nameToId[match.group(2)]))

# Graphe vert
countRelation = 0;
print("Vert:")
if greenFile.mode == 'r':
    lines = greenFile.readlines()
    for line in lines:
        match = re.match(r"([^ ]+) ([^ ]+).*\n", line)
        if match:
            if not (match.group(1) in nameToId):
                nameToId[match.group(1)] = counter
                counter = counter + 1
            if not (match.group(2) in nameToId):
                nameToId[match.group(2)] = counter
                counter = counter + 1
            countRelation = countRelation + 1;
            print(str(nameToId[match.group(1)])
                  + " " +
                  str(nameToId[match.group(2)]))

print("Nombre de sommets : " + str(counter))
print("Nombre d'aretes vertes : " + str(countRelation))
