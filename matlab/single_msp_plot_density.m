function single_msp_plot_density(dl, lap, alap, msc, co_u, co_v, random, ...
    title_str, use_leg)
                     

inds = 1:size(dl, 2);

% filter data
min_tol = 1e-6;
msc(2, :) = min(max(msc(2, :), min_tol), 1);
dl(2, :) = min(max(dl(2, :), min_tol), 1);
lap(2, :) = min(max(lap(2, :), min_tol), 1);
alap(2, :) = min(max(alap(2, :), min_tol), 1);
random(2, :) = min(max(random(2, :), min_tol), 1);
co_u(2, :) = min(max(co_u(2, :), min_tol), 1);
co_v(2, :) = min(max(co_v(2, :), min_tol), 1);
co = max(co_u, co_v);

width = 4; height = 3;

make_plot_pre(width, height);
start = 20;

if use_leg
%start = length(inds);
end

loglog(inds(start:end), msc(2, start:end), 'r-', 'LineWidth', 4);
hold on;
loglog(inds(start:end), dl(2, start:end), 'g-', ...
       inds(start:end), lap(2, start:end), 'b--', ...
       inds(start:end), alap(2, start:end), 'm:', ...
       inds(start:end), co(2, start:end), 'c-.', ...
       'LineWidth', 1);
hold on;
loglog(inds(start:end), random(2, start:end), 'k-', 'LineWidth', 2);

if ~use_leg
%if use_leg
    title(title_str, 'interpreter', 'none');
    xlabel('|S|', 'interpreter', 'none');
    ylabel('density', 'interpreter', 'none');
end
if use_leg
    ylim([0, 1]); xlim([0, 1]);
    loc = 'North'
    %loc = 'NorthEast';
    legend('TSC', 'DL', 'UL', 'AL', 'Co', 'Random', ...
           'Location', loc);
end
make_plot_post(width, height);

out_dir = '/Users/benson/Desktop/2014/austin-multilinear/FIG';
%print(sprintf('%s/%s_density_leg', out_dir, title_str), '-depsc2','-r300');
print(sprintf('%s/%s_density', out_dir, title_str), '-depsc2','-r300');
