import matplotlib as mpl
import matplotlib.pyplot as plt
import scipy.io as sio
import sys

'''
Create a .mat file of all the data so that we can plot things in Matlab.
'''

def get_best(num_cut_sweep):
    best = {}
    total = len(num_cut_sweep)
    for i, num_cut in enumerate(num_cut_sweep):
        min_size = min(i + 1, total - (i + 1))
        if min_size in best:
            best[min_size] = min(num_cut, best[min_size])
        else:
            best[min_size] = num_cut

    return zip(*sorted(best.items()))


all_data = {}
def gather_data(data, suffix):
    for cut_type in ['multi', 'dirlap', 'lap', 'random', 'd3c-only-dirlap']:
        key = '%s_%s_%s' % (data, cut_type, suffix)
        with open(key + '.txt') as f:
            num_cut_sweep = [float(line) for line in f]
            # Format for Matlab
            key = key.split('/')[-1]
            key = key.replace('-', '_')
            all_data[key] = get_best(num_cut_sweep)

for data_set in ['Slashdot0811', 'Wiki-Vote', 'as-caida20071105',
                 'email-Enron', 'soc-Epinions1', 'amazon0302',
                 'twitter_combined', 'Cit-HepPh', 'web-Stanford']:
    gather_data('results/' + data_set, 'd3c_cond')
    sio.savemat('d3c_cond.mat', all_data)
