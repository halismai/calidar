function p = h2p(H)

   a = quaternion(H(1:3,1:3));
   p = [a(2:end); H(2:3,end)];

end
