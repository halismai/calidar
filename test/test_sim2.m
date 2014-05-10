N = [0 0 1]';
d = 5;

w = 5;
h = 3;
b = [-w w w -w; h h -h h];

phi = deg2rad(0:1:360); % motor rotations
theta = deg2rad(-90:0.5:90); % mirror rotations per scan line
r_d = [sin(theta); zeros(1,length(theta)); cos(theta)]; % directions
r_o = repmat([0 0 0]',1,length(theta));  % origin of the sensor

t0 =  -(dot(r_o(:,1), N) + d);
NN = repmat(N, 1, length(theta));

% calibration offsets
t_x = 5/100;
t_y = 5/100;
t_z = 0;
t_off = repmat([t_y; t_y; t_z], 1, length(theta));

pts = [];
for i=1:length(phi)

  c = cos(phi(i));
  s = sin(phi(i));
  R = [c -s 0; s c 0; 0 0 1];

  v = R * (r_d + t_off);

  % compute the intersection
  t = t0 ./ bsxfun(@dot, v, NN);
  p = r_o + bsxfun(@mtimes, t, v);

  ii = find(abs(p(1,:)) <= w & abs(p(2,:)) <= h & p(3,:) < d);
  pts = [pts p(:,ii)];

end
