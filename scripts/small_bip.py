import snap
import numpy as np

G = snap.PNGraph.New()

n = 5
set1 = range(0, n)
set2 = range(n, 2 * n)
set3 = range(2 * n, 3 * n)
set4 = range(3 * n, 4 * n)

for i in set1 + set2 + set3 + set4:
    G.AddNode(i)

def add_edges(side1, side2, p):
    for i in side1:
        for j in side2:
            if np.random.random() < p and i != j:
                G.AddEdge(i, j)

p = 0.7
add_edges(set1, set2, p)
add_edges(set2, set1, p)
p = 0.8
add_edges(set3, set4, p)
add_edges(set4, set3, p)

# Now add some nodes between them
p = 0.4
add_edges(set1, set3, p)
add_edges(set2, set4, p)
p = 0.3
add_edges(set3, set1, p)
add_edges(set4, set2, p)

p = 0.1
add_edges(set1, set1, p)
add_edges(set2, set2, p)
add_edges(set3, set3, p)
add_edges(set4, set4, p)

out_file = '../data/small-bip.txt'
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
