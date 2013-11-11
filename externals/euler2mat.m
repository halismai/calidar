function R = euler2mat(alpha, beta, gamma)
% EULER2MAT - Converts an Euler representation 
%   of a 3D rotation to a 3x3 orthogonal matrix
%   The rotation around X axis is applied first
%   followed by the rotation around Y axis, and
%   the rotation around Z axis is applied last.
%   
% INPUTS:
%   alpha - rotation angle around Z axis in degrees
%   beta  - rotation angle around Y axis in degrees
%   gamma - rotation angle around X axis in degrees
%


ca = cos(alpha*pi/180); sa = sin(alpha*pi/180);
cb = cos(beta*pi/180); sb = sin(beta*pi/180);
cg = cos(gamma*pi/180); sg = sin(gamma*pi/180);

R = [ca*cb, ca*sb*sg-sa*cg, ca*sb*cg+sa*sg; ...
     sa*cb, sa*sb*sg+ca*cg, sa*sb*cg-ca*sg; ...
     -sb,   cb*sg,          cb*cg];
