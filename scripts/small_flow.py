import snap

G = snap.PNGraph.New()

for i in range(6):
    G.AddNode(i)

G.AddEdge(0, 1)
G.AddEdge(1, 2)
G.AddEdge(2, 0)

G.AddEdge(1, 0)
G.AddEdge(2, 1)
G.AddEdge(0, 2)

G.AddEdge(3, 4)
G.AddEdge(4, 5)
G.AddEdge(5, 3)

G.AddEdge(1, 3)
G.AddEdge(1, 4)
G.AddEdge(2, 3)
G.AddEdge(2, 4)

G.AddEdge(5, 0)

out_file = '../data/flow.txt'
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
