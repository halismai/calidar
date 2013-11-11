if ~exist('data', 'var')
  data = example_sensor.load('~/code/lcc/lcc-nrec-export/test_data.csv');
end

% data from a small room
min_range = 0.35;
max_range = 8.00;

[s1,s2] = example_sensor.split(data, min_range, max_range);
[x1,x2] = example_sensor.actuate_split(s1,s2);



