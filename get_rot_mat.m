function R = get_rot_mat(theta,axs)
  % R=get_rot_mat(theta,axs) 
  %
  % returns a rotation matrix of 'theta' degrees 
  % about 'axs' 
  %
  % axs could be 'X', 'Y', 'Z' or any random unit vector
  % if axs is a vector but not unit, it'll be normalized to unit length
  % 
  % Hatem Alismail <halismai@cs.cmu.edu> 
  % Last modified: Fri 15 Feb 2013 03:21:21 PM EST

  check_soln = 1;

  if nargin < 2 
    axs = 'x';
  end

  t = theta * pi / 180; 
  c = cos(t); s = sin(t); 

  if strcmp(class(axs),'char')

    switch lower(axs)
      case 'x' % Y
        R = [1 0 0; 0 c -s; 0 s c];
      case 'y'
        R = [c 0 s; 0 1 0; -s 0 c];
      case 'z'
        R = [ c -s 0; s c 0; 0 0 1];
      otherwise
        error(['Unknown rotation axis ' axs]); 
    end

  else 

    % make sure it is a unit vector 
    if norm(axs) ~= 1
      %disp('normalizing to unit vector');
      axs = axs ./ sqrt(sum(axs.^2));
    end  
    u = axs;
    sum(u.^2);
    ux = axs(1); uy = axs(2); uz = axs(3); 

    R = [ux^2 + (1-ux^2)*c, ux*uy*(1-c)-u(3)*s ux*uz*(1-c)+uy*s;
    ux*uy*(1-c) + uz*s, uy^2+(1-uy^2)*c,uy*uz*(1-c)-ux*s;
    ux*uz*(1-c) - uy*s, uy*uz*(1-c)+ux*s, uz^2+(1-uz^2)*c];
  end 

  if check_soln 
    n = R*R' - eye(3); 
    e = ones(3)*1e-12; 
    if any(n>e) 
      error('bad matrix');
    end
  end

end % get_rot_mat

