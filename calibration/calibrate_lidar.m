function [H,C,it] = calibrate_lidar(s1,s2, opts)
  % function [H,C,it] = calibrate_lidar(s1,s2, options)
  %
  % Runs the lidar internal calibration algorithm
  %
  % INPUT
  %    s1,s2      the two half-scan splits
  %    options    struct with calibration options and function handles
  %
  % OUTPUT
  %    H          the calibration
  %    C          Covariance of the estimated parameters
  %
  % NOTE the output calibration does not have to be single matrix, it could be a
  %  struct with more than a single transformation. See examples/

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modifiedx: Mon 11 Nov 2013 06:22:41 PM EST
  %
  % License: See LICENSE file

  narginchk(3,3);
  opts = verify_options(opts);
  H = opts.H_init;

  opts = setup_display(opts, s1, s2);
  p_prev = opts.h2p(H); % for convergence testing

  if opts.verbose
    printf_fmt = repmat('% 0.3f ', 1, length(p_prev));
  end


  for it=1:opts.max_outer_iters
    [H,C] = run_optimization(H, s1, s2, opts);

    p_now = opts.h2p(H);
    [dr,dt] = opts.compute_delta_params(p_prev, p_now);

    if opts.verbose
      fprintf('Iteration %d/%d\n', it, opts.max_outer_iters);
      fprintf('  delta rotation    %0.3e degrees\n', dr);
      fprintf('  delta translation %0.3e cm\n', 100*dt);
      fprintf(['  prev_params ' printf_fmt '\n'], p_prev);
      fprintf(['       params ' printf_fmt '\n'], p_now);
    end

    if dr < opts.R_delta_thresh && dt < opts.t_delta_thresh
      if opts.verbose
        fprintf('Converged in %d iters. Change in parameters is too small! [%f %f]\n',...
          it,dr,dt);
      end
      break;
    end

    p_prev = p_now;
  end  % iters

  % do not rescale anything
  % rescaling the covariance to unit determinant
  %C = C ./ (abs(det(C)).^(1/size(C,1)));

end  % calibrate_lidar


function [H,C] = run_optimization(H, s1, s2, opts)
  % function H = run_optimization(H, s1, s2, opts)
  %
  % Runs the optimization with the current estimate of the calibration 'H'
  %
  % INPUT
  %   H       current estimate of calibratrion parameters
  %   s1      the first half-scan
  %   s2      the second half-scan
  %   opts    algorithm options
  % OUTPUT
  %

  % compute normals and correspondences indices
  [x1,x2]       = opts.actuation_func(s1,s2,H);
  fprintf('calling compute_adaptive_normals_mex with %d\n',opts.normals_k);
  [n1,n_scores] = compute_adaptive_normals_mex(x1', opts.normals_k);
  [i1,i2]       = find_correspondences(x1,x2, ...
    opts.max_neighbor_dist_sq,opts.use_unique_corrs);

  function e = error_fn(params)
    [x1,x2] = opts.actuation_func(s1, s2, opts.p2h(params));

    % error on correspondences only, point-to-plane
    % TODO add the gradient

    %e = n_scores(i1).*bsxfun(@dot, (x1(i1,:)-x2(i2,:))', n1(:, i1));% / length(i1);
    e = bsxfun(@dot, (x1(i1,:)-x2(i2,:))', n1(:, i1)) / sqrt(length(i1));
  end

  y1=[]; y2=[];
  function s = out_fn(pp, varargin)
    s = false;
    [y1,y2] = opts.actuation_func(s1, s2, opts.p2h(pp));
    set(opts.plot_hdle1,'xdata',y1(i1,1),'ydata',y1(i1,2),'zdata',y1(i1,3));
    set(opts.plot_hdle2,'xdata',y2(i2,1),'ydata',y2(i2,2),'zdata',y2(i2,3));
    drawnow;
  end

  if ~isempty(opts.plot_hdle1) && ~isempty(opts.plot_hdle2)
    opts.optim_opts.OutputFcn = @out_fn;
  end

  %opts.lower_bound
  %opts.upper_bound

  [p,~,~,~,~,~,J] = lsqnonlin(@error_fn, opts.h2p(H), ...
    opts.lower_bound, opts.upper_bound, opts.optim_opts);
  H = opts.p2h(p);

  C = inv(J'*J);

end % run_optimization


function [i1,i2] = find_correspondences(x1, x2, max_d_sq, use_unique_corrs)
  % function [i1,i2] = find_correspondences(x1, x2, max_d_sq, use_unique_corrs)

  tree = KdTree(x1');
  [i1, dists] = tree.knnsearch(x2', 1);

  if use_unique_corrs
    [i1, i2, dists] = keep_unique_corrs(i1, dists);
    ibad = dists > max_d_sq;
    i1(ibad) = [];
    i2(ibad) = [];
  else
    i2 = 1:size(x2,1);
  end

  assert(length(i1)>10,'Not enough correspondences, increase max_neighbor_dist_sq');
end  % find_correspondences

function [i1,i2,d] = keep_unique_corrs(i1,d)
  i2 = 1:length(i1);
  [d,is] = sort(d);
  i1 = i1(is); i2 = i2(is);

  [i1,is] = unique(i1); i2 = i2(is); d = d(is);
end


function opts = setup_display(opts, s1, s2)
  if opts.do_show
    [x1,x2] = opts.actuation_func(s1,s2,opts.H_init);
    hold off;
    opts.plot_hdle1 = plot33(x1','.',8); hold on;
    opts.plot_hdle2 = plot33(x2','.',8);
    set(opts.plot_hdle1, 'color', [255 180 0]/255);
    set(opts.plot_hdle2, 'color', [0 80 255]/255);
    %view(opts.view_axis);
    view([0 0 1])
    drawnow;
  end
end
