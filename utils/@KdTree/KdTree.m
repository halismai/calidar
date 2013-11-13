classdef KdTree < handle
  % classdef nanoflann < handle
  % 
  % KdTree class for Matlab (tested with 3D data)
  %
  % Data must be (dim)x(Npts). The class will automatically use the nanoflann
  % wrapper if it can find nanoflann_mex.$(MEX_SUFFIX) on the path
  %
  % NOTE 
  % To be consistent with Matlab's kdtree, inputs and outputs are all Euclidean
  % distances (NOT squared Euclidean)
  %
  % NOTE 
  % We will use single precision data for the tree
  %
  % Last modified: Tue 12 Nov 2013 03:58:32 PM EST
  % <halismai@cs.cmu.edu>

  methods
    function obj = KdTree(data, max_leaf_size)
      %function obj = KdTree(data, max_leaf_size)
      %
      % INPUT 
      %    data 3xN or 4xN (homogeneous) array of points 
      %    max_leaf_size maximum leaf size for nanoflann [default=10]
      %    
      %
      narginchk(1,2); if nargin < 2, max_leaf_size = 10; end

      assert(size(data,1)==3  || size(data,1)==4, ...
        'data must be 3xN or 4xN\n');

      obj.data = single(data);
      obj.use_nanoflann = (3==exist(strcat('nanoflann_mex.',mexext),'file'));
      if obj.use_nanoflann 
        %fprintf('will use nanoflann_mex\n');
        obj.tree_handle_ = nanoflann_mex('new', obj.data, max_leaf_size);
      else 
        %fprintf('will use Matlab built-in kdtree\n');
        obj.tree_handle_ = createns(obj.data', 'NSMethod', 'kdtree');
      end 
    end % KdTree

    function delete(obj)
      if obj.use_nanoflann, nanoflann_mex('delete', obj.tree_handle_); end
    end


    function [inds,dists] = knnsearch(obj, query, k)
      %function [inds,dists] = knnsearch(obj, query, k)
      % 
      % INPUT 
      %   query matrix of points, same dimension as data used to build the
      %   tree. Query points are in columns, i.e. 3xM
      %
      %   k     number of k neighbors to search for [default=1]
      narginchk(2,3); if nargin < 3, k=1; end
      if obj.use_nanoflann
        [inds,dists] = nanoflann_mex('knnsearch', ...
          obj.tree_handle_, single(query), k);
      else 
        [inds,dists] = knnsearch(obj.tree_handle_, single(query)','k', k);
      end 
    end  % knnsearch

    function [inds,dists] = rangesearch(obj, query, r)
      % function [inds,dists] = rangesearch(obj, query, r)
      %  
      % INPUT 
      %   query matrix of M pts, 
      %   r     radius search r can be a single number of a different number per
      %   point 
      %
      % OUTPUT 
      %   inds,dists cell arrays of indices and distances to the nearest
      %   neighbors
      narginchk(3,3);
      assert(isscalar(r) || isvector(r), 'r must be a vector/scalar');
      if length(r) > 1
        assert(length(r) == size(query,2), 'r length mismatch');
      end 

      if obj.use_nanoflann
        query 
        r
        [inds,dists] = nanoflann_mex('rangesearch', ...
          obj.tree_handle_, single(query), single(r));
      else
        if length(r) == size(query,2)
          % a different range search per point. This loop will be very slow in
          % Matlab for a large number of points 
          Q = single(query);
          inds = cell(1, length(r));
          dists = cell(1, length(r));
          parfor i=1:length(r) 
            [ii,dd] = rangesearch(obj.tree_handle_, Q(:,i)', r(i));
            inds{i} = ii{:};
            dists{i} = dd{:};
          end 
        else % search with the same radius 
          [inds,dists] = rangesearch(obj.tree_handle_, single(query)', r);
        end 
      end

    end 

  end % methods

  
  properties (SetAccess = private, Hidden = true)
    tree_handle_; % this will point to nanoflann class if available, otherwise
                  % it will use Matlab's statistics toolbox 
  end 

  properties (SetAccess = private)
    use_nanoflann; % true if nanoflann_mex is found
    data;
  end 
end % nanoflann
