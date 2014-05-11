if ~exist('d1', 'var')
  d1=rcta.load('data/HC/h2_calibrated.csv');
end

min_range = 0.25;
max_range = 30.0;

[s1,s2] = rcta.split(d1,min_range,max_range);

H = eye(4);
H(1:3,end) = [0; 10; 10];%./100;


[x1,x2] = rcta.actuate_split(s1,s2, H);

r = [s1(:,4); s2(:,4)];
range = [s1(:,3);s2(:,3)];
c = reflectance2real(r, range, 200);
c = squeeze(real2rgb(c, jet(256)))';
c = uint8(255*c);

toply_mex('points4.ply', single([x1' x2']),c);

%[p1,c1,e1] = colorize_by_err(x1,x2);

%{
[ss1,ss2] = rcta.split(d2,min_range,max_range);
[y1,y2] = rcta.actuate_split(ss1,ss2);
[p2,c2,e2] = colorize_by_err(y1,y2);

toply_mex('no_cal.ply', single(p1)', uint8(255*c1)');
toply_mex('w_cal.ply',single(p2)', uint8(255*c2)');

toply_mex('before.ply',single([x1' x2']));
toply_mex('after.ply',single([y1' y2']));
%}


