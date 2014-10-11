import sys

import numpy as np
import snap

# Enumerate all of the directed 3 cycles in a graph.

symmetrize = True

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
                    if (u == v or v == w or w == u):
                        continue
                    if not G.IsEdge(v, w):
                        continue
                    
                    # w -> u -> v -> w
                    output.write('%d %d %d\n' % (w, v, u))
                    total += 1
                    if symmetrize:
                        output.write('%d %d %d\n' % (v, w, u))
                        total += 1

    print 'number of dir. 3 cycles (no reciprocated edges): ', total

                                    
def main():
    name = sys.argv[1]
    out_name = name.split('.txt')[0] + '-d3c.txt'
    G = snap.LoadEdgeList(snap.PNGraph, name, 0, 1)
    EnumerateDir3Cycles(G, out_name)

if __name__ == '__main__':
    main()
