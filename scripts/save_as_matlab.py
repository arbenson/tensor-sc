import snap
import sys

# Simple script to save data as a matlab .mat file.

graph = sys.argv[1]
Graph = snap.LoadEdgeList(snap.PNGraph, graph)
snap.SaveMatlabSparseMtx(Graph, sys.argv[1].split('.txt')[0] + '.mat')

