function s = check_struct_field(s, fname, val)
  %function s = check_struct_field(s, fname, val)

  % Hatem Alismail <halismai@cs.cmu.edu>
  % Last modified: Fri 15 Feb 2013 03:53:56 PM EST


  assert(isstruct(s));
  if ~has_field(s, fname)
    if nargin < 3
      error('field %s not found and no default value is given',fname);
    end
    s.(fname) = val;
  end
end

function v = has_field(s, fname)
  v = isfield(s, fname) && ~isempty(s.(fname));
end

