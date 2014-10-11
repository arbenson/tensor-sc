function [score] = directionality(data_name)

data = dlmread(data_name);
fprintf('computing score for %s...\n', data_name);
A = sparse(data(:, 1), data(:, 2), data(:, 3));

D = sum(A, 2);
D_inv = sparse(1:size(A, 1), 1:size(A, 1), 1 ./ D);
P = D_inv * A;

B = P';
[V, ~, ~] = eigs(B, 1);
v = V(:, 1);

V_half = sparse(1:size(A, 1), 1:size(A, 1), sqrt(v));
V_half_inv = sparse(1:size(A, 1), 1:size(A, 1), 1 ./ sqrt(v));

P_circ = V_half * (P * V_half_inv);

Z = (P_circ - P_circ') / 2;

[~,S,~] = svds(Z, 1);
score = S(1);
