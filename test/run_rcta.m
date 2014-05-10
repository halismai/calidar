do_show = true;

H_init = eye(4);
H_init(3,end) = 7.5/100;
H_init = eye(4);
Lb=[];
Ub=[];

opts = struct('actuation_func', @rcta.actuate_split,...
    'p2h',@rcta.p2h, 'h2p',@rcta.h2p, ...
    'compute_delta_params',@rcta.compute_delta_params,...
    'H_init', H_init,...
    'do_show', do_show, 'max_outer_iters', 50, ...
  'lower_bound', Lb, 'upper_bound', Ub);


if ~exist('data','var')
   data=rcta.load('data/HC/h1_uncalibrated.csv');
end

min_range = 0.25;
max_range = 30.0;

[s1,s2] = rcta.split(data,min_range,max_range);

[H,C] = calibrate_lidar(s1(1:5:end,:),s2(1:5:end,:),opts);

