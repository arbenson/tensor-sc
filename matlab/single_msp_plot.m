function single_msp_plot(dl, lap, alap, msc, co_u, co_v, random, ...
    title_str, use_leg)
                     

inds = 1:size(dl, 2);

% filter data
min_tol = 1e-6;
msc(2, :) = max(msc(2, :), min_tol);
dl(2, :) = max(dl(2, :), min_tol);
lap(2, :) = max(lap(2, :), min_tol);
alap(2, :) = max(alap(2, :), min_tol);
random(2, :) = max(random(2, :), min_tol);
co_u(2, :) = max(co_u(2, :), min_tol);
co_v(2, :) = max(co_v(2, :), min_tol);
co = min(co_u, co_v);

width = 4; height = 3;

make_plot_pre(width, height);
start = 20;

if use_leg
start = length(inds);
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
    title(title_str, 'interpreter', 'none');
    xlabel('|S|', 'interpreter', 'none');
    ylabel('$\phi_3(S)$', 'interpreter', 'latex');
end
if use_leg
    ylim([0, 1]); xlim([0, 1]);
    legend('MSC', 'DL', 'UL', 'AL', 'Co', 'Random', ...
           'Location', 'North');
end
make_plot_post(width, height);

out_dir = '/Users/benson/Desktop/2014/austin-multilinear/FIG';
print(sprintf('%s/%s', out_dir, title_str), '-depsc2','-r300');
