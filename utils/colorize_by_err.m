function [x,c,err] = colorize_by_err(x1,x2,k)
% function [x,c] = colorize_by_err(x1,x2,k)
  %
  % Colorizes points by their error point-plane distance
  %

  if nargin < 3, k=50; end

  % compute the normals
  [n1,n_scores] = compute_adaptive_normals_mex(x1', k);

  [i1,i2] = find_correspondences(x1, x2);

  err = bsxfun(@dot, (x1(i1,:)-x2(i2,:))', n1(:,i1)).^2;

  i_good = err < 2*mean(err);

  err = err(i_good);

  c = (err - min(err)) ./ (max(err)-min(err));
  %c = log(c + 1.0);
  c = squeeze(real2rgb(c,pmkmp(64,'CubicL')));
  %c = squeeze(real2rgb(c,lbmap(16,'RedBlue')));
  %c = squeeze(real2rgb(c,jet));
  x = x1(i1(i_good),:);
end


function [i1,i2] = find_correspondences(x1,x2)
   tree = KdTree(x1');
   [i1,d] = tree.knnsearch(x2',1);
   [i1,i2] = keep_unique_corrs(i1,d);
 end

function [i1,i2,d] = keep_unique_corrs(i1,d)
  i2 = 1:length(i1);
  [d,is] = sort(d);
  i1 = i1(is); i2 = i2(is);

  [i1,is] = unique(i1); i2 = i2(is); d = d(is);
end

