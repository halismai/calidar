function make_cuboid(cntr, dims)

  [v f] = createCube;
  v = bsxfun(@minus, v, mean(v,1));

  Ts = diag([dims(1) dims(2) dims(3) 0])
  Tt = eye(4); Tt(1:3,end) = cntr;

  [x y z] = transformPoint3d(v, Tt*Ts)

end 

