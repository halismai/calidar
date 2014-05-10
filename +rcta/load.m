function data = load(fn)
  % function data = load(fn)
  % Nodder only
  %
  %  data(:,1) = theta
  %  data(:,2) = phi
  %  data(:,3) = range
  %  data(:,4) = reflectance


  data = csvread(fn, 1, 0); % skip the first row

  % distane, mirror, tilt, intensity
  %data = data(:,[1 2 4 5]); % remove other things
  % re-arrange
  data = data(:,[2 4 1 5 3]);

end
