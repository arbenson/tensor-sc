import sys

import numpy as np
import snap

symmetrize = True

# Enumerate all of the directed 3 cycles in a graph.

def EnumerateDir3Cycles(G, name):
    ''' Count all directed 3 cycles in the graph.
    Returns an array of dictionaries counts such that
    counts[w][v] is the number of 3-cycles involving the edge w -> v '''
    total = 0
    with open(name, 'w') as output:
        for u_ in G.Nodes():
            u = u_.GetId()
            for v in u_.GetOutEdges():
                for w in u_.GetInEdges():
                    if (v != w and G.IsEdge(v, w)):
                        if not G.IsEdge(u, w) and not G.IsEdge(v, u) and not G.IsEdge(w, v):
                            # w -> u -> v -> w
                            output.write('%d %d %d\n' % (w, v, u))
                            output.write('%d %d %d\n' % (v, w, u))
                            total += 1

    print 'number of pure dir. 3 cycles: ', total


def main():
    name = sys.argv[1]
    out_name = name.split('.txt')[0] + '-d3c-noback.txt'
    G = snap.LoadEdgeList(snap.PNGraph, name, 0, 1)
    EnumerateDir3Cycles(G, out_name)

if __name__ == '__main__':
    main()
