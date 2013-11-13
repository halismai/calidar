function save_before_after(prefix, dd, min_range, max_range, H)

  [s1, s2] = example_sensor.split(dd, min_range, max_range);

  ref_vals = [s1(:,end); s2(:, end)];
  range_vals = [s1(:,3); s2(:,3)];

  clr = squeeze(real2rgb(reflectance2real(ref_vals, range_vals), gray(1024)))';
  clr = uint8(255*clr);

  save_pts(sprintf('%s_before',prefix),s1,s2,clr,eye(4),false);
  save_pts(sprintf('%s_after',prefix),s1,s2,clr,H, true);

  
end  % save_before_after

function save_pts(prefix, s1, s2, cc, H, is_after)

  [x1, x2] = example_sensor.actuate_split(s1, s2, H);

  if ~is_after
    c1 = cc(:,1:size(x1,1));
    c2 = cc(:,(size(x1,1)+1):end);
    save_pts_helper(sprintf('%s_s1.ply', prefix), x1', c1);
    save_pts_helper(sprintf('%s_s2.ply', prefix), x2', c2);
  else 
    p = single([x1' x2']);
    save_pts_helper(sprintf('%s_result.ply', prefix), p, cc);
  end 

end 


function save_pts_helper(fn, x, c)

  x = single(x);
  n = compute_adaptive_normals_mex(x, 32);
  toply_mex(fn, x, c, n);

end 
