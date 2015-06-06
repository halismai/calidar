function x = rotate_points_mex(x, phi)
  % function x = rotate_points_mex(x, phi)
  %
  % Applies a rotation per point.
  %
  % INPUT
  %     x  Nx3 list of points
  %     phi N vector of rotation angles per point
  %
  % OUPUT
  %     x rotated points
  %
  % NOTE this is a fallback options if the mex file was not compiled
  % NOTE rotation direction is specific to the current sensor

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 05:21:33 PM EST
  %
  % License: See LICENSE file

  narginchk(2,2);
  assert(size(x,1) == length(phi), 'length mismatch');

  parfor i=1:size(x,1)
    c = cos(phi(i)); s = sin(phi(i));
    % apply rotation about the Z-axis
    x(i,:) = x(i,:) * [c -s 0; s c 0; 0 0 1];
  end

end
