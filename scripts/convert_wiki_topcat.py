import numpy as np
import scipy.io as sio
import scipy.sparse

mat = sio.loadmat('wiki.mat')['H']
I, J, _ = scipy.sparse.find(mat)

with open('wiki_topcat.txt', 'w') as f:
    for i, j in zip(I, J):
        f.write('%d %d\n' % (i, j))

