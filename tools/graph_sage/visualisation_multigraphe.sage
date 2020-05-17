d={}
graph_descriptor = open("relations.txt", "r")
lines =  graph_descriptor.readlines()


greenEdges = [];
for i in range(0,len(lines),2):
	d[int(lines[i])] = []
	voisins = lines[i+1].split(";")
	for j in range(len(voisins)-1):
		d[int(lines[i])].append(int(voisins[j].split(":")[0]))
		if(voisins[j].split(":")[1] == 'v'):
			greenEdges.append((int(lines[i]), int(voisins[j].split(":")[0])))
graph_descriptor.close();
G = Graph(d)

edge_colors = {}
edge_colors['green'] = []

for e in G.edges():
	if((e[0],e[1]) in greenEdges):
		edge_colors['green'].append(e)

G.show(figsize=[15,15], edge_color='black',  edge_colors=edge_colors)
G.plot(edge_color='black',  edge_colors=edge_colors).save('graph.png')
