function label(xl,yl,zl)
  % function label
  % simple function to put labels on the figure
  %
  % INPUT [all optional]
  %    xl text for x-axis
  %    yl ditto y
  %    zl ditto z
  %
  % See also: PLOT33 PLOT22

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Fri 15 Feb 2013 03:53:56 PM EST

  if nargin < 3, zl = 'Z'; end
  if nargin < 2, yl = 'Y'; end
  if nargin < 1, xl = 'X'; end

  xlabel(xl);
  ylabel(yl);

  % if we were provided with two inputs then it is most probably a call to
  % label a 2D figure, there is no need to get anything on the 3rd dimension

  if nargin ~= 2
    zlabel(zl);
  end

end % label

