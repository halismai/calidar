% 
N = 100;
X = rand(3, N);

fprintf('building the tree\n');
tic, tree  = KdTree(X,4); toc

%{
fprintf('searching\n');
for k=1:1
  fprintf(' k %d\n', k);
  tic, [ii,dd] = tree.knnsearch(X, 1); toc
end 
%}

fprintf('rangesearch\n');
tic
[ii,dd] = tree.rangesearch(X, .5^2);
toc

