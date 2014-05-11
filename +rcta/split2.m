function [s1, s2] = split2( data, min_range, max_range )

  [s1, s2] = rcta.split(data, min_range, max_range);

  s1 = s1(1:50:end,:);
  s2 = s2(2:50:end,:);
end
