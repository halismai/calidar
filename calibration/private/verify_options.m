function o = verify_options(o)
  % function o = verify_options(o)
  % 
  % Verifies options and sets in defaults 

  % Hatem Alismail <halismai@cs.cmu.edu> 
  % Last modified: Tue 12 Nov 2013 03:57:40 PM EST
  %
  % License: See LICENSE file

  % Absolute requirments 
  %check_struct_field(o, 'triangulation_func');
  check_struct_field(o, 'actuation_func');

  check_struct_field(o, 'p2h'); % parameters to calibration tform 
  check_struct_field(o, 'h2p'); % calibration tform to parameters
  check_struct_field(o, 'compute_delta_params'); % computes delta parameters

  % initial calibration, this is set to manadatory in case the actuatio
  % mechansim is not simple. For simple motions (single tranform) the identity
  % should work find (see example/run_example)
  check_struct_field(o, 'H_init');

  % optional stuff,they should work fine out of the box

  % maximum number of iterations for the optimizer 
  o = check_struct_field(o, 'max_iters', 50);
  % maximum outer iterations (see paper)
  o = check_struct_field(o, 'max_outer_iters', 50);
  % optimization options for Matlab's Optimization toolbox
  o = check_struct_field(o, 'optim_opts', get_default_optimopts(o.max_iters));
  % number of neighbors to guide the adpative normal extraction 
  o = check_struct_field(o, 'normals_k', 30);
  % maximum distance for a closest neighbor (squared meters)
  o = check_struct_field(o, 'max_neighbor_dist_sq', 0.5^2);

  % TODO gradient of the cost function 
  
  % convergence parameters 
  % these parameters will need to be around the accuracy of the returns
  % delta rotation in DEGREES 
  o = check_struct_field(o, 'R_delta_thresh', 0.1);
  % delta translation in METERS 
  o = check_struct_field(o, 't_delta_thresh', 0.5/100);

  % TODO display related parameters
  o = check_struct_field(o, 'do_show', false); % no display 
  o = check_struct_field(o, 'view_axis', [0 1 0]);
  o = check_struct_field(o, 'plot_hdle1', []);
  o = check_struct_field(o, 'plot_hdle2', []);

  % verbose 
  o = check_struct_field(o, 'verbose', true);

  if o.verbose == false 
    o.optim_opts.Display = 'none';
  end 


end  % verify_options


function o = get_default_optimopts(maxiter)
  %function o = get_default_optimopts
  % default optimization options for lsqnonlin

  o = optimset(optimset('lsqnonlin'), ...
    'display', 'iter', ...
    'algorithm', 'levenberg-marquardt', ...
    'maxiter', maxiter, 'largescale', 'on', 'useparallel', 'always');
end 

