function [scores,n] = get_plane_scores(data, H, k, min_range, max_range)
  if nargin < 5, max_range = 35.0; end
  if nargin < 4, min_range = 0.25; end
  if nargin < 3, k = 50; end

  [s1,s2] = rcta.split(data, min_range, max_range);
  [x1,x2] = rcta.actuate_split(s1,s2,H);

  [n,scores] = compute_adaptive_normals_mex([x1; x2]', k);

end
