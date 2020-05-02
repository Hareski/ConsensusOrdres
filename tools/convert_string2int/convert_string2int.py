import re

# Initialisation
blackFile = open("noir.txt", "r")
greenFile = open("vert.txt", "r")
nameToId = {}
counter = 0

# Graphe noir
print("Noir:")
if blackFile.mode == 'r':
    lines = blackFile.readlines()
    for line in lines:
        match = re.match(r"([^ ]+) ([^ ]+)\n", line)
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
print("Vert:")
if greenFile.mode == 'r':
    lines = greenFile.readlines()
    for line in lines:
        match = re.match(r"([^ ]+) ([^ ]+)\n", line)
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
