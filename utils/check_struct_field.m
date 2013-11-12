function s = check_struct_field(s, fname, val)
  assert(isstruct(s));
  if ~has_field(s, fname)
    if nargin < 3
      error('field %s not found and no default value is given',fname);
    end
    s = setfield(s, fname, val);
  end
end

function v = has_field(s, fname)
  v = isfield(s, fname) && ~isempty(getfield(s,fname));
end
