import sys

data_file = sys.argv[1]

def thick_line(i, j):
  return '\\path[draw, ultra thick] (%s) edge [->] (%s);' % (i, j)    

def thin_line(i, j):
  return '\\path[draw, dashed] (%s) edge [->] (%s);' % (i, j)    

set1 = range(0, 2)
set2 = range(2, 4)
set3 = range(4, 6)

with open(data_file, 'r') as f:
    for line in f:
        if line[0] == '#':
            continue

        i, j = [int(x) for x in line.split()]
        if (i in set1 and j in set2 or \
            i in set2 and j in set3 or \
            i in set3 and j in set1):
            print thick_line(i, j)
        else:
            print thin_line(i, j)



