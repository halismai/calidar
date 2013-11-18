cntr = [0 0 0]';
%dims = [10 7 3]';
dims = [10 10 5]';

[v f] = createCube; v = bsxfun(@minus, v, mean(v,1));
T = [...
  1 0 0 cntr(1); ...
  0 1 0 cntr(2); ...
  0 0 1 cntr(3); ...
  0 0 0 1] * ...
  [dims(1) 0 0 0; ...
  0 dims(2) 0 0; ...
  0 0 dims(3) 0; 
  0 0 0 1];


pts = T * [v ones(size(v,1),1)]';
pts = pts(1:3,:)';


fp = fopen('cuboid_10x10x5.ply', 'w'); 
fprintf(fp, 'ply\n');
fprintf(fp, 'format ascii 1.0\n');
fprintf(fp, 'element vertex %d\n', size(pts,1));
fprintf(fp, 'property float x\n');
fprintf(fp, 'property float y\n');
fprintf(fp, 'property float z\n');
fprintf(fp, 'element face %d\n', size(f,1));
fprintf(fp, 'property list uint8 int32 vertex_index\n');
fprintf(fp, 'end_header\n');
fprintf(fp, '%f %f %f\n', pts');
fprintf(fp, '4 %d %d %d %d\n', f'-1);
fclose(fp);


