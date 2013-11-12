function [dr,dt] = compute_delta_params(p1,p2)

  if length(p1) == 5
    dr = norm(p1(1:3) - p2(1:3));
    dt = norm(p1(4:end)-p2(4:end));
  else
    dr = norm(p1(1:2) - p2(1:2));
    dt = norm(p1(3:end) - p2(3:end));
  end 
end 
