from sage.all_cmdline import *   # import sage library

_sage_const_0 = Integer(0); _sage_const_2 = Integer(2); _sage_const_1 = Integer(1); _sage_const_15 = Integer(15); _sage_const_100 = Integer(100)

# Initialisation
if len(sys.argv) != 3:
    print('Usage : ' + sys.argv[0] + ' input_file output_file')
    sys.exit(2)
graph_descriptor = open(sys.argv[1], "r")
outputPath = sys.argv[2]
d={}
lines =  graph_descriptor.readlines()

# Construction
greenEdges = [];
for i in range(_sage_const_0 ,len(lines),_sage_const_2 ):
	d[int(lines[i])] = []
	voisins = lines[i+_sage_const_1 ].split(";")
	for j in range(len(voisins)-_sage_const_1 ):
		d[int(lines[i])].append(int(voisins[j].split(":")[_sage_const_0 ]))
		if(voisins[j].split(":")[_sage_const_1 ] == 'v'):
			greenEdges.append((int(lines[i]), int(voisins[j].split(":")[_sage_const_0 ])))
graph_descriptor.close();
G = Graph(d)

# Green or black
edge_colors = {}
edge_colors['green'] = []
for e in G.edges():
	if((e[_sage_const_0 ],e[_sage_const_1 ]) in greenEdges):
		edge_colors['green'].append(e)

# Show and save
G.show(figsize=[_sage_const_100 ,_sage_const_100 ], edge_color='black',  edge_colors=edge_colors)
G.plot(edge_color='black',  edge_colors=edge_colors).save(outputPath)

