function write_before_after_simple(prefix, s1, s2, H)
  % function write_before_after_simple(prefix, s1, s2, H)

  ref_v =   [s1(:,4)' s2(:,4)'];
  range_v = [s1(:,3)' s2(:,3)'];
  clr = real2rgb(reflectance2real(ref_v, range_v), bone(1024));
  clr = uint8(255*squeeze(clr)');

  [x1, x2] = example_sensor.actuate_split(s1, s2, eye(4));
  toply_mex(sprintf('%s_before.ply', prefix), single([x1' x2']), clr);

  [x1, x2] = example_sensor.actuate_split(s1, s2, H);
  toply_mex(sprintf('%s_after.ply', prefix), single([x1' x2']), clr);

end
