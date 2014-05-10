H_none = eye(4); % no calibration
H_cad  = eye(4); H_cad(3,end) = 7.5/100;
H_alg  = eye(4);
H_alg(1:3,1:3) = euler2mat(0.1022,-0.0463,0);
H_alg(1:3,end) = [0; 0.0028; 0.0728];

if ~exist('data', 'var')
  data = rcta.load('data/HC/h1_uncalibrated.csv');
end

n1 = get_plane_scores(data, H_none);
n2 = get_plane_scores(data, H_cad);
n3 = get_plane_scores(data, H_alg);

