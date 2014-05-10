function angles = mat2euler(R)
% MAT2EULER - Finds Euler angles from a 3x3
%   rotation matrix
%
% OUTPUTS:
%   alpha - rotation angle around Z axis
%   beta  - rotation angle around Y axis
%   gamma - rotation angle around X axis
%

eta = sqrt(R(1,1)^2+R(2,1)^2);
angles(1) = asin(R(2,1)/eta)*180/pi;
angles(2) = asin(-R(3,1))*180/pi;
angles(3) = asin(R(3,2)/eta)*180/pi;
