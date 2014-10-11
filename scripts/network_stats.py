import sys

import numpy as np
import snap

# Enumerate all of the directed 3 cycles in a graph.

def GetStatistics(G):
    dir3cycles = 0
    recip_edges = 0
    for u_ in G.Nodes():
        u = u_.GetId()
        for v in u_.GetOutEdges():
            for w in u_.GetInEdges():
                if (u == v or v == w or w == u):
                    continue
                if (u < min(v, w) and G.IsEdge(v, w)):
                    # w -> v -> u -> w
                    dir3cycles += 1

    print '|V| & |E| & Total D3C & Fraction edges in D3C & Recip. Edges'
    print '%d & %d & %d' % (G.GetNodes(), G.GetEdges(), dir3cycles)
                                    
def main():
    name = sys.argv[1]
    print name
    G = snap.LoadEdgeList(snap.PNGraph, name, 0, 1)
    MxScc = snap.GetMxScc(G)
    GetStatistics(MxScc)

if __name__ == '__main__':
    main()

