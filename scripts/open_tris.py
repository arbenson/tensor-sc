import snap
import numpy as np
import sys

def EnumerateOpenTris(G, name):
    total = 0
    with open(name, 'w') as output:
        for u_ in G.Nodes():
            u = u_.GetId()
            for v in u_.GetInEdges():
                v_ = G.GetNI(v)
                for w in u_.GetOutEdges():
                    if v != w and not G.IsEdge(v, w) and not G.IsEdge(w, v):
                        output.write('%d %d %d\n' % (v, w, u))
                        total += 1

    print 'number of open triangles: ', total

   
def main():
    name = sys.argv[1]
    out_name = name.split('.txt')[0] + '-open-tris.txt'
    G = snap.LoadEdgeList(snap.PNGraph, name, 0, 1)
    EnumerateOpenTris(G, out_name)

if __name__ == '__main__':
    main()
