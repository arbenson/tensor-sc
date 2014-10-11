import snap
import sys

'''
Simple script to get maximal bi-connected component.
'''

graph = sys.argv[1]
Gin = snap.LoadEdgeList(snap.PNGraph, graph)
BiCon = snap.GetMxBiCon(Gin)
Gout = snap.ConvertGraph(snap.PNGraph, BiCon, True)
print 'Number of nodes: ', Gout.GetNodes()
print 'Number of edges: ', Gout.GetEdges()
out_graph = graph.split('.txt')[0] + '-bicon.txt'
snap.SaveEdgeList(Gout, out_graph)

