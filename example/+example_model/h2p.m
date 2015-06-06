function p = h2p(H)
  % function p = h2p(H)
  %
  %  Converts a Homogenous 3D transform (4x4 matrix) into a vector of
  %  optimization parameters
  %
  %  INPUT
  %    H    4x4 Homogenous transform matrix
  %
  %  OUPUT
  %    p    column vector of optimization parameters
  %
  % NOTE
  %     For this example sensor, we have a 5DOF kinematic model. Three
  %     rotational DOF and two for the translation along the Y- and X- axes.
  %     Translation along the Z-axis is underconstrained as it is the motor's
  %     rotation axis

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 08:20:12 PM EST
  %
  % License: See LICENSE file

  assert(isequal([4 4], size(H)), 'H must a 4x4 homogenous transform');

  ndof = 4;
  p = zeros(ndof,1);

  if ndof == 5
    p(1:3) = mat2euler(H(1:3,1:3));
    p(4:end) = [H(1,end); H(2,end)]; % x,y translation values
  else
    a = mat2euler(H(1:3,1:3));
    p(1:2) = a(2:3); % Y and X rot, no Z
    p(3:end) = [H(1,end); H(2,end)];
  end

end % h2p
