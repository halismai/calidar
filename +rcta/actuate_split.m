function [x1,x2] = actuate_split(s1,s2,H)

  if nargin < 3, H=eye(4); end

  x1 = rcta.triangulate(s1, H);
  x2 = rcta.triangulate(s2, H);

  x1 = rcta.rotate_points_mex(x1, s1(:,2), s1(:,end));
  x2 = rcta.rotate_points_mex(x2, s2(:,2), s2(:,end));


end

function x = rotate_points(x,a,b)

  % nodding, rotation about the x axis
  %y = zeros(size(x));

  c1 = cos(a); % nodding
  s1 = sin(a);

  c2 = cos(b); % spinning
  s2 = sin(b);

  for i=1:size(x,1)
    % nod first
    x(i,:) = x(i,:) * [1 0 0; 0 c1(i) -s1(i); 0 s1(i) c1(i)];
    % spin (y-axis)
    x(i,:) = x(i,:) * [c2(i) 0 s2(i); 0 1 0; -s2(i) 0 c2(i)];
  end
end
