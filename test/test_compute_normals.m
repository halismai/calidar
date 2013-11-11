[x,y] = meshgrid(-5:.01:5,-5:.01:5);
z = ones(size(x));

X(1,:) = x(:)';
X(2,:) = y(:)';
X(3,:) = z(:)';

fprintf('computing normals on %d points\n', size(X,2));
tic
[n,s] = compute_normals_mex(X, 20);
toc

fprintf('computing adaptive normals\n')
tic,
[nw,sw] = compute_adaptive_normals_mex(X, 20);
toc

%{
Xn = X + 0.1^2*randn(size(X));
tic
[n1,s1] = compute_normals_mex(Xn,20);
toc
%}
