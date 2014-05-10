function [x1,x2] = actuate_split(s1,s2,H)
  % function [x1,x2] = actuate_split(s1,s2,H)
  %
  % Applies the motor actuation to the half scans. Here you include your
  % kinematic chain.
  %
  % INPUT
  %   s1,s2   the first and second half-scans, from example_sensor.split
  %   H       the calibration [default = eye(4)]
  %
  % OUTPUT
  %   x1,x2   N1x3 and N2x3 points with current calibration applied

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 05:17:28 PM EST
  %
  % License: See LICENSE file

  narginchk(2,3);
  if nargin < 3, H = eye(4); end

  x1 = example_sensor.triangulate(s1, H);
  x2 = example_sensor.triangulate(s2, H);


  % apply the motor angle per points
  % this is a slow loop per point (compile the mex file for a faster result)
  x1 = example_sensor.rotate_points_mex(x1, s1(:,2));
  x2 = example_sensor.rotate_points_mex(x2, s2(:,2));

end  % actuate_split

