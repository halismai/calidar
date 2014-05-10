function x = triangulate(data, H)

  t = data(:,1); % theta
  r = data(:,3); % range

  x = zeros(size(data,1),3);
  x(:,1)=r.*cos(t);
  x(:,2)=r.*sin(t);

  if nargin > 1
    x = bsxfun(@plus, x*H(1:3,1:3), H(1:3,end)');
  end

end
