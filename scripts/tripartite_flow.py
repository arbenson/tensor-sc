import snap
import numpy as np

G = snap.PNGraph.New()

set1 = range(0, 4)
set2 = range(4, 7)
set3 = range(7, 9)
set4 = range(9, 12)

for i in set1 + set2 + set3 + set4:
    G.AddNode(i)

def add_edges(side1, side2, p):
    for i in side1:
        for j in side2:
            if np.random.random() < p and i != j:
                G.AddEdge(i, j)

p = 0.8
add_edges(set1, set2, p)
add_edges(set2, set3, p)
add_edges(set3, set1, p)

p = 0.2
add_edges(set1, set1, p)
add_edges(set2, set2, p)
add_edges(set3, set3, p)

p = 0.2
add_edges(set4, set4, p)

p = 0.4
add_edges(set4, set1 + set2 + set3, p)
add_edges(set1 + set2 + set3, set4, p)

if 1:
    in_avg = np.mean([G.GetNI(i).GetInDeg() for i in set1 + set2 + set3])
    out_avg = np.mean([G.GetNI(i).GetOutDeg() for i in set1 + set2 + set3])
    print in_avg, out_avg
if 1:
    in_avg = np.mean([G.GetNI(i).GetInDeg() for i in set4])
    out_avg = np.mean([G.GetNI(i).GetOutDeg() for i in set4])
    print in_avg, out_avg

out_file = '../data/tripartite_flow.txt'
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
