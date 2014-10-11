
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
    for cut_type in ['msc', 'dl', 'lap', 'alap', 'cocluster_u', 'cocluster_v', 'random']:
        key = '%s-filter_%s_%s' % (data, cut_type, suffix)
        with open(key + '.txt') as f:
            num_cut_sweep = [float(line) for line in f]
            # Format for Matlab
            key = key.split('/')[-1]
            key = key.replace('-', '_')
            all_data[key] = get_best(num_cut_sweep)

for data_set in ['soc-Slashdot0811', 'wiki-Vote', 'as-caida20071105',
                 'email-Enron', 'soc-Epinions1', 'amazon0312',
                 'twitter_combined', 'email-EuAll', 'cit-HepPh',
                 'web-Stanford', 'wiki-RfA-net', 'wiki-Talk']:
    gather_data('d3c_cond_results/' + data_set, 'd3c_cond')
    gather_data('num_cut_results/' + data_set, 'num_cut')
    gather_data('density_results/' + data_set, 'density')
    sio.savemat('cut_data.mat', all_data)
