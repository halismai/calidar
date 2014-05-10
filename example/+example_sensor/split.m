function [s1,s2] = split(data, min_range, max_range)
  % function [s1,s2] = split(data, min_range, max_range)
  %
  % Splits the lidar data into two overlapping point sets depdending on the
  % actuation angle
  %
  % INPUT
  %
  %    data  ,    Nx3 raw data from example_sensor.load()
  %    min_range  minimum range to triangulate [default = 0.30 m]
  %    max_range  maximum range to triangulate [default = 30.0 m]
  %
  % OUTPUT
  %    s1,s2      structure containing data for each half scan. This is the raw
  %               data splin into two.
  %
  % NOTE
  %   For this example sensor, splitting is based on the spinning motor angle

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Mon 11 Nov 2013 05:17:19 PM EST
  %
  % License: See LICENSE file

  narginchk(1,3);
  if nargin < 2, min_range = 0.30; end
  if nargin < 3, max_range = 30.0; end

  phi = abs(data(:,2)); % so we don't care about motor spinning direction
  ivalid = data(:,3) > min_range & data(:,3) < max_range;
  s1 = data(ivalid & phi<pi, :);
  s2 = data(ivalid & phi>=pi,:);

end % split

