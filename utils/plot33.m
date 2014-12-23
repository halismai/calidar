function h=plot33(X,clr,ms)
  % function h=plot33(X,clr,ms)
  % Plots the 3D data in X with proper acis scaling, and dots to represent the
  % points by default
  %
  % NOTE The data is in cols
  % INPUT
  %    X    3xN array of points. It can be more than 3 rows, the first 3 rows
  %    will be used for coordinates, the rest will be ignored.
  %    clr  Matlab style 'r.' or 's+'
  %    ms   The marker size to use (e.g. 2)
  %
  % OUTPUT
  %    h    handle to the figure
  %
  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Fri 15 Feb 2013 07:29:30 PM EST

  if nargin<3, ms=2; end % default marker size
  if nargin<2, clr='k.'; end % default style black dot

  % make sure we have 3D data
  assert( size(X,1)>= 3, 'Data must be (at least 3)xN');

  h=plot3(X(1,:),X(2,:),X(3,:),clr,'markersize',ms);

  % make the axis equal, tight and add a box in it
  axis equal; axis tight; box on;
  label; % put the default X,Y,Z label on the figure
  rotate3d on; % set it to rotate

end % plot33

