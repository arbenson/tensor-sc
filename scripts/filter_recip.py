import snap
import sys

# Filter the graph

graph = sys.argv[1]
G = snap.LoadEdgeList(snap.PNGraph, graph)

G_filter = snap.PNGraph.New()
for u_ in G.Nodes():
    u = u_.GetId()
    G_filter.AddNode(u)

edge_hash = {}
for u_ in G.Nodes():
    u = u_.GetId()
    for v in u_.GetOutEdges():
        for w in u_.GetInEdges():
            if (G.IsEdge(v, w)):
                # w -> v -> u -> w
                edge_hash[(v, u)] = 1
                edge_hash[(u, w)] = 1
                edge_hash[(w, v)] = 1

for u, v in edge_hash:
    G_filter.AddEdge(u, v)

G_filter = snap.ConvertGraph(snap.PNGraph, G_filter, True)
out_name = graph.split('.txt')[0] + '-filter.txt'
snap.SaveEdgeList(G_filter, out_name)
