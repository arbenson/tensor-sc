import snap
import sys

# Simple script to re-index to 0-indexed graph.

graph = sys.argv[1]
if len(sys.argv) > 2 and sys.argv[2] == 2:
    Gin = snap.LoadEdgeList(snap.PUNGraph, graph)
else:
    Gin = snap.LoadEdgeList(snap.PNGraph, graph)
    
Gout = snap.ConvertGraph(snap.PNGraph, Gin, True)
print 'Number of nodes: ', Gout.GetNodes()
print 'Number of edges: ', Gout.GetEdges()
snap.SaveEdgeList(Gout, graph)
