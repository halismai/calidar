function out = reduce_data_fov(in, fov)
  drange = deg2rad([-fov/2 fov/2]);
  out = in(in(:,1) >= drange(1) & in(:,1) <= drange(2), :);
end
