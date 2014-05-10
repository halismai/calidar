function H = p2h(p)
   w = sqrt(1 - sum(p(1:3).^2));
   H = eye(4);
   H(1:3,1:3) = quaternion([w;p(1:3)]);
   H(2:3,end) = p(4:end);
end
