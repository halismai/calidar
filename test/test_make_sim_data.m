H = eye(4);
H(1:3,end) = [2 -2 3]' / 100;

p1 = make_sim_data_mex(eye(4));
p2 = make_sim_data_mex(H);
