function H = p2h(p)
  % function H = p2h(p)
  %
  % Converts a vector of optimization parameters into a homogenous transform
  %
  % INPUT
  %     p   the vector of paramters
  %
  % OUTPUT
  %     H   the 4x4 Homogenous transform
  %
  %
  % NOTE: for this example, the sensor is spinning about the Z-axis. The length
  % of the parameters vector is 4, i.e. a 4 DOF model

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 07:08:52 PM EST
  %
  % License: See LICENSE file

  assert(isvector(p) && (length(p) == 5 || length(p)==4), ...
    'incorrect parameters vector length');
  if size(p,1) == 1, p = p'; end % make it a column vector if needed

  H = eye(4);

  if length(p) == 5
    % the first 3 elements of the vector a euler angles
    H(1:3,1:3) = euler2mat(p(1), p(2), p(3));
    % the translation parts are restricted to X and Y.
    % Translation long Z is underconstrained
    H(1:3,end) = [p(4:end); 0];
  else
    H(1:3,1:3) = euler2mat(0, p(1), p(2));
    H(1:3,end) = [p(3:end); 0];
  end

end  % p2h

