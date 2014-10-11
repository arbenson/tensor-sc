import snap

G = snap.PNGraph.New()

for i in range(12):
    G.AddNode(i)

# Level 1
G.AddEdge(0, 1)
G.AddEdge(1, 2)
G.AddEdge(2, 0)

G.AddEdge(1, 0)
G.AddEdge(2, 1)
G.AddEdge(0, 2)

G.AddEdge(0, 3)
G.AddEdge(3, 2)

# Level 2
G.AddEdge(4, 5)
G.AddEdge(5, 6)
G.AddEdge(6, 4)
G.AddEdge(6, 7)
G.AddEdge(7, 5)

G.AddEdge(5, 7)

# Level 3
G.AddEdge(8, 9)
G.AddEdge(9, 10)
G.AddEdge(10, 8)
G.AddEdge(10, 11)
G.AddEdge(11, 10)

G.AddEdge(8, 11)

# Level 1 -> 2
G.AddEdge(1, 4)
G.AddEdge(1, 5)
G.AddEdge(1, 7)
G.AddEdge(2, 4)
G.AddEdge(2, 5)
G.AddEdge(3, 5)
G.AddEdge(3, 7)

# Level 2 -> 3
G.AddEdge(6, 8)
G.AddEdge(6, 9)
G.AddEdge(6, 11)
G.AddEdge(7, 8)
G.AddEdge(7, 9)

# Level 1 -> 3
G.AddEdge(3, 9)

# Single link going back up
G.AddEdge(10, 0)

# Extras
G.AddEdge(8, 7)
G.AddEdge(7, 3)

out_file = '../data/large_flow.txt'
print 'data:', out_file
snap.SaveEdgeList(G, out_file)
