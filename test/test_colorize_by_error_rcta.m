H_none = eye(4); % no calibration
H_cad  = eye(4); H_cad(3,end) = 7.5/100;
H_alg  = eye(4);
H_alg(1:3,1:3) = euler2mat(0.1022,-0.0463,0);
H_alg(1:3,end) = [0; 0.0028; 0.0728];


if ~exist('data', 'var')
  data = rcta.load('data/HC/h1_uncalibrated.csv');
end


min_range = 0.1;
max_range = 40.0;

[s1,s2] = rcta.split(data,min_range,max_range);
%{
[x1,x2] = rcta.actuate_split(s1,s2,H_none);
[X,C,err] = colorize_by_err(x1,x2);
toply_mex('H_none.ply', single(X'), uint8(255*C'));
%}


[x1,x2] = rcta.actuate_split(s1,s2,H_alg);
c1 = real2rgb(reflectance2real(s1(:,4),s1(:,3),100), bone);
c2 = real2rgb(reflectance2real(s2(:,4),s2(:,3),100), bone);
c1 = squeeze(c1);
c2 = squeeze(c2);

toply_mex('colorized.ply', single([x1;x2]'), uint8(255*[c1;c2]'));







