import snap
import sys

# Simple script to re-index to 0-indexed graph.

graph = sys.argv[1]
Graph = snap.LoadEdgeList(snap.PNGraph, graph)
snap.DrawGViz(Graph, snap.gvlDot, "output.png")

