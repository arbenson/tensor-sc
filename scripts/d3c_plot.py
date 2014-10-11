import matplotlib as mpl
import matplotlib.pyplot as plt
import sys

data = sys.argv[1]
suffix = sys.argv[2]
if len(sys.argv) > 3:
    have_leg = int(sys.argv[3])
else:
    have_leg = True

def get_best(num_cut_sweep):
    best = {}
    total = len(num_cut_sweep)
    for i, num_cut in enumerate(num_cut_sweep):
        min_size = min(i + 1, total - (i + 1))
        if min_size in best:
            if suffix == 'density':
                best[min_size] = max(num_cut, best[min_size])
            else:
                best[min_size] = min(num_cut, best[min_size])
        else:
            best[min_size] = num_cut

    inds, data = zip(*sorted(best.items()))
    inds = inds[30:]
    data = data[30:]
    return inds, data


def get_data(alg):
    with open('%s_%s_%s.txt' % (data, alg, suffix)) as f:
        if suffix == 'num_cut':
            all_data = [float(line) for line in f]
        elif suffix == 'd3c_cond2':
            all_data = [min(max(float(line), 1e-4), 1) for line in f]
        elif suffix == 'd3c_cond':
            all_data = [max(float(line), 1e-4) for line in f]
        elif suffix == 'density':
            all_data = [min(max(float(line), 1e-4), 1) for line in f]
        elif suffix == 'touch':
            all_data = [min(max(float(line), 1e-4), 2) for line in f]
        else:
            all_data = [float(line) for line in f]
    return get_best(all_data)


multi_inds, multi_vals = get_data('msc')
dirlap_inds, dirlap_vals = get_data('dl')
lap_inds, lap_vals = get_data('lap')
alap_inds, alap_vals = get_data('alap')
co_u_inds, co_u_vals = get_data('cocluster_u')
co_v_inds, co_v_vals = get_data('cocluster_v')
random_inds, random_vals = get_data('random')


fig = plt.figure()
font = {'family' : 'normal',
        'weight' : 'bold',
        'size'   : 18}
mpl.rc('font', **font)
mpl.rcParams['lines.linewidth'] = 4

plt.loglog(multi_inds, multi_vals, 'r--',
           dirlap_inds, dirlap_vals, 'g--',
           lap_inds, lap_vals, 'y--',
           alap_inds, alap_vals, 'k--',
           co_u_inds, co_u_vals, 'm--',
           co_v_inds, co_v_vals, 'c--',
           random_inds, random_vals, 'b--')


plt.ylabel(suffix)
plt.xlabel('min($|S|$, $|\\bar{S}|$)')
#plt.legend(['multi-linear', 'dirlap', 'lap', 'alap', 'co-u', 'co-v', 'random'],
#           loc='upper right')

plt.title(data.split('/')[-1])
plt.show()
F = plt.gcf()
F.set_size_inches((2, 2))
fig.savefig('%s.eps' % data)
