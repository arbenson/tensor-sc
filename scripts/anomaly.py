import snap
import numpy as np

G = snap.PNGraph.New()

set1 = range(0, 2)
set2 = range(2, 4)
set3 = range(4, 6)
set4 = range(6, 20)

for i in set1 + set2 + set3 + set4:
    G.AddNode(i)

def add_edges(side1, side2, p):
    for i in side1:
        for j in side2:
            if np.random.random() < p and i != j:
                G.AddEdge(i, j)

# Choose random edges to hook up with
def AddRandEdges(side1, side2, num, forward):
    for i in side1:
        ends = [int(x) for x in np.random.choice(side2, num)]
        for j in ends:
            if forward:
                if not G.IsEdge(i, j) and i != j:
                    G.AddEdge(i, j)
            else:
                if not G.IsEdge(j, i) and i != j:
                    G.AddEdge(j, i)

p = 1
add_edges(set1, set2, p)
add_edges(set2, set3, p)
add_edges(set3, set1, p)

num = 2
AddRandEdges(set1, set4, num, True)
AddRandEdges(set2, set4, num, True)
AddRandEdges(set3, set4, num, True)

num = 1
AddRandEdges(set1, set4, num, False)
AddRandEdges(set2, set4, num, False)
AddRandEdges(set3, set4, num, False)

p = 0.25
add_edges(set4, set4, p)


if 1:
    in_avg = np.mean([G.GetNI(i).GetInDeg() for i in set1 + set2 + set3])
    out_avg = np.mean([G.GetNI(i).GetOutDeg() for i in set1 + set2 + set3])
    print in_avg, out_avg
if 1:
    in_avg = np.mean([G.GetNI(i).GetInDeg() for i in set4])
    out_avg = np.mean([G.GetNI(i).GetOutDeg() for i in set4])
    print in_avg, out_avg

out_file = '../data/anomaly.txt'
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
