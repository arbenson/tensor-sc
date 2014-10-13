import snap

'''
 This script takes the data for the wiki-RfA network and converts
 it to a SNAP graph.  The wiki-RfA network is available at:

    http://snap.stanford.edu/data/wiki-RfA.html

'''

nodes = {}
edges = {}

def update_dict(src, trg):
    if src not in nodes:
        nodes[src] = len(nodes)
    if trg not in nodes:
        nodes[trg] = len(nodes)

    src_ind = nodes[src]
    trg_ind = nodes[trg]
    edges[(src_ind, trg_ind)] = 1


with open('wiki-RfA.txt') as f:
    src = None
    trg = None
    for line in f:
        if line[0:3] == 'SRC':
            src = line[4:-1]
        if line[0:3] == 'TGT':
            trg = line[4:-1]
            update_dict(src, trg)


G = snap.PNGraph.New()
for key in nodes:
    G.AddNode(nodes[key])

for i, j in edges:
    G.AddEdge(i, j)

snap.SaveEdgeList(G, 'wiki-RfA-net.txt')
