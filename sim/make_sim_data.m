% 
% we will do 4 planes 
%
N = [0 0 1; ... % front
     0 1 0; ... % ground
     1 0 0; ... % left wall
    -1 0 0]; %  right wall
height = 5;
d = [5 5 5 5]; % distances for each plane from the origin


fov = 180.0; % degrees
theta_res = 1.0; % degrees
theta = deg2rad( -(fov/2):theta_res:(fov/2) );


r_d = [sin(theta); zeros(1,length(theta)); cos(theta)]; % direction
r_o = repmat([0;0;0],1,length(theta));  % origin

t0 = zeros(1,length(d));
for i=1:length(d)
  t0(i) = -(dot(r_o(:,1), N(i,:))+d(i));
end 

NN = cell(1, size(N,1)); % pre-allocate stuff for matlab's happiness
for i=1:size(N,1), NN{i}=repmat(N(i,:)', 1, length(theta)); end

% calibration offsets 
R_off = eye(3); % 
t_off = repmat([0 0 0]', 1, length(theta));

% motor rotations 
phi_res = 2; % degrees
phi = deg2rad( 0:phi_res:360 );

bounds = repmat([5 5 5]', 1, length(theta));

pts = [];
% run the simulation 
for i=1:length(phi)
  c = cos(phi(i)); s = sin(phi(i)); 
  R = [c -s 0; s c 0; 0 0 1]; % rotation about +Z;

  % direction of the rays
  v = R*(R_off*r_d + t_off);

  % compute intersections with the planes 
  %P = cell(1, length(NN));
  for j=1:length(NN)
    t = t0(j) ./ bsxfun(@dot, v, NN{j});
    p = r_o + bsxfun(@mtimes, t, v);
    ii = all(abs(p) < bounds);
    pts = [pts p(:,ii)];
  end 

end 


     
