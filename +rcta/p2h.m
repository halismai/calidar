function H = p2h(p)
  % function H = p2h(p)
  %
  % parameter vector to Homogenous transformation
  %
  % For a nodder we have 2 translations (t_y & t_z)
  % with 2 dof rotation (r_y,r_z)
  %
  % This assumes the coordinate system with Y forward and Z up
  %

  H = eye(4);
  if length(p) > 2
    H(1:3,1:3) = euler2mat(p(1),p(2),0.0);
    H(2:3,end) = p(3:end);
  else
    H(2:3,end) = p;
  end

end
