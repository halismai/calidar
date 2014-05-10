function x = triangulate(data, H)
  % function x = triangulate(data)
  %
  % Applies in-plane triangulation for the lidar points.
  % Given polar coordinates, theta (the mirror angle) and the associate range,
  % the function converts the values into Euclidean/Cartesian
  %
  % INPUT
  %    data an Nx3 matrix obtaind from example_sensor.load
  %    H    current calibration [deafult = eye(4)]
  %
  % OUTPUT
  %    X: Nx3 set of points triangualated in the plane
  %
  % NOTE
  % For this example sensor
  %    * The coordinate system is right-handed with
  %       +Z forward
  %       +X to the right
  %       +Y downward
  %
  %    * The mirror spins counter-clockwise, with the origin at the center
  %    * Other sensors will have different mechansim, in particular the SICK's
  %    mirror rotation is opposite of the Hokuyo

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 05:30:08 PM EST
  %
  % License: See LICENSE file

  narginchk(1,2);

  t = data(:,1); % theta
  r = data(:,3); % range

  x = zeros(size(data,1), 3);
  %[x(:,3), x(:,1)] = pol2cart(t, r); % the y-axis is the rotation plane, value=0
  % this is identical to pol2car, but faster (avoids the function call), and
  % in case other do not have pol2cart for some reason
  x(:,1) = r.*sin(t);
  x(:,3) = r.*cos(t);


  if nargin > 1
    % apply the calibration
    % rotation followed by a translation
    x = bsxfun(@plus, x*H(1:3,1:3), H(1:3,end)');
  end

end  % triangulate

