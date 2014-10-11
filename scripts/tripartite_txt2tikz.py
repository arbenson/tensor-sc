import sys

data_file = sys.argv[1]

def thick_line(i, j):
  return '\\path[draw, ultra thick] (%s) edge [->] (%s);' % (i, j)    

def thin_line(i, j):
  return '\\path[draw, dashed] (%s) edge [->] (%s);' % (i, j)    

with open(data_file, 'r') as f:
    for line in f:
        if line[0] == '#':
            continue

        i, j = [int(x) for x in line.split()]
        if (i in range(0, 4) and j in range(4, 7) or \
            i in range(4, 7) and j in range(7, 9) or \
            i in range(7, 9) and j in range(0, 4)):
            print thick_line(i, j)
        else:
            print thin_line(i, j)



