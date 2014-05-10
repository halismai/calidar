function p = h2p(H)
  % function p = h2p(H)
  %
  % Homogenous transformation to vector of parameters
  %
  %a = mat2euler(H);
  %p = [a(1); a(2); H(2:3,end)];
  p = H(2:3,end);
end
