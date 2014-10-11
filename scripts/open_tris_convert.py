import snap
import numpy as np
import sys

def TriClosure(G, name):
    G_new = snap.PNGraph.New()
    for u_ in G.Nodes():
        u = u_.GetId()
        G_new.AddNode(u)
    
    for u_ in G.Nodes():
        u = u_.GetId()
        for v in u_.GetInEdges():
            v_ = G.GetNI(v)
            for w in u_.GetOutEdges():
                if v != w and not G.IsEdge(v, w) and not G.IsEdge(w, v):
                    G_new.AddEdge(v, w)

    snap.SaveEdgeList(G_new, name)
   
def main():
    name = sys.argv[1]
    out_name = name.split('.txt')[0] + '-tri-closure.txt'
    G = snap.LoadEdgeList(snap.PNGraph, name, 0, 1)
    TriClosure(G, out_name)

if __name__ == '__main__':
    main()

