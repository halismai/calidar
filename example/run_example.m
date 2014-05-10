opts = struct('actuation_func', @example_sensor.actuate_split, ...
  'p2h', @example_model.p2h, ...
  'h2p', @example_model.h2p, ...
  'compute_delta_params', @example_model.compute_delta_params, ...
  'H_init', eye(4), ...
  'do_show', true, ...
  'max_outer_iters', 10);

if ~exist('data', 'var')
  % loading this is slow, so we do it once
  data = example_sensor.load('data/spinning_data_example.csv');
end

min_range = 0.3;
max_range = 8.0;
% split the scan data based on the actuation mechansim
[s1,s2] = example_sensor.split(data, min_range, max_range);


[H,C] = calibrate_lidar(s1, s2, opts);
