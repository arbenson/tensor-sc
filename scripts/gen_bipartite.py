import snap
import numpy as np

G = snap.PNGraph.New()
N1 = 10
N2 = 20

set1 = range(N1)
set2 = range(N1, N1 + N2)

def AddEdge(G, i, j):
    if not G.IsEdge(i, j):
        G.AddEdge(i, j)

for i in set1:
    G.AddNode(i)

for i in set2:
    G.AddNode(i)

# Attach nodes in set1 to themselves with high probability
p = 0.9
for i in set1:
    for j in set1:
        if i != j and np.random.random() < p:
            G.AddEdge(i, j)

for i in set2:
    for _ in xrange(int(p * N1)):
        trg = int(np.random.randint(0, N1))
        AddEdge(G, i, trg)
        AddEdge(G, trg, i)

out_file = '../data/bipartite-%d.txt' % (N1 + N2)
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
