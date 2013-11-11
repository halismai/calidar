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
  %     for this example sensor, we have a 5DOF kinematic model. Three
  %     rotational DOF and two for the translation along the Y and X axes.
  %     Translation along the Z axis is underconstrained as it is about the
  %     motor's rotation axis 
  
  % Hatem Alismail <halismai@cs.cmu.edu> 
  % Last modified: Mon 11 Nov 2013 05:15:45 PM EST
  %
  % License: See LICENSE file
  
  assert(isequal([4 4], size(H)), 'H must a 4x4 homogenous transform');

  p = zeros(5,1); 
  p(1:3) = mat2euler(H(1:3,1:3));
  p(4:end) = [H(1,end); H(2,end)]; % x,y translation values

end % h2p
