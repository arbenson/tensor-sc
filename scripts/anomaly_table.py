def get_data(data_file):
    with open(data_file) as f:
        return [int(x) for x in f]

data1 = get_data('anomaly_tsc_order.txt')
data2 = get_data('anomaly_d3c-only-noback-dirlap_order.txt')
data3 = get_data('anomaly_dl_order.txt')

data = [(data1[i], data2[i], data3[i]) for i in range(len(data1))]
set1 = range(0, 5)

def format_index(val):
    if val in set1:
        return '\\textcolor{red}{%d}' % val
    return str(val)

for x, y, z in data:
    print '%s & %s & %s \\\\' % (format_index(x),
                                 format_index(y),
                                 format_index(z))
