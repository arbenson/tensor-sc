import matplotlib.pyplot as plt
import numpy as np
import sys

data = sys.argv[1]

def num_common(list1, list2):
    return len(list1) + len(list2) - len(np.unique(list1 + list2))

def best_num_common(order1, order2, num_nodes):
    order1_first = order1[0:num_nodes]
    order1_last = order1[-num_nodes:]
    order2_first = order2[0:num_nodes]
    order2_last = order2[-num_nodes:]
    best = max(num_common(order1_first, order2_first),
               num_common(order1_first, order2_last),
               num_common(order1_last, order2_first),
               num_common(order1_last, order2_last))
    return float(best) / num_nodes

with open('%s_msc_order.txt' % (data)) as f:
    multi = [int(line) for line in f]

with open('%s_dl_order.txt' % (data)) as f:
    dirlap = [int(line) for line in f]

with open('%s_lap_order.txt' % (data)) as f:
    lap = [int(line) for line in f]

def num_common_curve(order1, order2):
    total_nodes = len(multi)    
    sizes = range(1, total_nodes, total_nodes / 100)
    return sizes, [best_num_common(order1, order2, i) for i in sizes]

sizes1, common1 = num_common_curve(multi, dirlap)
sizes2, common2 = num_common_curve(multi, lap)
plt.plot(sizes1, common1, 'b*--',
         sizes2, common2, 'r*--')
plt.legend(['multi / dirlap',
            'dirlap / lap'])
plt.show()

