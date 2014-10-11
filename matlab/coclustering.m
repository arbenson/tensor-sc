function [evec1, evec2] = coclustering(data_name, out_name)

data = dlmread(data_name);
A = sparse(data(:, 1), data(:, 2), data(:, 3));

D_row = sum(A, 2);
D_col = sum(A, 1);

D_row_half_inv = sparse(1:size(A, 1), 1:size(A, 1), 1 ./ sqrt(D_row));
D_col_half_inv = sparse(1:size(A, 2), 1:size(A, 2), 1 ./ sqrt(D_col));

B = D_row_half_inv * A * D_col_half_inv;

OPTIONS = {};
OPTIONS.tol = 1e-6;
OPTIONS.maxit = 250;
OPTIONS.disp = 1;
[U,~,V] = svds(B, 2, 'L', OPTIONS);
evec1 = D_row_half_inv * U(:, 2);
evec2 = D_col_half_inv * V(:, 2);

dlmwrite(sprintf('%s_u', out_name), evec1);
dlmwrite(sprintf('%s_v', out_name), evec2);

