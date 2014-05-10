function [s1,s2] = split(data,min_range,max_range)
  % function [s1,s2] = split(data,min_range,max_range)

  if nargin < 3, max_range = 25.0; end
  if nargin < 2, min_range = 0.25; end

  ivalid = data(:,3) > min_range & data(:,3) < max_range;
  d = [0;diff(data(:,2))];
  s1 = data(ivalid & d<0, :);
  s2 = data(ivalid & d>0, :);

end
