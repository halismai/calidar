function data = load(fn)
  % function data = load(fn)
  % 
  % Loads sensor data from file. For this example sensor, the file is a CSV file
  % with the following 
  %
  %  theta phi range
  %
  %  where, 
  %    theta is the mirror angle in radians
  %    phi   is the spining angle (interpolated) in radians
  %    range measured range in meters
  %
  % INPUT 
  %   fn the filename (CSV file)
  %
  % OUTPUT
  %   data the loaded sensor data 
  %     data(:,1) = theta 
  %     data(:,2) = phi 
  %     data(:,3) = range
  %     data(:,4) = reflectance (not used for calibration purposes)
  
  % Hatem Alismail <halismai@cs.cmu.edu> 
  % Last modified: Mon 11 Nov 2013 05:16:51 PM EST
  %
  % License: See LICENSE file

  data = csvread(fn); 
end  % load
