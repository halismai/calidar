N = [0 0 1]';
d = 5; % distance from origin
w = 5; % width
h = 3; % height
% bounding box
b = [-w w w -w; ...
     h  h -h h];


% lidar theta 
theta = deg2rad( -135:0.25:135 );

% direction of the ray 
r_d = [sin(theta); zeros(1,length(theta)); cos(theta)];

% origin
r_o = [0;0;0];

% intersection 
t = -(dot(r_o,N) + d) ./ bsxfun(@dot, r_d, repmat(N,1,length(theta)));
p = repmat(r_o,1,length(theta)) + bsxfun(@mtimes, t, r_d);

ii = find(p(1,:) < w & p(1,:) > -w & ...
          p(2,:) < h & p(2,:) > -h & ...
          p(3,:) < d);



